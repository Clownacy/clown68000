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
		Emit("SupervisorCheck(&stuff);");
		Emit("");
	}
}

void EmitInstructionSize(const Instruction instruction)
{
	const InstructionSize size = Instruction_GetSize(instruction);

	if (size != INSTRUCTION_SIZE_NONE)
	{
		switch (Instruction_GetSize(instruction))
		{
			case INSTRUCTION_SIZE_BYTE:
				/* Hardcoded to a byte. */
				Emit("SetSize_Byte(&stuff);");
				break;

			case INSTRUCTION_SIZE_WORD:
				/* Hardcoded to a word. */
				Emit("SetSize_Word(&stuff);");
				break;

			case INSTRUCTION_SIZE_LONGWORD:
				/* Hardcoded to a longword. */
				Emit("SetSize_Longword(&stuff);");
				break;

			case INSTRUCTION_SIZE_LONGWORD_REGISTER_BYTE_MEMORY:
				/* 4 if register - 1 if memory. */
				Emit("SetSize_LongwordRegisterByteMemory(&stuff);");
				break;

			case INSTRUCTION_SIZE_MOVE:
				/* Derived from an odd bitfield. */
				Emit("SetSize_Move(&stuff);");
				break;

			case INSTRUCTION_SIZE_EXT:
				Emit("SetSize_Ext(&stuff);");
				break;

			case INSTRUCTION_SIZE_STANDARD:
				/* Standard. */
				Emit("SetSize_Standard(&stuff);");
				break;

			case INSTRUCTION_SIZE_NONE:
				/* Doesn't have a size. */
				break;
		}

		Emit("SetMSBBitIndex(&stuff);");
		Emit("");
	}
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
				Emit("DecodeSource_ImmediateData(&stuff);");
				break;

			case INSTRUCTION_SOURCE_DATA_REGISTER_SECONDARY:
				/* Secondary data register. */
				Emit("DecodeSource_DataRegisterSecondary(&stuff);");
				break;

			case INSTRUCTION_SOURCE_IMMEDIATE_DATA_BYTE:
				/* Immediate value (byte). */
				Emit("DecodeSource_ImmediateDataByte(&stuff);");
				break;

			case INSTRUCTION_SOURCE_MEMORY_ADDRESS_PRIMARY:
				/* Memory address */
				Emit("DecodeSource_MemoryAddressPrimary(&stuff);");
				break;

			case INSTRUCTION_SOURCE_STATUS_REGISTER:
				Emit("DecodeSource_StatusRegister(&stuff);");
				break;

			case INSTRUCTION_SOURCE_IMMEDIATE_DATA_WORD:
				/* Immediate value (word). */
				Emit("DecodeSource_ImmediateDataWord(&stuff);");
				break;

			case INSTRUCTION_SOURCE_BCD_X:
				Emit("DecodeSource_BCDX(&stuff);");
				break;

			case INSTRUCTION_SOURCE_DATA_REGISTER_SECONDARY_OR_PRIMARY_ADDRESS_MODE:
				/* Primary address mode or secondary data register, based on direction bit. */
				Emit("DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE_SIZED:
				/* Word or longword based on bit 8. */
				Emit("DecodeSource_PrimaryAddressModeSized(&stuff);");
				break;

			case INSTRUCTION_SOURCE_ADDRESS_REGISTER_PRIMARY_POSTINCREMENT:
				Emit("DecodeSource_AddressRegisterPrimaryPostIncrement(&stuff);");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE:
				/* Primary address mode. */
				Emit("DecodeSource_PrimaryAddressMode(&stuff);");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE_WORD:
				/* Primary address mode, hardcoded to word-size. */
				Emit("DecodeSource_PrimaryAddressModeWord(&stuff);");
				break;

			case INSTRUCTION_SOURCE_NONE:
				/* Doesn't have a source address mode to decode. */
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
				Emit("DecodeDestination_DataRegisterPrimary(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_DATA_REGISTER_SECONDARY:
				/* Data register (secondary) */
				Emit("DecodeDestination_DataRegisterSecondary(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY:
				/* Address register (secondary) */
				Emit("DecodeDestination_AddressRegisterSecondary(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_SECONDARY_ADDRESS_MODE:
				/* Secondary address mode */
				Emit("DecodeDestination_SecondaryAddressMode(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_BCD_X:
				Emit("DecodeDestination_BCDX(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_DATA_REGISTER_SECONDARY_OR_PRIMARY_ADDRESS_MODE:
				/* Primary address mode or secondary data register, based on direction bit */
				Emit("DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY_FULL:
				/* Full secondary address register */
				Emit("DecodeDestination_AddressRegisterSecondaryFull(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY_POSTINCREMENT:
				Emit("DecodeDestination_AddressRegisterSecondaryPostIncrement(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_PRIMARY_ADDRESS_MODE:
				/* Using primary address mode */
				Emit("DecodeDestination_PrimaryAddressMode(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_CONDITION_CODE_REGISTER:
				Emit("DecodeDestination_ConditionCodeRegister(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_STATUS_REGISTER:
				Emit("DecodeDestination_StatusRegister(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_MOVEM:
				/* Memory address */
				Emit("DecodeDestination_MOVEM(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_MOVEP:
				/* Memory address */
				Emit("DecodeDestination_MOVEP(&stuff);");
				break;

			case INSTRUCTION_DESTINATION_NONE:
				/* Doesn't have a destination address mode to decode. */
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
		Emit("ReadSource(&stuff);");
		Emit("");
	}
}

void EmitInstructionReadDestinationOperand(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandRead(instruction))
	{
		Emit("/* Read destination operand. */");
		Emit("ReadDestination(&stuff);");
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
		Emit("WriteDestination(&stuff);");
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
			Emit("state->status_register |= (((stuff.source_value & stuff.destination_value) | ((stuff.source_value | stuff.destination_value) & ~stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;");
			break;

		case INSTRUCTION_CARRY_STANDARD_BORROW:
			Emit("state->status_register &= ~CONDITION_CODE_CARRY;");
			Emit("state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;");
			break;

		case INSTRUCTION_CARRY_NEG:
			Emit("state->status_register &= ~CONDITION_CODE_CARRY;");
			Emit("state->status_register |= ((stuff.destination_value | stuff.result_value) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;");
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
			Emit("state->status_register |= (((stuff.source_value & stuff.destination_value & ~stuff.result_value) | (~stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;");
			break;

		case INSTRUCTION_OVERFLOW_SUB:
			Emit("state->status_register &= ~CONDITION_CODE_OVERFLOW;");
			Emit("state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;");
			break;

		case INSTRUCTION_OVERFLOW_NEG:
			Emit("state->status_register &= ~CONDITION_CODE_OVERFLOW;");
			Emit("state->status_register |= ((stuff.destination_value & stuff.result_value) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;");
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
			Emit("state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));");
			break;

		case INSTRUCTION_ZERO_SET_IF_ZERO_CLEAR_OTHERWISE:
			Emit("/* Standard behaviour: set if result is zero; clear otherwise */");
			Emit("state->status_register &= ~CONDITION_CODE_ZERO;");
			Emit("state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));");
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
			Emit("state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;");
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
