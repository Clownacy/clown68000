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
	SplitOpcode opcode;
	Instruction instruction;
	cc_u32f operation_size, msb_bit_index;
	DecodedAddressMode source_decoded_address_mode, destination_decoded_address_mode;
	cc_u32f source_value, destination_value, result_value;
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


/* Microcode Operations */

static void SupervisorCheck(Stuff* const stuff)
{
	/* Only allow this instruction in supervisor mode. */
	if ((stuff->state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(stuff, 8);
		longjmp(stuff->exception.context, 1);
	}
}

static void SetSize_Byte(Stuff* const stuff)
{
	stuff->operation_size = 1;
}

static void SetSize_Word(Stuff* const stuff)
{
	stuff->operation_size = 2;
}

static void SetSize_Longword(Stuff* const stuff)
{
	stuff->operation_size = 4;
}

static void SetSize_LongwordRegisterByteMemory(Stuff* const stuff)
{
	stuff->operation_size = stuff->opcode.primary_address_mode == ADDRESS_MODE_DATA_REGISTER ? 4 : 1;
}

static void SetSize_Move(Stuff* const stuff)
{
	switch (stuff->opcode.raw & 0x3000)
	{
		case 0x1000:
			stuff->operation_size = 1;
			break;

		case 0x2000:
			stuff->operation_size = 4; /* Yup, this isn't a typo. */
			break;

		case 0x3000:
			stuff->operation_size = 2;
			break;
	}
}

static void SetSize_Ext(Stuff* const stuff)
{
	stuff->operation_size = (stuff->opcode.raw & 0x0040) != 0 ? 4 : 2;
}

static void SetSize_Standard(Stuff* const stuff)
{
	stuff->operation_size = 1 << stuff->opcode.bits_6_and_7;
}

static void SetMSBBitIndex(Stuff* const stuff)
{
	stuff->msb_bit_index = stuff->operation_size * 8 - 1;
}

static void DecodeSource_ImmediateData(Stuff* const stuff)
{
	/* Immediate value (any size). */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_SPECIAL, ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE);
}

static void DecodeSource_DataRegisterSecondary(Stuff* const stuff)
{
	/* Secondary data register. */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_DATA_REGISTER, stuff->opcode.secondary_register);
}

static void DecodeSource_ImmediateDataByte(Stuff* const stuff)
{
	/* Immediate value (byte). */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, 1, ADDRESS_MODE_SPECIAL, ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE);
}

static void DecodeSource_MemoryAddressPrimary(Stuff* const stuff)
{
	/* Memory address */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, 0, stuff->opcode.primary_address_mode, stuff->opcode.primary_register); /* 0 is a special value that means to obtain the address rather than the data at that address. */
}

static void DecodeSource_StatusRegister(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, 0, ADDRESS_MODE_STATUS_REGISTER, 0);
}

static void DecodeSource_ImmediateDataWord(Stuff* const stuff)
{
	/* Immediate value (word). */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, 2, ADDRESS_MODE_SPECIAL, ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE);
}

static void DecodeSource_BCDX(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->operation_size, (stuff->opcode.raw & 0x0008) != 0 ? ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT : ADDRESS_MODE_DATA_REGISTER, stuff->opcode.primary_register);
}

static void DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode(Stuff* const stuff)
{
	/* Primary address mode or secondary data register, based on direction bit. */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->operation_size, stuff->opcode.bit_8 ? ADDRESS_MODE_DATA_REGISTER : stuff->opcode.primary_address_mode, stuff->opcode.bit_8 ? stuff->opcode.secondary_register : stuff->opcode.primary_register);
}

static void DecodeSource_PrimaryAddressModeSized(Stuff* const stuff)
{
	/* Word or longword based on bit 8. */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->opcode.bit_8 ? 4 : 2, stuff->opcode.primary_address_mode, stuff->opcode.primary_register);
}

static void DecodeSource_AddressRegisterPrimaryPostIncrement(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT, stuff->opcode.primary_register);
}

static void DecodeSource_PrimaryAddressMode(Stuff* const stuff)
{
	/* Primary address mode. */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, stuff->operation_size, stuff->opcode.primary_address_mode, stuff->opcode.primary_register);
}

static void DecodeSource_PrimaryAddressModeWord(Stuff* const stuff)
{
	/* Primary address mode, hardcoded to word-size. */
	DecodeAddressMode(stuff, &stuff->source_decoded_address_mode, 2, stuff->opcode.primary_address_mode, stuff->opcode.primary_register);
}

static void DecodeDestination_DataRegisterPrimary(Stuff* const stuff)
{
	/* Data register (primary) */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_DATA_REGISTER, stuff->opcode.primary_register);
}

static void DecodeDestination_DataRegisterSecondary(Stuff* const stuff)
{
	/* Data register (secondary) */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_DATA_REGISTER, stuff->opcode.secondary_register);
}

static void DecodeDestination_AddressRegisterSecondary(Stuff* const stuff)
{
	/* Address register (secondary) */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_ADDRESS_REGISTER, stuff->opcode.secondary_register);
}

static void DecodeDestination_SecondaryAddressMode(Stuff* const stuff)
{
	/* Secondary address mode */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, stuff->opcode.secondary_address_mode, stuff->opcode.secondary_register);
}

static void DecodeDestination_BCDX(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, (stuff->opcode.raw & 0x0008) != 0 ? ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT : ADDRESS_MODE_DATA_REGISTER, stuff->opcode.secondary_register);
}

static void DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode(Stuff* const stuff)
{
	/* Primary address mode or secondary data register, based on direction bit */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, stuff->opcode.bit_8 ? stuff->opcode.primary_address_mode : ADDRESS_MODE_DATA_REGISTER, stuff->opcode.bit_8 ? stuff->opcode.primary_register : stuff->opcode.secondary_register);
}

static void DecodeDestination_AddressRegisterSecondaryFull(Stuff* const stuff)
{
	/* Full secondary address register */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, 4, ADDRESS_MODE_ADDRESS_REGISTER, stuff->opcode.secondary_register);
}

static void DecodeDestination_AddressRegisterSecondaryPostIncrement(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT, stuff->opcode.secondary_register);
}

static void DecodeDestination_PrimaryAddressMode(Stuff* const stuff)
{
	/* Using primary address mode */
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->operation_size, stuff->opcode.primary_address_mode, stuff->opcode.primary_register);
}

static void DecodeDestination_ConditionCodeRegister(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, 0, ADDRESS_MODE_CONDITION_CODE_REGISTER, 0);
}

static void DecodeDestination_StatusRegister(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, 0, ADDRESS_MODE_STATUS_REGISTER, 0);
}

static void DecodeDestination_MOVEM(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, 0, stuff->opcode.primary_address_mode, stuff->opcode.primary_register); /* 0 is a special value that means to obtain the address rather than the data at that address. */
}

static void DecodeDestination_MOVEP(Stuff* const stuff)
{
	DecodeAddressMode(stuff, &stuff->destination_decoded_address_mode, 0, ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT, stuff->opcode.primary_register); /* 0 is a special value that means to obtain the address rather than the data at that address. */
}

static void ReadSource(Stuff* const stuff)
{
	stuff->source_value = GetValueUsingDecodedAddressMode(stuff, &stuff->source_decoded_address_mode);
}

static void ReadDestination(Stuff* const stuff)
{
	stuff->destination_value = GetValueUsingDecodedAddressMode(stuff, &stuff->destination_decoded_address_mode);
}

static void WriteDestination(Stuff* const stuff)
{
	SetValueUsingDecodedAddressMode(stuff, &stuff->destination_decoded_address_mode, stuff->result_value);
}

static void Carry_StandardCarry(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;
	stuff->state->status_register |= (((stuff->source_value & stuff->destination_value) | ((stuff->source_value | stuff->destination_value) & ~stuff->result_value)) >> (stuff->msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
}

static void Carry_StandardBorrow(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;
	stuff->state->status_register |= (((stuff->source_value & ~stuff->destination_value) | ((stuff->source_value | ~stuff->destination_value) & stuff->result_value)) >> (stuff->msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
}

static void Carry_NEG(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;
	stuff->state->status_register |= ((stuff->destination_value | stuff->result_value) >> (stuff->msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
}

static void Carry_Clear(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;
}

static void Overflow_ADD(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_OVERFLOW;
	stuff->state->status_register |= (((stuff->source_value & stuff->destination_value & ~stuff->result_value) | (~stuff->source_value & ~stuff->destination_value & stuff->result_value)) >> (stuff->msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
}

static void Overflow_SUB(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_OVERFLOW;
	stuff->state->status_register |= (((~stuff->source_value & stuff->destination_value & ~stuff->result_value) | (stuff->source_value & ~stuff->destination_value & stuff->result_value)) >> (stuff->msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
}

static void Overflow_NEG(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_OVERFLOW;
	stuff->state->status_register |= ((stuff->destination_value & stuff->result_value) >> (stuff->msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
}

static void Overflow_Clear(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_OVERFLOW;
}

static void Zero_ClearIfNonZeroUnaffectedOtherwise(Stuff* const stuff)
{
	/* Cleared if the result is nonzero; unchanged otherwise */
	stuff->state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff->result_value & (0xFFFFFFFF >> (32 - stuff->msb_bit_index - 1))) == 0));
}

static void Zero_SetIfZeroClearOtherwise(Stuff* const stuff)
{
	/* Standard behaviour: set if result is zero; clear otherwise */
	stuff->state->status_register &= ~CONDITION_CODE_ZERO;
	stuff->state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff->result_value & (0xFFFFFFFF >> (32 - stuff->msb_bit_index - 1))) == 0));
}

static void Negative_SetIfNegativeClearOtherwise(Stuff* const stuff)
{
	/* Standard behaviour: set if result value is negative; clear otherwise */
	stuff->state->status_register &= ~CONDITION_CODE_NEGATIVE;
	stuff->state->status_register |= CONDITION_CODE_NEGATIVE & (stuff->result_value >> (stuff->msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
}

static void Extend_SetToCarry(Stuff* const stuff)
{
	/* Standard behaviour: set to CARRY */
	stuff->state->status_register &= ~CONDITION_CODE_EXTEND;
	stuff->state->status_register |= CONDITION_CODE_EXTEND & (0 - ((stuff->state->status_register & CONDITION_CODE_CARRY) != 0));
}

static void Action_OR(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_OR;
}

static void Action_AND(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_AND;
}

static void Action_SUBA(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SUBA;
}

static void Action_SUBQ(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SUBQ;
}

static void Action_SUB(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SUB;
}

static void Action_ADDA(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ADDA;
}

static void Action_ADDQ(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ADDQ;
}

static void Action_ADD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ADD;
}

static void Action_EOR(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_EOR;
}

static void Action_BCHG(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BCHG;
}

static void Action_BCLR(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BCLR;
}

static void Action_BSET(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BSET;
}

static void Action_BTST(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BTST;
}

static void Action_MOVEP(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MOVEP;
}

static void Action_MOVEA(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MOVEA;
}

static void Action_MOVE(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MOVE;
}

static void Action_LINK(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_LINK;
}

static void Action_UNLK(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_UNLK;
}

static void Action_NEGX(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_NEGX;
}

static void Action_CLR(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_CLR;
}

static void Action_NEG(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_NEG;
}

static void Action_NOT(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_NOT;
}

static void Action_EXT(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_EXT;
}

static void Action_NBCD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_NBCD;
}

static void Action_SWAP(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SWAP;
}

static void Action_PEA(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_PEA;
}

static void Action_ILLEGAL(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ILLEGAL;
}

static void Action_TAS(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_TAS;
}

static void Action_TRAP(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_TRAP;
}

static void Action_MOVE_USP(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MOVE_USP;
}

static void Action_RESET(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_RESET;
}

static void Action_STOP(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_STOP;
}

static void Action_RTE(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_RTE;
}

static void Action_RTS(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_RTS;
}

static void Action_TRAPV(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_TRAPV;
}

static void Action_RTR(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_RTR;
}

static void Action_JSR(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_JSR;
}

static void Action_JMP(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_JMP;
}

static void Action_MOVEM(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MOVEM;
}

static void Action_CHK(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_CHK;
}

static void Action_SCC(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SCC;
}

static void Action_DBCC(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_DBCC;
}

static void Action_BRA_SHORT(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BRA_SHORT;
}

static void Action_BRA_WORD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BRA_WORD;
}

static void Action_BSR_SHORT(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BSR_SHORT;
}

static void Action_BSR_WORD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BSR_WORD;
}

static void Action_BCC_SHORT(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BCC_SHORT;
}

static void Action_BCC_WORD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_BCC_WORD;
}

static void Action_MOVEQ(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MOVEQ;
}

static void Action_DIV(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_DIV;
}

static void Action_SBCD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SBCD;
}

static void Action_SUBX(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SUBX;
}

static void Action_MUL(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_MUL;
}

static void Action_ABCD(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ABCD;
}

static void Action_EXG(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_EXG;
}

static void Action_ADDX(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ADDX;
}

static void Action_ASD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ASD_MEMORY;
}

static void Action_ASD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ASD_REGISTER;
}

static void Action_LSD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_LSD_MEMORY;
}

static void Action_LSD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_LSD_REGISTER;
}

static void Action_ROD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ROD_MEMORY;
}

static void Action_ROD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ROD_REGISTER;
}

static void Action_ROXD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ROXD_MEMORY;
}

static void Action_ROXD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_ROXD_REGISTER;
}

static void Action_UNIMPLEMENTED_1(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1;
}

static void Action_UNIMPLEMENTED_2(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2;
}

static void Action_NOP(Stuff* const stuff)
{
	(void)stuff;

	DO_INSTRUCTION_ACTION_NOP;
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

#include "m68k/gen.c"

/* This MUST match the order of the `Instruction` enum! */
static void (* const * const microcode[])(Stuff* const stuff) = {
	microcode_ABCD,
	microcode_ADD,
	microcode_ADDA,
	microcode_ADDAQ,
	microcode_ADDI,
	microcode_ADDQ,
	microcode_ADDX,
	microcode_AND,
	microcode_ANDI,
	microcode_ANDI_TO_CCR,
	microcode_ANDI_TO_SR,
	microcode_ASD_MEMORY,
	microcode_ASD_REGISTER,
	microcode_BCC_SHORT,
	microcode_BCC_WORD,
	microcode_BCHG_DYNAMIC,
	microcode_BCHG_STATIC,
	microcode_BCLR_DYNAMIC,
	microcode_BCLR_STATIC,
	microcode_BRA_SHORT,
	microcode_BRA_WORD,
	microcode_BSET_DYNAMIC,
	microcode_BSET_STATIC,
	microcode_BSR_SHORT,
	microcode_BSR_WORD,
	microcode_BTST_DYNAMIC,
	microcode_BTST_STATIC,
	microcode_CHK,
	microcode_CLR,
	microcode_CMP,
	microcode_CMPA,
	microcode_CMPI,
	microcode_CMPM,
	microcode_DBCC,
	microcode_DIVS,
	microcode_DIVU,
	microcode_EOR,
	microcode_EORI,
	microcode_EORI_TO_CCR,
	microcode_EORI_TO_SR,
	microcode_EXG,
	microcode_EXT,
	microcode_ILLEGAL,
	microcode_JMP,
	microcode_JSR,
	microcode_LEA,
	microcode_LINK,
	microcode_LSD_MEMORY,
	microcode_LSD_REGISTER,
	microcode_MOVE,
	microcode_MOVE_FROM_SR,
	microcode_MOVE_TO_CCR,
	microcode_MOVE_TO_SR,
	microcode_MOVE_USP,
	microcode_MOVEA,
	microcode_MOVEM,
	microcode_MOVEP,
	microcode_MOVEQ,
	microcode_MULS,
	microcode_MULU,
	microcode_NBCD,
	microcode_NEG,
	microcode_NEGX,
	microcode_NOP,
	microcode_NOT,
	microcode_OR,
	microcode_ORI,
	microcode_ORI_TO_CCR,
	microcode_ORI_TO_SR,
	microcode_PEA,
	microcode_RESET,
	microcode_ROD_MEMORY,
	microcode_ROD_REGISTER,
	microcode_ROXD_MEMORY,
	microcode_ROXD_REGISTER,
	microcode_RTE,
	microcode_RTR,
	microcode_RTS,
	microcode_SBCD,
	microcode_SCC,
	microcode_STOP,
	microcode_SUB,
	microcode_SUBA,
	microcode_SUBAQ,
	microcode_SUBI,
	microcode_SUBQ,
	microcode_SUBX,
	microcode_SWAP,
	microcode_TAS,
	microcode_TRAP,
	microcode_TRAPV,
	microcode_TST,
	microcode_UNLK,
	microcode_UNIMPLEMENTED_1,
	microcode_UNIMPLEMENTED_2
};

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
			unsigned int i;

			/* Process new instruction */
			stuff.source_value = stuff.destination_value = stuff.result_value = 0; /* TODO: This is unnecessary - remove this! */

			/* Figure out which instruction this is */
			stuff.instruction = DecodeOpcode(&stuff.opcode, ReadWord(&stuff, state->program_counter));

			/* We already pre-fetched the instruction, so just advance past it. */
			state->instruction_register = stuff.opcode.raw;
			state->program_counter += 2;

			/* Execute the instruction's micrcode */
			for (i = 0; microcode[stuff.instruction][i] != NULL; ++i)
				microcode[stuff.instruction][i](&stuff);

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
