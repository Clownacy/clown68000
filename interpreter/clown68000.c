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

Address error behaviour:
https://github.com/TomHarte/ProcessorTests/issues/28

*/

#include "clown68000.h"

#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include "../common/instruction.h"
#include "../common/opcode.h"

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
	cc_u32f operation_size, msb_bit_index;
	DecodedAddressMode source_decoded_address_mode, destination_decoded_address_mode;
	cc_u32f source_value, destination_value, result_value;
} Stuff;

/* Error callback. */
/* TODO: Remove this once all instructions are implemented. */

static void (*clown68000_error_callback)(void *user_data, const char *format, va_list arg);
static void *clown68000_error_callback_user_data;

void Clown68000_SetErrorCallback(void (*error_callback)(void *user_data, const char *format, va_list arg), const void* const user_data)
{
	clown68000_error_callback = error_callback;
	clown68000_error_callback_user_data = (void*)user_data;
}

CC_ATTRIBUTE_PRINTF(1, 2) static void Clown68000_PrintError(const char *format, ...)
{
	if (clown68000_error_callback != NULL)
	{
		va_list args;
		va_start(args, format);
		clown68000_error_callback(clown68000_error_callback_user_data, format, args);
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

	callbacks->write_callback(callbacks->user_data, (address & 0xFFFFFF) / 2, (cc_bool)!odd, odd, value | value << 8);
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
		case 0x0000: /* TODO: Verify this. */
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
	static const cc_u8l sizes[] = {1, 2, 4, 4}; /* TODO: Verify that last one... */
	stuff->operation_size = sizes[stuff->opcode.bits_6_and_7];
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
	stuff->source_decoded_address_mode.type = DECODED_ADDRESS_MODE_TYPE_STATUS_REGISTER;
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
	stuff->destination_decoded_address_mode.type = DECODED_ADDRESS_MODE_TYPE_CONDITION_CODE_REGISTER;
}

static void DecodeDestination_StatusRegister(Stuff* const stuff)
{
	stuff->destination_decoded_address_mode.type = DECODED_ADDRESS_MODE_TYPE_STATUS_REGISTER;
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
	stuff->result_value = stuff->destination_value | stuff->source_value;
}

static void Action_AND(Stuff* const stuff)
{
	stuff->result_value = stuff->destination_value & stuff->source_value;
}

static void Action_SUB(Stuff* const stuff)
{
	stuff->result_value = stuff->destination_value - stuff->source_value;
}

static void Action_SUBA(Stuff* const stuff)
{
	if (!stuff->opcode.bit_8)
		stuff->source_value = CC_SIGN_EXTEND_ULONG(15, stuff->source_value);

	Action_SUB(stuff);
}

static void Action_SUBQ(Stuff* const stuff)
{
	stuff->source_value = ((stuff->opcode.secondary_register - 1u) & 7u) + 1u; /* A little math trick to turn 0 into 8. */
	Action_SUB(stuff);
}

static void Action_ADD(Stuff* const stuff)
{
	stuff->result_value = stuff->destination_value + stuff->source_value;
}

static void Action_ADDA(Stuff* const stuff)
{
	if (!stuff->opcode.bit_8)
		stuff->source_value = CC_SIGN_EXTEND_ULONG(15, stuff->source_value);

	Action_ADD(stuff);
}

static void Action_ADDQ(Stuff* const stuff)
{
	stuff->source_value = ((stuff->opcode.secondary_register - 1u) & 7u) + 1u; /* A little math trick to turn 0 into 8. */
	Action_ADD(stuff);
}

static void Action_EOR(Stuff* const stuff)
{
	stuff->result_value = stuff->destination_value ^ stuff->source_value;
}

static void Action_BTST(Stuff* const stuff)
{
	/* Modulo the source value */
	stuff->source_value &= stuff->msb_bit_index;

	/* Set the zero flag to the specified bit */
	stuff->state->status_register &= ~CONDITION_CODE_ZERO;
	stuff->state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff->destination_value & (1ul << stuff->source_value)) == 0));
}

static void Action_BCHG(Stuff* const stuff)
{
	Action_BTST(stuff);
	stuff->result_value = stuff->destination_value ^ (1ul << stuff->source_value);
}

static void Action_BCLR(Stuff* const stuff)
{
	Action_BTST(stuff);
	stuff->result_value = stuff->destination_value & ~(1ul << stuff->source_value);
}

static void Action_BSET(Stuff* const stuff)
{
	Action_BTST(stuff);
	stuff->result_value = stuff->destination_value | (1ul << stuff->source_value);
}

static void Action_MOVEP(Stuff* const stuff)
{
	Clown68000_State* const state = stuff->state;

	switch (stuff->opcode.bits_6_and_7)
	{
		case 0:
			/* Memory to register (word) */
			state->data_registers[stuff->opcode.secondary_register] &= ~0xFFFFul;
			state->data_registers[stuff->opcode.secondary_register] |= ReadByte(stuff, stuff->destination_value + 2 * 0) << 8 * 1;
			state->data_registers[stuff->opcode.secondary_register] |= ReadByte(stuff, stuff->destination_value + 2 * 1) << 8 * 0;
			break;

		case 1:
			/* Memory to register (longword) */
			state->data_registers[stuff->opcode.secondary_register] = 0;
			state->data_registers[stuff->opcode.secondary_register] |= ReadByte(stuff, stuff->destination_value + 2 * 0) << 8 * 3;
			state->data_registers[stuff->opcode.secondary_register] |= ReadByte(stuff, stuff->destination_value + 2 * 1) << 8 * 2;
			state->data_registers[stuff->opcode.secondary_register] |= ReadByte(stuff, stuff->destination_value + 2 * 2) << 8 * 1;
			state->data_registers[stuff->opcode.secondary_register] |= ReadByte(stuff, stuff->destination_value + 2 * 3) << 8 * 0;
			break;

		case 2:
			/* Register to memory (word) */
			WriteByte(stuff, stuff->destination_value + 2 * 0, (state->data_registers[stuff->opcode.secondary_register] >> 8 * 1) & 0xFF);
			WriteByte(stuff, stuff->destination_value + 2 * 1, (state->data_registers[stuff->opcode.secondary_register] >> 8 * 0) & 0xFF);
			break;

		case 3:
			/* Register to memory (longword) */
			WriteByte(stuff, stuff->destination_value + 2 * 0, (state->data_registers[stuff->opcode.secondary_register] >> 8 * 3) & 0xFF);
			WriteByte(stuff, stuff->destination_value + 2 * 1, (state->data_registers[stuff->opcode.secondary_register] >> 8 * 2) & 0xFF);
			WriteByte(stuff, stuff->destination_value + 2 * 2, (state->data_registers[stuff->opcode.secondary_register] >> 8 * 1) & 0xFF);
			WriteByte(stuff, stuff->destination_value + 2 * 3, (state->data_registers[stuff->opcode.secondary_register] >> 8 * 0) & 0xFF);
			break;
	}
}

static void Action_MOVEA(Stuff* const stuff)
{
	stuff->result_value = CC_SIGN_EXTEND_ULONG(stuff->msb_bit_index, stuff->source_value);
}

static void Action_MOVE(Stuff* const stuff)
{
	stuff->result_value = stuff->source_value;
}

static void Action_LINK(Stuff* const stuff)
{
	/* Push address register to stack */
	stuff->state->address_registers[7] -= 4;
	WriteLongWord(stuff, stuff->state->address_registers[7], stuff->state->address_registers[stuff->opcode.primary_register]);

	/* Copy stack pointer to address register */
	stuff->state->address_registers[stuff->opcode.primary_register] = stuff->state->address_registers[7];

	/* Offset the stack pointer by the immediate value */
	stuff->state->address_registers[7] += CC_SIGN_EXTEND_ULONG(15, stuff->source_value);
}

static void Action_UNLK(Stuff* const stuff)
{
	cc_u32l value;

	stuff->state->address_registers[7] = stuff->state->address_registers[stuff->opcode.primary_register];
	value = ReadLongWord(stuff, stuff->state->address_registers[7]);
	stuff->state->address_registers[7] += 4;

	/* We need to do this last in case we're writing to A7. */
	stuff->state->address_registers[stuff->opcode.primary_register] = value;
}

static void Action_NEGX(Stuff* const stuff)
{
	stuff->result_value = 0 - stuff->destination_value - ((stuff->state->status_register & CONDITION_CODE_EXTEND) != 0 ? 1 : 0);
}

static void Action_CLR(Stuff* const stuff)
{
	stuff->result_value = 0;
}

static void Action_NEG(Stuff* const stuff)
{
	stuff->result_value = 0 - stuff->destination_value;
}

static void Action_NOT(Stuff* const stuff)
{
	stuff->result_value = ~stuff->destination_value;
}

static void Action_EXT(Stuff* const stuff)
{
	stuff->result_value = CC_SIGN_EXTEND_ULONG((stuff->opcode.raw & 0x0040) != 0 ? 15 : 7, stuff->destination_value);
}

static void Action_SWAP(Stuff* const stuff)
{
	stuff->result_value = ((stuff->destination_value & 0x0000FFFF) << 16) | ((stuff->destination_value & 0xFFFF0000) >> 16);
}

static void Action_PEA(Stuff* const stuff)
{
	stuff->state->address_registers[7] -= 4;
	WriteLongWord(stuff, stuff->state->address_registers[7], stuff->source_value);
}

static void Action_ILLEGAL(Stuff* const stuff)
{
	/* Illegal instruction. */
	Group1Or2Exception(stuff, 4);
}

static void Action_TAS(Stuff* const stuff)
{
	/* TODO - This instruction doesn't work properly on memory on the Mega Drive */
	stuff->state->status_register &= ~(CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO);
	stuff->state->status_register |= CONDITION_CODE_NEGATIVE & (0 - ((stuff->destination_value & 0x80) != 0));
	stuff->state->status_register |= CONDITION_CODE_ZERO & (0 - (stuff->destination_value == 0));

	stuff->result_value = stuff->destination_value | 0x80;
}

static void Action_TRAP(Stuff* const stuff)
{
	stuff->source_value = stuff->opcode.raw & 0xF;
	Group1Or2Exception(stuff, 32 + stuff->source_value);
}

static void Action_MOVE_USP(Stuff* const stuff)
{
	if ((stuff->opcode.raw & 8) != 0)
		stuff->state->address_registers[stuff->opcode.primary_register] = stuff->state->user_stack_pointer;
	else
		stuff->state->user_stack_pointer = stuff->state->address_registers[stuff->opcode.primary_register];
}

#define UNIMPLEMENTED_INSTRUCTION(instruction) Clown68000_PrintError("Unimplemented instruction " instruction " used at 0x%" CC_PRIXLEAST32, stuff->state->program_counter)

static void Action_RESET(Stuff* const stuff)
{
	/* TODO */
	UNIMPLEMENTED_INSTRUCTION("RESET");
}

static void Action_STOP(Stuff* const stuff)
{
	/* TODO */
	stuff->state->status_register = stuff->source_value;
	stuff->state->stopped = cc_true;
}

static void Action_RTE(Stuff* const stuff)
{
	const cc_u16f new_status = ReadWord(stuff, stuff->state->address_registers[7]) & STATUS_REGISTER_MASK;

	stuff->state->status_register = new_status;
	stuff->state->address_registers[7] += 2;
	stuff->state->program_counter = ReadLongWord(stuff, stuff->state->address_registers[7]);
	stuff->state->address_registers[7] += 4;

	/* Restore the previous supervisor bit so we can toggle properly. */
	/* TODO: Maybe redesign SetSupervisorMode so that it isn't so clunky to use here. */
	stuff->state->status_register |= STATUS_SUPERVISOR;
	SetSupervisorMode(stuff->state, (new_status & STATUS_SUPERVISOR) != 0);
}

static void Action_RTS(Stuff* const stuff)
{
	stuff->state->program_counter = ReadLongWord(stuff, stuff->state->address_registers[7]);
	stuff->state->address_registers[7] += 4;
}

static void Action_TRAPV(Stuff* const stuff)
{
	if ((stuff->state->status_register & CONDITION_CODE_OVERFLOW) != 0)
		Group1Or2Exception(stuff, 7);
}

static void Action_RTR(Stuff* const stuff)
{
	stuff->state->status_register &= ~CONDITION_CODE_REGISTER_MASK;
	stuff->state->status_register |= ReadByte(stuff, stuff->state->address_registers[7] + 1) & CONDITION_CODE_REGISTER_MASK;
	stuff->state->address_registers[7] += 2;
	stuff->state->program_counter = ReadLongWord(stuff, stuff->state->address_registers[7]);
	stuff->state->address_registers[7] += 4;
}

static void Action_JSR(Stuff* const stuff)
{
	stuff->state->address_registers[7] -= 4;
	WriteLongWord(stuff, stuff->state->address_registers[7], stuff->state->program_counter);
	stuff->state->program_counter = stuff->source_value;
}

static void Action_JMP(Stuff* const stuff)
{
	stuff->state->program_counter = stuff->source_value;
}

static void Action_MOVEM(Stuff* const stuff)
{
	/* Hot damn is this a mess */
	cc_u32f memory_address = stuff->destination_value; /* TODO: Maybe get rid of this alias? */
	cc_u16f i;
	cc_u16f bitfield;

	int delta;
	void (*write_function)(Stuff *stuff, cc_u32f address, cc_u32f value);

	if ((stuff->opcode.raw & 0x0040) != 0)
	{
		delta = 4;
		write_function = WriteLongWord;
	}
	else
	{
		delta = 2;
		write_function = WriteWord;
	}

	if (stuff->opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)
		delta = -delta;

	bitfield = stuff->source_value;

	/* First group of registers */
	for (i = 0; i < 8; ++i)
	{
		if ((bitfield & 1) != 0)
		{
			if ((stuff->opcode.raw & 0x0400) != 0)
			{
				/* Memory to register */
				if ((stuff->opcode.raw & 0x0040) != 0)
					stuff->state->data_registers[i] = ReadLongWord(stuff, memory_address);
				else
					stuff->state->data_registers[i] = CC_SIGN_EXTEND_ULONG(15, ReadWord(stuff, memory_address));
			}
			else
			{
				/* Register to memory */
				if (stuff->opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)
					write_function(stuff, memory_address + delta, stuff->state->address_registers[7 - i]);
				else
					write_function(stuff, memory_address, stuff->state->data_registers[i]);
			}

			memory_address += delta;
		}

		bitfield >>= 1;
	}

	/* Second group of registers */
	for (i = 0; i < 8; ++i)
	{
		if ((bitfield & 1) != 0)
		{
			if ((stuff->opcode.raw & 0x0400) != 0)
			{
				/* Memory to register */
				if ((stuff->opcode.raw & 0x0040) != 0)
					stuff->state->address_registers[i] = ReadLongWord(stuff, memory_address);
				else
					stuff->state->address_registers[i] = CC_SIGN_EXTEND_ULONG(15, ReadWord(stuff, memory_address));
			}
			else
			{
				/* Register to memory */
				if (stuff->opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)
					write_function(stuff, memory_address + delta, stuff->state->data_registers[7 - i]);
				else
					write_function(stuff, memory_address, stuff->state->address_registers[i]);
			}

			memory_address += delta;
		}

		bitfield >>= 1;
	}

	if (stuff->opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT || stuff->opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT)
		stuff->state->address_registers[stuff->opcode.primary_register] = memory_address;
}

static void Action_CHK(Stuff* const stuff)
{
	const cc_u32f value = stuff->state->data_registers[stuff->opcode.secondary_register] & 0xFFFF;

	if ((value & 0x8000) != 0)
	{
		/* Value is smaller than 0. */
		stuff->state->status_register |= CONDITION_CODE_NEGATIVE;
		Group1Or2Exception(stuff, 6);
	}
	else
	{
		const cc_u32f delta = value - stuff->source_value;

		if ((delta & 0x8000) == 0 && delta != 0)
		{
			/* Value is greater than upper bound. */
			stuff->state->status_register &= ~CONDITION_CODE_NEGATIVE;
			Group1Or2Exception(stuff, 6);
		}
	}
}

static void Action_SCC(Stuff* const stuff)
{
	stuff->result_value = IsOpcodeConditionTrue(stuff->state, stuff->opcode.raw) ? 0xFF : 0;
}

static void Action_DBCC(Stuff* const stuff)
{
	if (!IsOpcodeConditionTrue(stuff->state, stuff->opcode.raw))
	{
		cc_u16f loop_counter = stuff->state->data_registers[stuff->opcode.primary_register] & 0xFFFF;

		if (loop_counter-- != 0)
		{
			stuff->state->program_counter -= 2;
			stuff->state->program_counter += CC_SIGN_EXTEND_ULONG(15, stuff->source_value);
		}

		stuff->state->data_registers[stuff->opcode.primary_register] &= ~0xFFFFul;
		stuff->state->data_registers[stuff->opcode.primary_register] |= loop_counter & 0xFFFF;
	}
}

static void Action_BRA_SHORT(Stuff* const stuff)
{
	stuff->state->program_counter += CC_SIGN_EXTEND_ULONG(7, stuff->opcode.raw);
}

static void Action_BRA_WORD(Stuff* const stuff)
{
	stuff->state->program_counter -= 2;
	stuff->state->program_counter += CC_SIGN_EXTEND_ULONG(15, stuff->source_value);
}

static void Action_BSR_SHORT(Stuff* const stuff)
{
	stuff->state->address_registers[7] -= 4;
	WriteLongWord(stuff, stuff->state->address_registers[7], stuff->state->program_counter);
	Action_BRA_SHORT(stuff);
}

static void Action_BSR_WORD(Stuff* const stuff)
{
	stuff->state->address_registers[7] -= 4;
	WriteLongWord(stuff, stuff->state->address_registers[7], stuff->state->program_counter);
	Action_BRA_WORD(stuff);
}

static void Action_BCC_SHORT(Stuff* const stuff)
{
	if (IsOpcodeConditionTrue(stuff->state, stuff->opcode.raw))
		Action_BRA_SHORT(stuff);
}

static void Action_BCC_WORD(Stuff* const stuff)
{
	if (IsOpcodeConditionTrue(stuff->state, stuff->opcode.raw))
		Action_BRA_WORD(stuff);
}

static void Action_MOVEQ(Stuff* const stuff)
{
	stuff->result_value = CC_SIGN_EXTEND_ULONG(7, stuff->opcode.raw);
}

static void Action_DIVCommon(Stuff* const stuff, const cc_bool is_signed)
{
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;

	if (stuff->source_value == 0)
	{
		/* TODO: These hacks are needed for the validator. Is the validator actually correct? */
		stuff->state->status_register &= ~(CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO | CONDITION_CODE_OVERFLOW);
		stuff->state->program_counter -= 4;

		Group1Or2Exception(stuff, 5);
		longjmp(stuff->exception.context, 1);
	}
	else
	{
		const cc_bool source_is_negative = is_signed && (stuff->source_value & 0x8000) != 0;
		const cc_bool destination_is_negative = is_signed && (stuff->destination_value & 0x80000000) != 0;
		const cc_bool result_is_negative = source_is_negative != destination_is_negative;

		const cc_u32f absolute_source_value = source_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(15, stuff->source_value) : stuff->source_value;
		const cc_u32f absolute_destination_value = destination_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(31, stuff->destination_value) : stuff->destination_value;

		const cc_u32f absolute_quotient = absolute_destination_value / absolute_source_value;
		const cc_u32f quotient = result_is_negative ? 0 - absolute_quotient : absolute_quotient;

		/* Overflow detection */
		if (absolute_quotient > (!is_signed ? 0xFFFFul : (result_is_negative ? 0x8000ul : 0x7FFFul)))
		{
			stuff->state->status_register |= CONDITION_CODE_OVERFLOW;

			stuff->result_value = stuff->destination_value;
		}
		else
		{
			const cc_u32f absolute_remainder = absolute_destination_value % absolute_source_value;
			const cc_u32f remainder = destination_is_negative ? 0 - absolute_remainder : absolute_remainder;

			stuff->result_value = (quotient & 0xFFFF) | ((remainder & 0xFFFF) << 16);

			stuff->state->status_register &= ~(CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO | CONDITION_CODE_OVERFLOW);
			stuff->state->status_register |= CONDITION_CODE_NEGATIVE & (0 - ((quotient & 0x8000) != 0));
			stuff->state->status_register |= CONDITION_CODE_ZERO & (0 - (quotient == 0));
		}
	}
}

static void Action_DIVS(Stuff* const stuff)
{
	Action_DIVCommon(stuff, cc_true);
}

static void Action_DIVU(Stuff* const stuff)
{
	Action_DIVCommon(stuff, cc_false);
}

static void Action_SUBX(Stuff* const stuff)
{
	stuff->result_value = stuff->destination_value - stuff->source_value - ((stuff->state->status_register & CONDITION_CODE_EXTEND) != 0 ? 1 : 0);
}

static void Action_SBCD(Stuff* const stuff)
{
	/* SBCD works in two steps: a standard SUBX, followed by a standard SUB between the result and a 'correction factor'. */
	Action_SUBX(stuff);

	/* The correction factor is determined by detecting both decimal and integer overflow of each
	   nibble of the result, and setting its corresponding nibble to 6 if overflow did occur. */
	/* Credit goes to Flamewing for this neat logic trick. */
	stuff->source_value = (((stuff->source_value & ~stuff->destination_value) | ((stuff->source_value | ~stuff->destination_value) & stuff->result_value)) & 0x88) << 1;
	stuff->source_value = (stuff->source_value >> 2) | (stuff->source_value >> 3);

	stuff->destination_value = stuff->result_value;

	Action_SUB(stuff);

	/* Manually set the carry flag here. */
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;
	stuff->state->status_register |= (stuff->source_value & 0x40) != 0 || (~stuff->destination_value & stuff->result_value & 0x80) != 0 ? CONDITION_CODE_CARRY : 0;
}

static void Action_NBCD(Stuff* const stuff)
{
	stuff->source_value = stuff->destination_value;
	stuff->destination_value = 0;
	Action_SBCD(stuff);
}

static void Action_MULCommon(Stuff* const stuff, const cc_bool is_signed)
{
	const cc_bool multiplier_is_negative = is_signed && (stuff->source_value & 0x8000) != 0;
	const cc_bool multiplicand_is_negative = is_signed && (stuff->destination_value & 0x8000) != 0;
	const cc_bool result_is_negative = multiplier_is_negative != multiplicand_is_negative;

	const cc_u32f multiplier = multiplier_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(15, stuff->source_value) : stuff->source_value;
	const cc_u32f multiplicand = multiplicand_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(15, stuff->destination_value) : stuff->destination_value & 0xFFFF;

	const cc_u32f absolute_result = multiplicand * multiplier;

	stuff->result_value = result_is_negative ? 0 - absolute_result : absolute_result;
}

static void Action_MULS(Stuff* const stuff)
{
	Action_MULCommon(stuff, cc_true);
}

static void Action_MULU(Stuff* const stuff)
{
	Action_MULCommon(stuff, cc_false);
}

static void Action_ADDX(Stuff* const stuff)
{
	stuff->result_value = stuff->destination_value + stuff->source_value + ((stuff->state->status_register & CONDITION_CODE_EXTEND) != 0 ? 1 : 0);
}

static void Action_ABCD(Stuff* const stuff)
{
	/* ABCD works in two steps: a standard ADDX, followed by a standard ADD between the result and a 'correction factor'. */
	Action_ADDX(stuff);

	/* The correction factor is determined by detecting both decimal and integer overflow of each
	   nibble of the result, and setting its corresponding nibble to 6 if overflow did occur. */
	/* Credit goes to Flamewing for this neat logic trick. */
	stuff->source_value = (((stuff->source_value & stuff->destination_value) | ((stuff->source_value | stuff->destination_value) & ~stuff->result_value)) & 0x88) << 1;
	stuff->source_value |= ((stuff->result_value + 0x66) ^ stuff->result_value) & 0x110;
	stuff->source_value = (stuff->source_value >> 2) | (stuff->source_value >> 3);

	stuff->destination_value = stuff->result_value;

	Action_ADD(stuff);

	/* Manually set the carry flag here. */
	stuff->state->status_register &= ~CONDITION_CODE_CARRY;
	stuff->state->status_register |= (stuff->source_value & 0x40) != 0 || (stuff->destination_value & ~stuff->result_value & 0x80) != 0 ? CONDITION_CODE_CARRY : 0;
}

static void Action_EXG(Stuff* const stuff)
{
	cc_u32f temp;

	switch (stuff->opcode.raw & 0x00F8)
	{
		/* TODO: What should happen when an invalid bit pattern occurs? */
		case 0x0040:
			temp = stuff->state->data_registers[stuff->opcode.secondary_register];
			stuff->state->data_registers[stuff->opcode.secondary_register] = stuff->state->data_registers[stuff->opcode.primary_register];
			stuff->state->data_registers[stuff->opcode.primary_register] = temp;
			break;

		case 0x0048:
			temp = stuff->state->address_registers[stuff->opcode.secondary_register];
			stuff->state->address_registers[stuff->opcode.secondary_register] = stuff->state->address_registers[stuff->opcode.primary_register];
			stuff->state->address_registers[stuff->opcode.primary_register] = temp;
			break;

		case 0x0088:
			temp = stuff->state->data_registers[stuff->opcode.secondary_register];
			stuff->state->data_registers[stuff->opcode.secondary_register] = stuff->state->address_registers[stuff->opcode.primary_register];
			stuff->state->address_registers[stuff->opcode.primary_register] = temp;
			break;
	}
}

#define DO_INSTRUCTION_ACTION_SHIFT_1_ASD \
	const unsigned long original_sign_bit = stuff->destination_value & sign_bit_bitmask;

#define DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD

#define DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY \
	count = 1;

#define DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER \
	count = (stuff->opcode.raw & 0x0020) != 0 ? stuff->state->data_registers[stuff->opcode.secondary_register] % 64 : ((stuff->opcode.secondary_register - 1u) & 7u) + 1u; /* A little math trick to turn 0 into 8 */

#define DO_INSTRUCTION_ACTION_SHIFT_3_ASD \
	stuff->result_value <<= 1; \
	stuff->state->status_register |= CONDITION_CODE_OVERFLOW & (0 - ((stuff->result_value & sign_bit_bitmask) != original_sign_bit));

#define DO_INSTRUCTION_ACTION_SHIFT_3_LSD \
	stuff->result_value <<= 1;

#define DO_INSTRUCTION_ACTION_SHIFT_3_ROXD \
	stuff->result_value <<= 1; \
	stuff->result_value |= (stuff->state->status_register & CONDITION_CODE_EXTEND) != 0;

#define DO_INSTRUCTION_ACTION_SHIFT_3_ROD \
	stuff->result_value = (stuff->result_value << 1) | ((stuff->result_value & sign_bit_bitmask) != 0);

#define DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD \
	stuff->state->status_register &= ~CONDITION_CODE_EXTEND; \
	stuff->state->status_register |= CONDITION_CODE_EXTEND & (0 - ((stuff->state->status_register & CONDITION_CODE_CARRY) != 0));

#define DO_INSTRUCTION_ACTION_SHIFT_4_ROD

#define DO_INSTRUCTION_ACTION_SHIFT_5_ASD \
	stuff->result_value >>= 1; \
	stuff->result_value |= original_sign_bit;

#define DO_INSTRUCTION_ACTION_SHIFT_5_LSD \
	stuff->result_value >>= 1;

#define DO_INSTRUCTION_ACTION_SHIFT_5_ROXD \
	stuff->result_value >>= 1; \
	stuff->result_value |= sign_bit_bitmask & (0 - ((stuff->state->status_register & CONDITION_CODE_EXTEND) != 0));

#define DO_INSTRUCTION_ACTION_SHIFT_5_ROD \
	stuff->result_value = (stuff->result_value >> 1) | (sign_bit_bitmask & (0 - ((stuff->result_value & 1) != 0)));

#define DO_INSTRUCTION_ACTION_SHIFT_6_ROXD \
	stuff->state->status_register |= CONDITION_CODE_CARRY & (0 - ((stuff->state->status_register & CONDITION_CODE_EXTEND) != 0));

#define DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD

#define DO_INSTRUCTION_ACTION_SHIFT(SUB_ACTION_1, SUB_ACTION_2, SUB_ACTION_3, SUB_ACTION_4, SUB_ACTION_5, SUB_ACTION_6) \
	const cc_u32f sign_bit_bitmask = (cc_u32f)1 << stuff->msb_bit_index; \
 \
	SUB_ACTION_1 \
 \
	cc_u16f i; \
	cc_u16f count; \
 \
	stuff->result_value = stuff->destination_value; \
 \
	SUB_ACTION_2; \
 \
	stuff->state->status_register &= ~(CONDITION_CODE_OVERFLOW | CONDITION_CODE_CARRY); \
 \
	SUB_ACTION_6; \
 \
	if (stuff->opcode.bit_8) \
	{ \
		/* Left */ \
		for (i = 0; i < count; ++i) \
		{ \
			stuff->state->status_register &= ~CONDITION_CODE_CARRY; \
			stuff->state->status_register |= CONDITION_CODE_CARRY & (0 - ((stuff->result_value & sign_bit_bitmask) != 0)); \
 \
			SUB_ACTION_3; \
 \
			SUB_ACTION_4; \
		} \
	} \
	else \
	{ \
		/* Right */ \
		for (i = 0; i < count; ++i) \
		{ \
			stuff->state->status_register &= ~CONDITION_CODE_CARRY; \
			stuff->state->status_register |= CONDITION_CODE_CARRY & (0 - ((stuff->result_value & 1) != 0)); \
 \
			SUB_ACTION_5; \
 \
			SUB_ACTION_4; \
		} \
	}

static void Action_ASD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_ASD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ASD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)
}

static void Action_ASD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_ASD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ASD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)
}

static void Action_LSD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_LSD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_LSD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)
}

static void Action_LSD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_LSD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_LSD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)
}

static void Action_ROD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_ROD, DO_INSTRUCTION_ACTION_SHIFT_4_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)
}

static void Action_ROD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_ROD, DO_INSTRUCTION_ACTION_SHIFT_4_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)
}

static void Action_ROXD_MEMORY(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_ROXD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROXD, DO_INSTRUCTION_ACTION_SHIFT_6_ROXD)
}

static void Action_ROXD_REGISTER(Stuff* const stuff)
{
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_ROXD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROXD, DO_INSTRUCTION_ACTION_SHIFT_6_ROXD)
}

static void Action_UNIMPLEMENTED_1(Stuff* const stuff)
{
	Group1Or2Exception(stuff, 10);
}

static void Action_UNIMPLEMENTED_2(Stuff* const stuff)
{
	Group1Or2Exception(stuff, 11);
}

static void Action_NOP(Stuff* const stuff)
{
	(void)stuff;

	/* Doesn't do anything */
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
		state->stopped = cc_false;

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
		state->stopped = cc_false;
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
	if (state->halted || state->stopped)
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
			/* Figure out which instruction this is. */
			const Instruction instruction = DecodeOpcode(&stuff.opcode, ReadWord(&stuff, state->program_counter));

			/* We already pre-fetched the instruction, so just advance past it. */
			state->instruction_register = stuff.opcode.raw;
			state->program_counter += 2;

			switch (instruction)
			{
				#include "microcode.c"
			}
		}
	}
}
