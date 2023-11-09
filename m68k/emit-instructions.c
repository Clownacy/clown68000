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

#include "emit-instructions.h"

#include <stdio.h>

#include "emit.h"
#include "instruction-properties.h"

#define EMIT
#include "instruction-actions.h"

void EmitInstructionSupervisorCheck(const Instruction instruction)
{
	if (Instruction_IsPrivileged(instruction))
	{
		Emit("/* Only allow this instruction in supervisor mode. */");
		Emit("if ((state->status_register & STATUS_SUPERVISOR) == 0)");
		Emit("{");
		Emit("	Group1Or2Exception(&stuff, 8);");
		Emit("	longjmp(stuff.exception.context, 1);");
		Emit("}");
		Emit("");
	}
}

static EmitOperandSource(const char* const operation_size, const char* const address_mode, const char* const address_mode_register)
{
	EmitFormatted("DecodeAddressMode(&stuff, &source_decoded_address_mode, %s, %s, %s);", operation_size, address_mode, address_mode_register);
}

static EmitOperandDestination(const char* const operation_size, const char* const address_mode, const char* const address_mode_register)
{
	EmitFormatted("DecodeAddressMode(&stuff, &destination_decoded_address_mode, %s, %s, %s);", operation_size, address_mode, address_mode_register);
}

void EmitInstructionSourceAddressMode(const Instruction instruction)
{
	if (Instruction_IsSourceOperandRead(instruction))
	{
		Emit("/* Decode source address mode. */");

		switch (Instruction_GetSourceOperand(instruction))
		{
			case INSTRUCTION_SOURCE_IMMEDIATE_DATA:
				/* Immediate value (any size). */
				EmitOperandSource("decoded_opcode.size", "ADDRESS_MODE_SPECIAL", "ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE");
				break;

			case INSTRUCTION_SOURCE_DATA_REGISTER_SECONDARY:
				/* Secondary data register. */
				EmitOperandSource("decoded_opcode.size", "ADDRESS_MODE_DATA_REGISTER", "opcode.secondary_register");
				break;

			case INSTRUCTION_SOURCE_IMMEDIATE_DATA_BYTE:
				/* Immediate value (byte). */
				EmitOperandSource("1", "ADDRESS_MODE_SPECIAL", "ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE");
				break;

			case INSTRUCTION_SOURCE_MEMORY_ADDRESS_PRIMARY:
				/* Memory address */
				EmitOperandSource("0", "opcode.primary_address_mode", "opcode.primary_register"); /* 0 is a special value that means to obtain the address rather than the data at that address. */
				break;

			case INSTRUCTION_SOURCE_STATUS_REGISTER:
				EmitOperandSource("0", "ADDRESS_MODE_STATUS_REGISTER", "0");
				break;

			case INSTRUCTION_SOURCE_IMMEDIATE_DATA_WORD:
				/* Immediate value (word). */
				EmitOperandSource("2", "ADDRESS_MODE_SPECIAL", "ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE");
				break;

			case INSTRUCTION_SOURCE_BCD_X:
				EmitOperandSource("decoded_opcode.size", "(opcode.raw & 0x0008) != 0 ? ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT : ADDRESS_MODE_DATA_REGISTER", "opcode.primary_register");
				break;

			case INSTRUCTION_SOURCE_DATA_REGISTER_SECONDARY_OR_PRIMARY_ADDRESS_MODE:
				/* Primary address mode or secondary data register, based on direction bit. */
				EmitOperandSource("decoded_opcode.size", "opcode.bit_8 ? ADDRESS_MODE_DATA_REGISTER : opcode.primary_address_mode", "opcode.bit_8 ? opcode.secondary_register : opcode.primary_register");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE_SIZED:
				/* Word or longword based on bit 8. */
				EmitOperandSource("opcode.bit_8 ? 4 : 2", "opcode.primary_address_mode", "opcode.primary_register");
				break;

			case INSTRUCTION_SOURCE_ADDRESS_REGISTER_PRIMARY_POSTINCREMENT:
				EmitOperandSource("decoded_opcode.size", "ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT", "opcode.primary_register");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE:
				/* Primary address mode. */
				EmitOperandSource("decoded_opcode.size", "opcode.primary_address_mode", "opcode.primary_register");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE_WORD:
				/* Primary address mode, hardcoded to word-size. */
				EmitOperandSource("2", "opcode.primary_address_mode", "opcode.primary_register");
				break;

			case INSTRUCTION_SOURCE_NONE:
				/* Doesn't have a source address mode to decode. */
				EmitOperandSource("0", "ADDRESS_MODE_NONE", "0");
				break;
		}

		Emit("");
	}
}

void EmitInstructionDestinationAddressMode(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandRead(instruction) || Instruction_IsDestinationOperandWritten(instruction))
	{
		Emit("/* Decode destination address mode. */");

		switch (Instruction_GetDestinationOperand(instruction))
		{
			case INSTRUCTION_DESTINATION_DATA_REGISTER_PRIMARY:
				/* Data register (primary) */
				EmitOperandDestination("decoded_opcode.size", "ADDRESS_MODE_DATA_REGISTER", "opcode.primary_register");
				break;

			case INSTRUCTION_DESTINATION_DATA_REGISTER_SECONDARY:
				/* Data register (secondary) */
				EmitOperandDestination("decoded_opcode.size", "ADDRESS_MODE_DATA_REGISTER", "opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY:
				/* Address register (secondary) */
				EmitOperandDestination("decoded_opcode.size", "ADDRESS_MODE_ADDRESS_REGISTER", "opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_SECONDARY_ADDRESS_MODE:
				/* Secondary address mode */
				EmitOperandDestination("decoded_opcode.size", "opcode.secondary_address_mode", "opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_BCD_X:
				EmitOperandDestination("decoded_opcode.size", "(opcode.raw & 0x0008) != 0 ? ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT : ADDRESS_MODE_DATA_REGISTER", "opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_DATA_REGISTER_SECONDARY_OR_PRIMARY_ADDRESS_MODE:
				/* Primary address mode or secondary data register, based on direction bit */
				EmitOperandDestination("decoded_opcode.size", "opcode.bit_8 ? opcode.primary_address_mode : ADDRESS_MODE_DATA_REGISTER", "opcode.bit_8 ? opcode.primary_register : opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY_FULL:
				/* Full secondary address register */
				EmitOperandDestination("4", "ADDRESS_MODE_ADDRESS_REGISTER", "opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY_POSTINCREMENT:
				EmitOperandDestination("decoded_opcode.size", "ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT", "opcode.secondary_register");
				break;

			case INSTRUCTION_DESTINATION_PRIMARY_ADDRESS_MODE:
				/* Using primary address mode */
				EmitOperandDestination("decoded_opcode.size", "opcode.primary_address_mode", "opcode.primary_register");
				break;

			case INSTRUCTION_DESTINATION_CONDITION_CODE_REGISTER:
				EmitOperandDestination("0", "ADDRESS_MODE_CONDITION_CODE_REGISTER", "0");
				break;

			case INSTRUCTION_DESTINATION_STATUS_REGISTER:
				EmitOperandDestination("0", "ADDRESS_MODE_STATUS_REGISTER", "0");
				break;

			case INSTRUCTION_DESTINATION_MOVEM:
				/* Memory address */
				EmitOperandDestination("0", "opcode.primary_address_mode", "opcode.primary_register"); /* 0 is a special value that means to obtain the address rather than the data at that address. */
				break;

			case INSTRUCTION_DESTINATION_MOVEP:
				/* Memory address */
				EmitOperandDestination("0", "ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT", "opcode.primary_register"); /* 0 is a special value that means to obtain the address rather than the data at that address. */
				break;

			case INSTRUCTION_DESTINATION_NONE:
				/* Doesn't have a destination address mode to decode. */
				EmitOperandDestination("0", "ADDRESS_MODE_NONE", "0");
				break;
		}

		Emit("");
	}
}

void EmitInstructionReadSourceOperand(const Instruction instruction)
{
	if (Instruction_IsSourceOperandRead(instruction))
	{
		Emit("/* Read source operand. */");
		Emit("source_value = GetValueUsingDecodedAddressMode(&stuff, &source_decoded_address_mode);");
		Emit("");
	}
}

void EmitInstructionReadDestinationOperand(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandRead(instruction))
	{
		Emit("/* Read destination operand. */");
		Emit("destination_value = GetValueUsingDecodedAddressMode(&stuff, &destination_decoded_address_mode);");
		Emit("");
	}
}

void EmitInstructionAction(const Instruction instruction)
{
	Emit("/* Do the actual instruction. */");

	switch (Instruction_GetAction(instruction))
	{
		case INSTRUCTION_ACTION_OR:
			DO_INSTRUCTION_ACTION_OR;
			break;

		case INSTRUCTION_ACTION_AND:
			DO_INSTRUCTION_ACTION_AND;
			break;

		case INSTRUCTION_ACTION_SUBA:
			DO_INSTRUCTION_ACTION_SUBA;
			break;

		case INSTRUCTION_ACTION_SUBQ:
			DO_INSTRUCTION_ACTION_SUBQ;
			break;

		case INSTRUCTION_ACTION_SUB:
			DO_INSTRUCTION_ACTION_SUB;
			break;

		case INSTRUCTION_ACTION_ADDA:
			DO_INSTRUCTION_ACTION_ADDA;
			break;

		case INSTRUCTION_ACTION_ADDQ:
			DO_INSTRUCTION_ACTION_ADDQ;
			break;

		case INSTRUCTION_ACTION_ADD:
			DO_INSTRUCTION_ACTION_ADD;
			break;

		case INSTRUCTION_ACTION_EOR:
			DO_INSTRUCTION_ACTION_EOR;
			break;

		case INSTRUCTION_ACTION_BCHG:
			DO_INSTRUCTION_ACTION_BCHG;
			break;

		case INSTRUCTION_ACTION_BCLR:
			DO_INSTRUCTION_ACTION_BCLR;
			break;

		case INSTRUCTION_ACTION_BSET:
			DO_INSTRUCTION_ACTION_BSET;
			break;

		case INSTRUCTION_ACTION_BTST:
			DO_INSTRUCTION_ACTION_BTST;
			break;

		case INSTRUCTION_ACTION_MOVEP:
			DO_INSTRUCTION_ACTION_MOVEP;
			break;

		case INSTRUCTION_ACTION_MOVEA:
			DO_INSTRUCTION_ACTION_MOVEA;
			break;

		case INSTRUCTION_ACTION_MOVE:
			DO_INSTRUCTION_ACTION_MOVE;
			break;

		case INSTRUCTION_ACTION_LINK:
			DO_INSTRUCTION_ACTION_LINK;
			break;

		case INSTRUCTION_ACTION_UNLK:
			DO_INSTRUCTION_ACTION_UNLK;
			break;

		case INSTRUCTION_ACTION_NEGX:
			DO_INSTRUCTION_ACTION_NEGX;
			break;

		case INSTRUCTION_ACTION_CLR:
			DO_INSTRUCTION_ACTION_CLR;
			break;

		case INSTRUCTION_ACTION_NEG:
			DO_INSTRUCTION_ACTION_NEG;
			break;

		case INSTRUCTION_ACTION_NOT:
			DO_INSTRUCTION_ACTION_NOT;
			break;

		case INSTRUCTION_ACTION_EXT:
			DO_INSTRUCTION_ACTION_EXT;
			break;

		case INSTRUCTION_ACTION_NBCD:
			DO_INSTRUCTION_ACTION_NBCD;
			break;

		case INSTRUCTION_ACTION_SWAP:
			DO_INSTRUCTION_ACTION_SWAP;
			break;

		case INSTRUCTION_ACTION_PEA:
			DO_INSTRUCTION_ACTION_PEA;
			break;

		case INSTRUCTION_ACTION_ILLEGAL:
			DO_INSTRUCTION_ACTION_ILLEGAL;
			break;

		case INSTRUCTION_ACTION_TAS:
			DO_INSTRUCTION_ACTION_TAS;
			break;

		case INSTRUCTION_ACTION_TRAP:
			DO_INSTRUCTION_ACTION_TRAP;
			break;

		case INSTRUCTION_ACTION_MOVE_USP:
			DO_INSTRUCTION_ACTION_MOVE_USP;
			break;

		case INSTRUCTION_ACTION_RESET:
			DO_INSTRUCTION_ACTION_RESET;
			break;

		case INSTRUCTION_ACTION_STOP:
			DO_INSTRUCTION_ACTION_STOP;
			break;

		case INSTRUCTION_ACTION_RTE:
			DO_INSTRUCTION_ACTION_RTE;
			break;

		case INSTRUCTION_ACTION_RTS:
			DO_INSTRUCTION_ACTION_RTS;
			break;

		case INSTRUCTION_ACTION_TRAPV:
			DO_INSTRUCTION_ACTION_TRAPV;
			break;

		case INSTRUCTION_ACTION_RTR:
			DO_INSTRUCTION_ACTION_RTR;
			break;

		case INSTRUCTION_ACTION_JSR:
			DO_INSTRUCTION_ACTION_JSR;
			break;

		case INSTRUCTION_ACTION_JMP:
			DO_INSTRUCTION_ACTION_JMP;
			break;

		case INSTRUCTION_ACTION_MOVEM:
			DO_INSTRUCTION_ACTION_MOVEM;
			break;

		case INSTRUCTION_ACTION_CHK:
			DO_INSTRUCTION_ACTION_CHK;
			break;

		case INSTRUCTION_ACTION_SCC:
			DO_INSTRUCTION_ACTION_SCC;
			break;

		case INSTRUCTION_ACTION_DBCC:
			DO_INSTRUCTION_ACTION_DBCC;
			break;

		case INSTRUCTION_ACTION_BRA_SHORT:
			DO_INSTRUCTION_ACTION_BRA_SHORT;
			break;

		case INSTRUCTION_ACTION_BRA_WORD:
			DO_INSTRUCTION_ACTION_BRA_WORD;
			break;

		case INSTRUCTION_ACTION_BSR_SHORT:
			DO_INSTRUCTION_ACTION_BSR_SHORT;
			break;

		case INSTRUCTION_ACTION_BSR_WORD:
			DO_INSTRUCTION_ACTION_BSR_WORD;
			break;

		case INSTRUCTION_ACTION_BCC_SHORT:
			DO_INSTRUCTION_ACTION_BCC_SHORT;
			break;

		case INSTRUCTION_ACTION_BCC_WORD:
			DO_INSTRUCTION_ACTION_BCC_WORD;
			break;

		case INSTRUCTION_ACTION_MOVEQ:
			DO_INSTRUCTION_ACTION_MOVEQ;
			break;

		case INSTRUCTION_ACTION_DIV:
			DO_INSTRUCTION_ACTION_DIV;
			break;

		case INSTRUCTION_ACTION_SBCD:
			DO_INSTRUCTION_ACTION_SBCD;
			break;

		case INSTRUCTION_ACTION_SUBX:
			DO_INSTRUCTION_ACTION_SUBX;
			break;

		case INSTRUCTION_ACTION_MUL:
			DO_INSTRUCTION_ACTION_MUL;
			break;

		case INSTRUCTION_ACTION_ABCD:
			DO_INSTRUCTION_ACTION_ABCD;
			break;

		case INSTRUCTION_ACTION_EXG:
			DO_INSTRUCTION_ACTION_EXG;
			break;

		case INSTRUCTION_ACTION_ADDX:
			DO_INSTRUCTION_ACTION_ADDX;
			break;

		case INSTRUCTION_ACTION_ASD_MEMORY:
			DO_INSTRUCTION_ACTION_ASD_MEMORY;
			break;

		case INSTRUCTION_ACTION_ASD_REGISTER:
			DO_INSTRUCTION_ACTION_ASD_REGISTER;
			break;

		case INSTRUCTION_ACTION_LSD_MEMORY:
			DO_INSTRUCTION_ACTION_LSD_MEMORY;
			break;

		case INSTRUCTION_ACTION_LSD_REGISTER:
			DO_INSTRUCTION_ACTION_LSD_REGISTER;
			break;

		case INSTRUCTION_ACTION_ROD_MEMORY:
			DO_INSTRUCTION_ACTION_ROD_MEMORY;
			break;

		case INSTRUCTION_ACTION_ROD_REGISTER:
			DO_INSTRUCTION_ACTION_ROD_REGISTER;
			break;

		case INSTRUCTION_ACTION_ROXD_MEMORY:
			DO_INSTRUCTION_ACTION_ROXD_MEMORY;
			break;

		case INSTRUCTION_ACTION_ROXD_REGISTER:
			DO_INSTRUCTION_ACTION_ROXD_REGISTER;
			break;

		case INSTRUCTION_ACTION_UNIMPLEMENTED_1:
			DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1;
			break;

		case INSTRUCTION_ACTION_UNIMPLEMENTED_2:
			DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2;
			break;

		case INSTRUCTION_ACTION_NOP:
			DO_INSTRUCTION_ACTION_NOP;
			break;
	}

	Emit("");
}

void EmitInstructionWriteDestinationOperand(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandWritten(instruction))
	{
		Emit("/* Write destination operand. */");
		Emit("SetValueUsingDecodedAddressMode(&stuff, &destination_decoded_address_mode, result_value);");
		Emit("");
	}
}

void EmitInstructionConditionCodes(const Instruction instruction)
{
	Emit("/* Update the condition codes in the following order: */");
	Emit("/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */");
	Emit("");

	Emit("/* Update CARRY condition code */");
	switch (Instruction_GetCarryModifier(instruction))
	{
		case INSTRUCTION_CARRY_STANDARD_CARRY:
			Emit("state->status_register &= ~CONDITION_CODE_CARRY;");
			Emit("state->status_register |= (((source_value & destination_value) | ((source_value | destination_value) & ~result_value)) >> (msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;");
			break;

		case INSTRUCTION_CARRY_STANDARD_BORROW:
			Emit("state->status_register &= ~CONDITION_CODE_CARRY;");
			Emit("state->status_register |= (((source_value & ~destination_value) | ((source_value | ~destination_value) & result_value)) >> (msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;");
			break;

		case INSTRUCTION_CARRY_NEG:
			Emit("state->status_register &= ~CONDITION_CODE_CARRY;");
			Emit("state->status_register |= ((destination_value | result_value) >> (msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;");
			break;

		case INSTRUCTION_CARRY_CLEAR:
			Emit("state->status_register &= ~CONDITION_CODE_CARRY;");
			break;

		case INSTRUCTION_CARRY_UNDEFINED:
			Emit("/* Undefined */");
			break;

		case INSTRUCTION_CARRY_UNAFFECTED:
			Emit("/* Unaffected */");
			break;
	}

	Emit("/* Update OVERFLOW condition code */");
	switch (Instruction_GetOverflowModifier(instruction))
	{
		case INSTRUCTION_OVERFLOW_ADD:
			Emit("state->status_register &= ~CONDITION_CODE_OVERFLOW;");
			Emit("state->status_register |= (((source_value & destination_value & ~result_value) | (~source_value & ~destination_value & result_value)) >> (msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;");
			break;

		case INSTRUCTION_OVERFLOW_SUB:
			Emit("state->status_register &= ~CONDITION_CODE_OVERFLOW;");
			Emit("state->status_register |= (((~source_value & destination_value & ~result_value) | (source_value & ~destination_value & result_value)) >> (msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;");
			break;

		case INSTRUCTION_OVERFLOW_NEG:
			Emit("state->status_register &= ~CONDITION_CODE_OVERFLOW;");
			Emit("state->status_register |= ((destination_value & result_value) >> (msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;");
			break;

		case INSTRUCTION_OVERFLOW_CLEARED:
			Emit("state->status_register &= ~CONDITION_CODE_OVERFLOW;");
			break;

		case INSTRUCTION_OVERFLOW_UNDEFINED:
			Emit("/* Undefined */");
			break;

		case INSTRUCTION_OVERFLOW_UNAFFECTED:
			Emit("/* Unaffected */");
			break;
	}

	Emit("/* Update ZERO condition code */");
	switch (Instruction_GetZeroModifier(instruction))
	{
		case INSTRUCTION_ZERO_CLEAR_IF_NONZERO_UNAFFECTED_OTHERWISE:
			Emit("/* Cleared if the result is nonzero; unchanged otherwise */");
			Emit("state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((result_value & (0xFFFFFFFF >> (32 - msb_bit_index - 1))) == 0));");
			break;

		case INSTRUCTION_ZERO_SET_IF_ZERO_CLEAR_OTHERWISE:
			Emit("/* Standard behaviour: set if result is zero; clear otherwise */");
			Emit("state->status_register &= ~CONDITION_CODE_ZERO;");
			Emit("state->status_register |= CONDITION_CODE_ZERO & (0 - ((result_value & (0xFFFFFFFF >> (32 - msb_bit_index - 1))) == 0));");
			break;

		case INSTRUCTION_ZERO_UNDEFINED:
			Emit("/* Undefined */");
			break;

		case INSTRUCTION_ZERO_UNAFFECTED:
			Emit("/* Unaffected */");
			break;
	}

	Emit("/* Update NEGATIVE condition code */");
	switch (Instruction_GetNegativeModifier(instruction))
	{
		case INSTRUCTION_NEGATIVE_SET_IF_NEGATIVE_CLEAR_OTHERWISE:
			Emit("/* Standard behaviour: set if result value is negative; clear otherwise */");
			Emit("state->status_register &= ~CONDITION_CODE_NEGATIVE;");
			Emit("state->status_register |= CONDITION_CODE_NEGATIVE & (result_value >> (msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;");
			break;

		case INSTRUCTION_NEGATIVE_UNAFFECTED:
			Emit("/* Unaffected */");
			break;
	}

	Emit("/* Update EXTEND condition code */");
	switch (Instruction_GetExtendModifier(instruction))
	{
		case INSTRUCTION_EXTEND_SET_TO_CARRY:
			Emit("/* Standard behaviour: set to CARRY */");
			Emit("state->status_register &= ~CONDITION_CODE_EXTEND;");
			Emit("state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));");
			break;

		case INSTRUCTION_EXTEND_UNAFFECTED:
			Emit("/* Unaffected */");
			break;
	}

	Emit("");
}
