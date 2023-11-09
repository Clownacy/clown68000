/*
    clown68000 - A Motorola 68000 emulator.
    Copyright (C) 2021-2023  Clownacy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*
TODO:

68k memory access order:
https://gendev.spritesmind.net/forum/viewtopic.php?p=36896#p36896

DIVU/DIVS timing:
https://gendev.spritesmind.net/forum/viewtopic.php?p=35569#p35569

Z80 reset stuff:
https://gendev.spritesmind.net/forum/viewtopic.php?p=36118#p36118

ABCD/SBCD quirks:
http://gendev.spritesmind.net/forum/viewtopic.php?f=2&t=1964
*/

#include "clown68000.h"

#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include "clowncommon/clowncommon.h"

#include "m68k/instruction-actions.h"
#include "m68k/instruction.h"
#include "m68k/opcode.h"

/*#define DEBUG_STUFF*/

#ifdef DEBUG_STUFF
#include <stdio.h>
#endif

enum
{
	CONDITION_CODE_CARRY_BIT    = 0,
	CONDITION_CODE_OVERFLOW_BIT = 1,
	CONDITION_CODE_ZERO_BIT     = 2,
	CONDITION_CODE_NEGATIVE_BIT = 3,
	CONDITION_CODE_EXTEND_BIT   = 4,

	CONDITION_CODE_CARRY    = 1 << CONDITION_CODE_CARRY_BIT,
	CONDITION_CODE_OVERFLOW = 1 << CONDITION_CODE_OVERFLOW_BIT,
	CONDITION_CODE_ZERO     = 1 << CONDITION_CODE_ZERO_BIT,
	CONDITION_CODE_NEGATIVE = 1 << CONDITION_CODE_NEGATIVE_BIT,
	CONDITION_CODE_EXTEND   = 1 << CONDITION_CODE_EXTEND_BIT,
	CONDITION_CODE_REGISTER_MASK = CONDITION_CODE_EXTEND | CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO | CONDITION_CODE_OVERFLOW | CONDITION_CODE_CARRY,

	STATUS_INTERRUPT_MASK   = 7 << 8,
	STATUS_SUPERVISOR       = 1 << 13,
	STATUS_TRACE            = 1 << 15,
	STATUS_REGISTER_MASK    = STATUS_TRACE | STATUS_SUPERVISOR | STATUS_INTERRUPT_MASK | CONDITION_CODE_REGISTER_MASK
};

typedef	enum DecodedAddressModeType
{
	DECODED_ADDRESS_MODE_TYPE_REGISTER,
	DECODED_ADDRESS_MODE_TYPE_MEMORY,
	DECODED_ADDRESS_MODE_TYPE_STATUS_REGISTER,
	DECODED_ADDRESS_MODE_TYPE_CONDITION_CODE_REGISTER
} DecodedAddressModeType;

typedef struct DecodedAddressMode
{
	DecodedAddressModeType type;
	union
	{
		struct
		{
			cc_u32l *address;
			cc_u32f operation_size_bitmask;
		} reg;
		struct
		{
			cc_u32f address;
			cc_u8f operation_size_in_bytes;
		} memory;
	} data;
} DecodedAddressMode;

typedef struct Stuff
{
	Clown68000_State *state;
	const Clown68000_ReadWriteCallbacks *callbacks;
	struct
	{
		jmp_buf context;
	} exception;
} Stuff;

/* Error callback. */
/* TODO: Remove this once all instructions are implemented. */

static void (*clown68000_error_callback)(const char *format, va_list arg);

void Clown68000_SetErrorCallback(void (*error_callback)(const char *format, va_list arg))
{
	/* TODO - Shouldn't this use the regular state instead of global state? */
	clown68000_error_callback = error_callback;
}

static void Clown68000_PrintError(const char *format, ...)
{
	if (clown68000_error_callback != NULL)
	{
		va_list args;
		va_start(args, format);
		clown68000_error_callback(format, args);
		va_end(args);
	}
}

/* Exception forward-declarations. */

static void Group1Or2Exception(Stuff *stuff, cc_u16f vector_offset);
static void Group0Exception(Stuff *stuff, cc_u16f vector_offset, cc_u32f access_address, cc_bool is_a_read);

/* Memory reads */

static cc_u32f ReadByte(const Stuff *stuff, cc_u32f address)
{
	const Clown68000_ReadWriteCallbacks* const callbacks = stuff->callbacks;
	const cc_bool odd = (address & 1) != 0;

	return (callbacks->read_callback(callbacks->user_data, (address & 0xFFFFFF) / 2, (cc_bool)!odd, odd) >> (odd ? 0 : 8)) & 0xFF;
}

static cc_u32f ReadWord(Stuff *stuff, cc_u32f address)
{
	const Clown68000_ReadWriteCallbacks* const callbacks = stuff->callbacks;

	if ((address & 1) != 0)
	{
		Group0Exception(stuff, 3, address, cc_true);
		longjmp(stuff->exception.context, 1);
	}

	return callbacks->read_callback(callbacks->user_data, (address & 0xFFFFFF) / 2, cc_true, cc_true);
}

static cc_u32f ReadLongWord(Stuff *stuff, cc_u32f address)
{
	cc_u32f value;

	const Clown68000_ReadWriteCallbacks* const callbacks = stuff->callbacks;

	if ((address & 1) != 0)
	{
		Group0Exception(stuff, 3, address, cc_true);
		longjmp(stuff->exception.context, 1);
	}

	value = 0;
	value |= (cc_u32f)callbacks->read_callback(callbacks->user_data, (address & 0xFFFFFF) / 2 + 0, cc_true, cc_true) << 16;
	value |= (cc_u32f)callbacks->read_callback(callbacks->user_data, (address & 0xFFFFFF) / 2 + 1, cc_true, cc_true) <<  0;

	return value;
}

/* Memory writes */

static void WriteByte(const Stuff *stuff, cc_u32f address, cc_u32f value)
{
	const Clown68000_ReadWriteCallbacks* const callbacks = stuff->callbacks;
	const cc_bool odd = (address & 1) != 0;

	callbacks->write_callback(callbacks->user_data, (address & 0xFFFFFF) / 2, (cc_bool)!odd, odd, value << (odd ? 0 : 8));
}

static void WriteWord(Stuff *stuff, cc_u32f address, cc_u32f value)
{
	const Clown68000_ReadWriteCallbacks* const callbacks = stuff->callbacks;

	if ((address & 1) != 0)
	{
		Group0Exception(stuff, 3, address, cc_false);
		longjmp(stuff->exception.context, 1);
	}

	callbacks->write_callback(callbacks->user_data, (address & 0xFFFFFF) / 2, cc_true, cc_true, value);
}

static void WriteLongWord(Stuff *stuff, cc_u32f address, cc_u32f value)
{
	const Clown68000_ReadWriteCallbacks* const callbacks = stuff->callbacks;

	if ((address & 1) != 0)
	{
		Group0Exception(stuff, 3, address, cc_false);
		longjmp(stuff->exception.context, 1);
	}

	callbacks->write_callback(callbacks->user_data, (address & 0xFFFFFF) / 2 + 0, cc_true, cc_true, value >> 16);
	callbacks->write_callback(callbacks->user_data, (address & 0xFFFFFF) / 2 + 1, cc_true, cc_true, value & 0xFFFF);
}

/* Supervisor mode */

static void SetSupervisorMode(Clown68000_State *state, cc_bool supervisor_mode)
{
	const cc_bool already_supervisor_mode = (state->status_register & STATUS_SUPERVISOR) != 0;

	if (supervisor_mode)
	{
		if (!already_supervisor_mode)
		{
			state->status_register |= STATUS_SUPERVISOR;
			state->user_stack_pointer = state->address_registers[7];
			state->address_registers[7] = state->supervisor_stack_pointer;
		}
	}
	else
	{
		if (already_supervisor_mode)
		{
			state->status_register &= ~STATUS_SUPERVISOR;
			state->supervisor_stack_pointer = state->address_registers[7];
			state->address_registers[7] = state->user_stack_pointer;
		}
	}
}

/* Exceptions */

static void Group1Or2Exception(Stuff *stuff, cc_u16f vector_offset)
{
	Clown68000_State* const state = stuff->state;
	const cc_u16l copy_status_register = state->status_register; /* Preserve the original supervisor bit. */

	/* Exit trace mode. */
	state->status_register &= ~STATUS_TRACE;
	/* Set supervisor bit */
	SetSupervisorMode(stuff->state, cc_true);

	state->address_registers[7] -= 4;
	WriteLongWord(stuff, state->address_registers[7], state->program_counter);
	state->address_registers[7] -= 2;
	WriteWord(stuff, state->address_registers[7], copy_status_register);

	state->program_counter = ReadLongWord(stuff, vector_offset * 4);
}

static void Group0Exception(Stuff *stuff, cc_u16f vector_offset, cc_u32f access_address, cc_bool is_a_read)
{
	Clown68000_State* const state = stuff->state;

	/* If a data or address error occurs during group 0 exception processing, then the CPU halts. */
	if ((state->address_registers[7] & 1) != 0)
	{
		state->halted = cc_true;
	}
	else
	{
		Group1Or2Exception(stuff, vector_offset);

		/* Push extra information to the stack. */
		state->address_registers[7] -= 2;
		WriteWord(stuff, state->address_registers[7], state->instruction_register);
		state->address_registers[7] -= 4;
		WriteLongWord(stuff, state->address_registers[7], access_address);
		state->address_registers[7] -= 2;
		/* TODO - Function code and 'Instruction/Not' bit. */
		/* According to the test suite, there really is a partial phantom copy of the intruction register here. */
		WriteWord(stuff, state->address_registers[7], (state->instruction_register & 0xFFE0) | (is_a_read << 4) | 0xE);
	}
}

/* Misc. utility */

static cc_u32f DecodeMemoryAddressMode(Stuff* const stuff, const unsigned int operation_size_in_bytes, const AddressMode address_mode, const unsigned int address_mode_register)
{
	Clown68000_State* const state = stuff->state;

	cc_u32f address;

	if (address_mode == ADDRESS_MODE_SPECIAL && address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_ABSOLUTE_SHORT)
	{
		/* Absolute short */
		const cc_u32f short_address = ReadWord(stuff, state->program_counter);

		address = CC_SIGN_EXTEND_ULONG(15, short_address);
		state->program_counter += 2;
	}
	else if (address_mode == ADDRESS_MODE_SPECIAL && address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_ABSOLUTE_LONG)
	{
		/* Absolute long */
		address = ReadLongWord(stuff, state->program_counter);
		state->program_counter += 4;
	}
	else if (address_mode == ADDRESS_MODE_SPECIAL && address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE)
	{
		/* Immediate value */
		if (operation_size_in_bytes == 1)
		{
			/* A byte-sized immediate value occupies two bytes of space */
			address = state->program_counter + 1;
			state->program_counter += 2;
		}
		else
		{
			address = state->program_counter;
			state->program_counter += operation_size_in_bytes;
		}
	}
	else if (address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT)
	{
		/* Address register indirect */
		address = state->address_registers[address_mode_register];
	}
	else if (address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)
	{
		/* Address register indirect with predecrement */

		/* The stack pointer moves two bytes instead of one byte, for alignment purposes */
		const cc_u16f increment_decrement_size = (address_mode_register == 7 && operation_size_in_bytes == 1) ? 2 : operation_size_in_bytes;

		state->address_registers[address_mode_register] -= increment_decrement_size;
		address = state->address_registers[address_mode_register];
	}
	else if (address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT)
	{
		/* Address register indirect with postincrement */

		/* The stack pointer moves two bytes instead of one byte, for alignment purposes */
		const cc_u16f increment_decrement_size = (address_mode_register == 7 && operation_size_in_bytes == 1) ? 2 : operation_size_in_bytes;

		address = state->address_registers[address_mode_register];
		state->address_registers[address_mode_register] += increment_decrement_size;
	}
	else
	{
		if (address_mode == ADDRESS_MODE_SPECIAL && (address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_DISPLACEMENT || address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_INDEX))
		{
			/* Program counter used as base address */
			address = state->program_counter;
		}
		else
		{
			/* Address register used as base address */
			address = state->address_registers[address_mode_register];
		}

		if (address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT || (address_mode == ADDRESS_MODE_SPECIAL && address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_DISPLACEMENT))
		{
			/* Add displacement */
			const cc_u32f displacement = ReadWord(stuff, state->program_counter);

			address += CC_SIGN_EXTEND_ULONG(15, displacement);
			state->program_counter += 2;
		}
		else if (address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_INDEX || (address_mode == ADDRESS_MODE_SPECIAL && address_mode_register == ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_INDEX))
		{
			/* Add index register and index literal */
			const cc_u32f extension_word = ReadWord(stuff, state->program_counter);
			const cc_bool is_address_register = (extension_word & 0x8000) != 0;
			const cc_u32f displacement_reg = (extension_word >> 12) & 7;
			const cc_bool is_longword = (extension_word & 0x0800) != 0;
			const cc_u32f displacement_literal_value = CC_SIGN_EXTEND_ULONG(7, extension_word);
			const cc_u32f displacement_reg_value = CC_SIGN_EXTEND_ULONG(is_longword ? 31 : 15, (is_address_register ? state->address_registers : state->data_registers)[displacement_reg]);

			address += displacement_reg_value;
			address += displacement_literal_value;
			state->program_counter += 2;
		}
	}

	return address;
}

static void DecodeAddressMode(Stuff* const stuff, DecodedAddressMode* const decoded_address_mode, const unsigned int operation_size_in_bytes, const AddressMode address_mode, const unsigned int address_mode_register)
{
	Clown68000_State* const state = stuff->state;

	switch (address_mode)
	{
		case ADDRESS_MODE_DATA_REGISTER:
		case ADDRESS_MODE_ADDRESS_REGISTER:
			/* Register */
			decoded_address_mode->type = DECODED_ADDRESS_MODE_TYPE_REGISTER;
			decoded_address_mode->data.reg.address = &(address_mode == ADDRESS_MODE_ADDRESS_REGISTER ? state->address_registers : state->data_registers)[address_mode_register];
			decoded_address_mode->data.reg.operation_size_bitmask = (0xFFFFFFFF >> (32 - operation_size_in_bytes * 8));
			break;

		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_INDEX:
		case ADDRESS_MODE_SPECIAL:
			/* Memory access */
			decoded_address_mode->type = DECODED_ADDRESS_MODE_TYPE_MEMORY;
			decoded_address_mode->data.memory.address = DecodeMemoryAddressMode(stuff, operation_size_in_bytes, address_mode, address_mode_register);
			decoded_address_mode->data.memory.operation_size_in_bytes = (cc_u8f)operation_size_in_bytes;
			break;

		case ADDRESS_MODE_STATUS_REGISTER:
			decoded_address_mode->type = DECODED_ADDRESS_MODE_TYPE_STATUS_REGISTER;
			break;

		case ADDRESS_MODE_CONDITION_CODE_REGISTER:
			decoded_address_mode->type = DECODED_ADDRESS_MODE_TYPE_CONDITION_CODE_REGISTER;
			break;

		case ADDRESS_MODE_NONE:
			assert(cc_false);
			break;
	}
}

static cc_u32f GetValueUsingDecodedAddressMode(Stuff *stuff, DecodedAddressMode *decoded_address_mode)
{
	cc_u32f value = 0;

	Clown68000_State* const state = stuff->state;

	switch (decoded_address_mode->type)
	{
		case DECODED_ADDRESS_MODE_TYPE_REGISTER:
			value = *decoded_address_mode->data.reg.address & decoded_address_mode->data.reg.operation_size_bitmask;
			break;

		case DECODED_ADDRESS_MODE_TYPE_MEMORY:
		{
			const cc_u32f address = decoded_address_mode->data.memory.address;

			switch (decoded_address_mode->data.memory.operation_size_in_bytes)
			{
				case 0:
					value = address;
					break;

				case 1:
					value = ReadByte(stuff, address);
					break;

				case 2:
					value = ReadWord(stuff, address);
					break;

				case 4:
					value = ReadLongWord(stuff, address);
					break;
			}

			break;
		}

		case DECODED_ADDRESS_MODE_TYPE_STATUS_REGISTER:
			value = state->status_register;
			break;

		case DECODED_ADDRESS_MODE_TYPE_CONDITION_CODE_REGISTER:
			value = state->status_register & 0xFF;
			break;
	}

	return value;
}

static void SetValueUsingDecodedAddressMode(Stuff *stuff, DecodedAddressMode *decoded_address_mode, cc_u32f value)
{
	Clown68000_State* const state = stuff->state;

	switch (decoded_address_mode->type)
	{
		case DECODED_ADDRESS_MODE_TYPE_REGISTER:
		{
			const cc_u32f destination_value = *decoded_address_mode->data.reg.address;
			const cc_u32f operation_size_bitmask = decoded_address_mode->data.reg.operation_size_bitmask;
			*decoded_address_mode->data.reg.address = (value & operation_size_bitmask) | (destination_value & ~operation_size_bitmask);
			break;
		}

		case DECODED_ADDRESS_MODE_TYPE_MEMORY:
		{
			const cc_u32f address = decoded_address_mode->data.memory.address;

			switch (decoded_address_mode->data.memory.operation_size_in_bytes)
			{
				case 0:
					/* This should never happen. */
					assert(cc_false);
					break;

				case 1:
					WriteByte(stuff, address, value);
					break;

				case 2:
					WriteWord(stuff, address, value);
					break;

				case 4:
					WriteLongWord(stuff, address, value);
					break;
			}

			break;
		}

		case DECODED_ADDRESS_MODE_TYPE_STATUS_REGISTER:
			SetSupervisorMode(stuff->state, (value & STATUS_SUPERVISOR) != 0);
			state->status_register = value & STATUS_REGISTER_MASK;

			break;

		case DECODED_ADDRESS_MODE_TYPE_CONDITION_CODE_REGISTER:
			state->status_register = (state->status_register & ~CONDITION_CODE_REGISTER_MASK) | (value & CONDITION_CODE_REGISTER_MASK);
			break;
	}
}

static cc_bool IsOpcodeConditionTrue(Clown68000_State *state, cc_u16f opcode)
{
	const cc_bool carry = (state->status_register & CONDITION_CODE_CARRY) != 0;
	const cc_bool overflow = (state->status_register & CONDITION_CODE_OVERFLOW) != 0;
	const cc_bool zero = (state->status_register & CONDITION_CODE_ZERO) != 0;
	const cc_bool negative = (state->status_register & CONDITION_CODE_NEGATIVE) != 0;

	switch ((opcode >> 8) & 0xF)
	{
		case 0x0:
			/* True */
			return cc_true;

		case 0x1:
			/* False */
			return cc_false;

		case 0x2:
			/* Higher */
			return !carry && !zero;

		case 0x3:
			/* Lower or same */
			return carry || zero;

		case 0x4:
			/* Carry clear */
			return !carry;

		case 0x5:
			/* Carry set */
			return carry;

		case 0x6:
			/* Not equal */
			return !zero;

		case 0x7:
			/* Equal */
			return zero;

		case 0x8:
			/* Overflow clear */
			return !overflow;

		case 0x9:
			/* Overflow set */
			return overflow;

		case 0xA:
			/* Plus */
			return !negative;

		case 0xB:
			/* Minus */
			return negative;

		case 0xC:
			/* Greater or equal */
			return negative == overflow;

		case 0xD:
			/* Less than */
			return negative != overflow;

		case 0xE:
			/* Greater than */
			return !zero && negative == overflow;

		case 0xF:
			/* Less or equal */
			return zero || negative != overflow;
	}

	assert(cc_false);
	return cc_false;
}


/* API */

void Clown68000_Reset(Clown68000_State *state, const Clown68000_ReadWriteCallbacks *callbacks)
{
	Stuff stuff;

	stuff.state = state;
	stuff.callbacks = callbacks;

	if (!setjmp(stuff.exception.context))
	{
		state->halted = cc_false;

		/* Disable trace mode. */
		state->status_register &= ~STATUS_TRACE;
		/* Set interrupt mask to 7 */
		state->status_register |= 0x0700;
		/* Set supervisor bit */
		SetSupervisorMode(state, cc_true);

		state->address_registers[7] = ReadLongWord(&stuff, 0);
		state->program_counter = ReadLongWord(&stuff, 4);
	}
}

void Clown68000_Interrupt(Clown68000_State *state, const Clown68000_ReadWriteCallbacks *callbacks, cc_u16f level)
{
	Stuff stuff;

	stuff.state = state;
	stuff.callbacks = callbacks;

	if (!setjmp(stuff.exception.context))
	{
		assert(level >= 1 && level <= 7);

		if (level == 7 || level > (((cc_u16f)state->status_register >> 8) & 7))
		{
			Group1Or2Exception(&stuff, 24 + level);

			/* Set interrupt mask set to current level */
			state->status_register &= ~STATUS_INTERRUPT_MASK;
			state->status_register |= level << 8;
		}
	}
}

void Clown68000_DoCycle(Clown68000_State *state, const Clown68000_ReadWriteCallbacks *callbacks)
{
	if (state->halted)
	{
		/* Nope, we're not doing anything. */
	}
	else if (state->cycles_left_in_instruction != 0)
	{
		/* Wait for current instruction to finish. */
		--state->cycles_left_in_instruction;
	}
	else
	{
		/* Initialise closure and exception stuff. */
		Stuff stuff;

		stuff.state = state;
		stuff.callbacks = callbacks;

		if (!setjmp(stuff.exception.context))
		{
			/* Process new instruction */
			SplitOpcode opcode;
			DecodedAddressMode source_decoded_address_mode, destination_decoded_address_mode;
			cc_u32f source_value, destination_value, result_value;
			DecodedOpcode decoded_opcode;
			cc_u32f msb_bit_index;

			source_value = destination_value = result_value = 0;

			/* Figure out which instruction this is */
			DecodeOpcode(&decoded_opcode, &opcode, ReadWord(&stuff, state->program_counter));

			/* We already pre-fetched the instruction, so just advance past it. */
			state->instruction_register = opcode.raw;
			state->program_counter += 2;

			msb_bit_index = (decoded_opcode.size * 8 - 1);

			switch (decoded_opcode.instruction)
			{
				#include "m68k/gen.c"
			}

		#ifdef DEBUG_STUFF
			{
				static const char* const instruction_strings[] = {
					"INSTRUCTION_ABCD",
					"INSTRUCTION_ADD",
					"INSTRUCTION_ADDA",
					"INSTRUCTION_ADDAQ",
					"INSTRUCTION_ADDI",
					"INSTRUCTION_ADDQ",
					"INSTRUCTION_ADDX",
					"INSTRUCTION_AND",
					"INSTRUCTION_ANDI",
					"INSTRUCTION_ANDI_TO_CCR",
					"INSTRUCTION_ANDI_TO_SR",
					"INSTRUCTION_ASD_MEMORY",
					"INSTRUCTION_ASD_REGISTER",
					"INSTRUCTION_BCC_SHORT",
					"INSTRUCTION_BCC_WORD",
					"INSTRUCTION_BCHG_DYNAMIC",
					"INSTRUCTION_BCHG_STATIC",
					"INSTRUCTION_BCLR_DYNAMIC",
					"INSTRUCTION_BCLR_STATIC",
					"INSTRUCTION_BRA_SHORT",
					"INSTRUCTION_BRA_WORD",
					"INSTRUCTION_BSET_DYNAMIC",
					"INSTRUCTION_BSET_STATIC",
					"INSTRUCTION_BSR_SHORT",
					"INSTRUCTION_BSR_WORD",
					"INSTRUCTION_BTST_DYNAMIC",
					"INSTRUCTION_BTST_STATIC",
					"INSTRUCTION_CHK",
					"INSTRUCTION_CLR",
					"INSTRUCTION_CMP",
					"INSTRUCTION_CMPA",
					"INSTRUCTION_CMPI",
					"INSTRUCTION_CMPM",
					"INSTRUCTION_DBCC",
					"INSTRUCTION_DIVS",
					"INSTRUCTION_DIVU",
					"INSTRUCTION_EOR",
					"INSTRUCTION_EORI",
					"INSTRUCTION_EORI_TO_CCR",
					"INSTRUCTION_EORI_TO_SR",
					"INSTRUCTION_EXG",
					"INSTRUCTION_EXT",
					"INSTRUCTION_ILLEGAL",
					"INSTRUCTION_JMP",
					"INSTRUCTION_JSR",
					"INSTRUCTION_LEA",
					"INSTRUCTION_LINK",
					"INSTRUCTION_LSD_MEMORY",
					"INSTRUCTION_LSD_REGISTER",
					"INSTRUCTION_MOVE",
					"INSTRUCTION_MOVE_FROM_SR",
					"INSTRUCTION_MOVE_TO_CCR",
					"INSTRUCTION_MOVE_TO_SR",
					"INSTRUCTION_MOVE_USP",
					"INSTRUCTION_MOVEA",
					"INSTRUCTION_MOVEM",
					"INSTRUCTION_MOVEP",
					"INSTRUCTION_MOVEQ",
					"INSTRUCTION_MULS",
					"INSTRUCTION_MULU",
					"INSTRUCTION_NBCD",
					"INSTRUCTION_NEG",
					"INSTRUCTION_NEGX",
					"INSTRUCTION_NOP",
					"INSTRUCTION_NOT",
					"INSTRUCTION_OR",
					"INSTRUCTION_ORI",
					"INSTRUCTION_ORI_TO_CCR",
					"INSTRUCTION_ORI_TO_SR",
					"INSTRUCTION_PEA",
					"INSTRUCTION_RESET",
					"INSTRUCTION_ROD_MEMORY",
					"INSTRUCTION_ROD_REGISTER",
					"INSTRUCTION_ROXD_MEMORY",
					"INSTRUCTION_ROXD_REGISTER",
					"INSTRUCTION_RTE",
					"INSTRUCTION_RTR",
					"INSTRUCTION_RTS",
					"INSTRUCTION_SBCD",
					"INSTRUCTION_SCC",
					"INSTRUCTION_STOP",
					"INSTRUCTION_SUB",
					"INSTRUCTION_SUBA",
					"INSTRUCTION_SUBAQ",
					"INSTRUCTION_SUBI",
					"INSTRUCTION_SUBQ",
					"INSTRUCTION_SUBX",
					"INSTRUCTION_SWAP",
					"INSTRUCTION_TAS",
					"INSTRUCTION_TRAP",
					"INSTRUCTION_TRAPV",
					"INSTRUCTION_TST",
					"INSTRUCTION_UNLK",

					"INSTRUCTION_UNIMPLEMENTED_1",
					"INSTRUCTION_UNIMPLEMENTED_2"
				};

				fprintf(stderr, "0x%.8" CC_PRIXLEAST32 " - %s\n", state->program_counter, instruction_strings[instruction]);
				state->program_counter |= 0; /* Something to latch a breakpoint onto */
			}
		#endif
		}
	}
}
