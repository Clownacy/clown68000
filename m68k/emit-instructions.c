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

void EmitInstructionSupervisorCheck(const Instruction instruction)
{
	if (Instruction_IsPrivileged(instruction))
	{
		/* Only allow this instruction in supervisor mode. */
		Emit("SupervisorCheck,\n\t");
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
				Emit("SetSize_Byte");
				break;

			case INSTRUCTION_SIZE_WORD:
				/* Hardcoded to a word. */
				Emit("SetSize_Word");
				break;

			case INSTRUCTION_SIZE_LONGWORD:
				/* Hardcoded to a longword. */
				Emit("SetSize_Longword");
				break;

			case INSTRUCTION_SIZE_LONGWORD_REGISTER_BYTE_MEMORY:
				/* 4 if register - 1 if memory. */
				Emit("SetSize_LongwordRegisterByteMemory");
				break;

			case INSTRUCTION_SIZE_MOVE:
				/* Derived from an odd bitfield. */
				Emit("SetSize_Move");
				break;

			case INSTRUCTION_SIZE_EXT:
				Emit("SetSize_Ext");
				break;

			case INSTRUCTION_SIZE_STANDARD:
				/* Standard. */
				Emit("SetSize_Standard");
				break;

			case INSTRUCTION_SIZE_NONE:
				/* Doesn't have a size. */
				break;
		}

		Emit(",\n\t");
		Emit("SetMSBBitIndex,\n\t");
	}
}

void EmitInstructionSourceAddressMode(const Instruction instruction)
{
	if (Instruction_IsSourceOperandRead(instruction))
	{
		/* Decode source address mode. */

		switch (Instruction_GetSourceOperand(instruction))
		{
			case INSTRUCTION_SOURCE_IMMEDIATE_DATA:
				/* Immediate value (any size). */
				Emit("DecodeSource_ImmediateData");
				break;

			case INSTRUCTION_SOURCE_DATA_REGISTER_SECONDARY:
				/* Secondary data register. */
				Emit("DecodeSource_DataRegisterSecondary");
				break;

			case INSTRUCTION_SOURCE_IMMEDIATE_DATA_BYTE:
				/* Immediate value (byte). */
				Emit("DecodeSource_ImmediateDataByte");
				break;

			case INSTRUCTION_SOURCE_MEMORY_ADDRESS_PRIMARY:
				/* Memory address */
				Emit("DecodeSource_MemoryAddressPrimary");
				break;

			case INSTRUCTION_SOURCE_STATUS_REGISTER:
				Emit("DecodeSource_StatusRegister");
				break;

			case INSTRUCTION_SOURCE_IMMEDIATE_DATA_WORD:
				/* Immediate value (word). */
				Emit("DecodeSource_ImmediateDataWord");
				break;

			case INSTRUCTION_SOURCE_BCD_X:
				Emit("DecodeSource_BCDX");
				break;

			case INSTRUCTION_SOURCE_DATA_REGISTER_SECONDARY_OR_PRIMARY_ADDRESS_MODE:
				/* Primary address mode or secondary data register, based on direction bit. */
				Emit("DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE_SIZED:
				/* Word or longword based on bit 8. */
				Emit("DecodeSource_PrimaryAddressModeSized");
				break;

			case INSTRUCTION_SOURCE_ADDRESS_REGISTER_PRIMARY_POSTINCREMENT:
				Emit("DecodeSource_AddressRegisterPrimaryPostIncrement");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE:
				/* Primary address mode. */
				Emit("DecodeSource_PrimaryAddressMode");
				break;

			case INSTRUCTION_SOURCE_PRIMARY_ADDRESS_MODE_WORD:
				/* Primary address mode, hardcoded to word-size. */
				Emit("DecodeSource_PrimaryAddressModeWord");
				break;

			case INSTRUCTION_SOURCE_NONE:
				/* Doesn't have a source address mode to decode. */
				break;
		}

		Emit(",\n\t");
	}
}

void EmitInstructionDestinationAddressMode(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandRead(instruction) || Instruction_IsDestinationOperandWritten(instruction))
	{
		/* Decode destination address mode. */

		switch (Instruction_GetDestinationOperand(instruction))
		{
			case INSTRUCTION_DESTINATION_DATA_REGISTER_PRIMARY:
				/* Data register (primary) */
				Emit("DecodeDestination_DataRegisterPrimary");
				break;

			case INSTRUCTION_DESTINATION_DATA_REGISTER_SECONDARY:
				/* Data register (secondary) */
				Emit("DecodeDestination_DataRegisterSecondary");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY:
				/* Address register (secondary) */
				Emit("DecodeDestination_AddressRegisterSecondary");
				break;

			case INSTRUCTION_DESTINATION_SECONDARY_ADDRESS_MODE:
				/* Secondary address mode */
				Emit("DecodeDestination_SecondaryAddressMode");
				break;

			case INSTRUCTION_DESTINATION_BCD_X:
				Emit("DecodeDestination_BCDX");
				break;

			case INSTRUCTION_DESTINATION_DATA_REGISTER_SECONDARY_OR_PRIMARY_ADDRESS_MODE:
				/* Primary address mode or secondary data register, based on direction bit */
				Emit("DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY_FULL:
				/* Full secondary address register */
				Emit("DecodeDestination_AddressRegisterSecondaryFull");
				break;

			case INSTRUCTION_DESTINATION_ADDRESS_REGISTER_SECONDARY_POSTINCREMENT:
				Emit("DecodeDestination_AddressRegisterSecondaryPostIncrement");
				break;

			case INSTRUCTION_DESTINATION_PRIMARY_ADDRESS_MODE:
				/* Using primary address mode */
				Emit("DecodeDestination_PrimaryAddressMode");
				break;

			case INSTRUCTION_DESTINATION_CONDITION_CODE_REGISTER:
				Emit("DecodeDestination_ConditionCodeRegister");
				break;

			case INSTRUCTION_DESTINATION_STATUS_REGISTER:
				Emit("DecodeDestination_StatusRegister");
				break;

			case INSTRUCTION_DESTINATION_MOVEM:
				/* Memory address */
				Emit("DecodeDestination_MOVEM");
				break;

			case INSTRUCTION_DESTINATION_MOVEP:
				/* Memory address */
				Emit("DecodeDestination_MOVEP");
				break;

			case INSTRUCTION_DESTINATION_NONE:
				/* Doesn't have a destination address mode to decode. */
				break;
		}

		Emit(",\n\t");
	}
}

void EmitInstructionReadSourceOperand(const Instruction instruction)
{
	if (Instruction_IsSourceOperandRead(instruction))
	{
		/* Read source operand. */
		Emit("ReadSource,\n\t");
	}
}

void EmitInstructionReadDestinationOperand(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandRead(instruction))
	{
		/* Read destination operand. */
		Emit("ReadDestination,\n\t");
	}
}

void EmitInstructionAction(const Instruction instruction)
{
	/* Do the actual instruction. */

	switch (Instruction_GetAction(instruction))
	{
		case INSTRUCTION_ACTION_OR:
			Emit("Action_OR");
			break;

		case INSTRUCTION_ACTION_AND:
			Emit("Action_AND");
			break;

		case INSTRUCTION_ACTION_SUBA:
			Emit("Action_SUBA");
			break;

		case INSTRUCTION_ACTION_SUBQ:
			Emit("Action_SUBQ");
			break;

		case INSTRUCTION_ACTION_SUB:
			Emit("Action_SUB");
			break;

		case INSTRUCTION_ACTION_ADDA:
			Emit("Action_ADDA");
			break;

		case INSTRUCTION_ACTION_ADDQ:
			Emit("Action_ADDQ");
			break;

		case INSTRUCTION_ACTION_ADD:
			Emit("Action_ADD");
			break;

		case INSTRUCTION_ACTION_EOR:
			Emit("Action_EOR");
			break;

		case INSTRUCTION_ACTION_BCHG:
			Emit("Action_BCHG");
			break;

		case INSTRUCTION_ACTION_BCLR:
			Emit("Action_BCLR");
			break;

		case INSTRUCTION_ACTION_BSET:
			Emit("Action_BSET");
			break;

		case INSTRUCTION_ACTION_BTST:
			Emit("Action_BTST");
			break;

		case INSTRUCTION_ACTION_MOVEP:
			Emit("Action_MOVEP");
			break;

		case INSTRUCTION_ACTION_MOVEA:
			Emit("Action_MOVEA");
			break;

		case INSTRUCTION_ACTION_MOVE:
			Emit("Action_MOVE");
			break;

		case INSTRUCTION_ACTION_LINK:
			Emit("Action_LINK");
			break;

		case INSTRUCTION_ACTION_UNLK:
			Emit("Action_UNLK");
			break;

		case INSTRUCTION_ACTION_NEGX:
			Emit("Action_NEGX");
			break;

		case INSTRUCTION_ACTION_CLR:
			Emit("Action_CLR");
			break;

		case INSTRUCTION_ACTION_NEG:
			Emit("Action_NEG");
			break;

		case INSTRUCTION_ACTION_NOT:
			Emit("Action_NOT");
			break;

		case INSTRUCTION_ACTION_EXT:
			Emit("Action_EXT");
			break;

		case INSTRUCTION_ACTION_NBCD:
			Emit("Action_NBCD");
			break;

		case INSTRUCTION_ACTION_SWAP:
			Emit("Action_SWAP");
			break;

		case INSTRUCTION_ACTION_PEA:
			Emit("Action_PEA");
			break;

		case INSTRUCTION_ACTION_ILLEGAL:
			Emit("Action_ILLEGAL");
			break;

		case INSTRUCTION_ACTION_TAS:
			Emit("Action_TAS");
			break;

		case INSTRUCTION_ACTION_TRAP:
			Emit("Action_TRAP");
			break;

		case INSTRUCTION_ACTION_MOVE_USP:
			Emit("Action_MOVE_USP");
			break;

		case INSTRUCTION_ACTION_RESET:
			Emit("Action_RESET");
			break;

		case INSTRUCTION_ACTION_STOP:
			Emit("Action_STOP");
			break;

		case INSTRUCTION_ACTION_RTE:
			Emit("Action_RTE");
			break;

		case INSTRUCTION_ACTION_RTS:
			Emit("Action_RTS");
			break;

		case INSTRUCTION_ACTION_TRAPV:
			Emit("Action_TRAPV");
			break;

		case INSTRUCTION_ACTION_RTR:
			Emit("Action_RTR");
			break;

		case INSTRUCTION_ACTION_JSR:
			Emit("Action_JSR");
			break;

		case INSTRUCTION_ACTION_JMP:
			Emit("Action_JMP");
			break;

		case INSTRUCTION_ACTION_MOVEM:
			Emit("Action_MOVEM");
			break;

		case INSTRUCTION_ACTION_CHK:
			Emit("Action_CHK");
			break;

		case INSTRUCTION_ACTION_SCC:
			Emit("Action_SCC");
			break;

		case INSTRUCTION_ACTION_DBCC:
			Emit("Action_DBCC");
			break;

		case INSTRUCTION_ACTION_BRA_SHORT:
			Emit("Action_BRA_SHORT");
			break;

		case INSTRUCTION_ACTION_BRA_WORD:
			Emit("Action_BRA_WORD");
			break;

		case INSTRUCTION_ACTION_BSR_SHORT:
			Emit("Action_BSR_SHORT");
			break;

		case INSTRUCTION_ACTION_BSR_WORD:
			Emit("Action_BSR_WORD");
			break;

		case INSTRUCTION_ACTION_BCC_SHORT:
			Emit("Action_BCC_SHORT");
			break;

		case INSTRUCTION_ACTION_BCC_WORD:
			Emit("Action_BCC_WORD");
			break;

		case INSTRUCTION_ACTION_MOVEQ:
			Emit("Action_MOVEQ");
			break;

		case INSTRUCTION_ACTION_DIVS:
			Emit("Action_DIVS");
			break;

		case INSTRUCTION_ACTION_DIVU:
			Emit("Action_DIVU");
			break;

		case INSTRUCTION_ACTION_SBCD:
			Emit("Action_SBCD");
			break;

		case INSTRUCTION_ACTION_SUBX:
			Emit("Action_SUBX");
			break;

		case INSTRUCTION_ACTION_MULS:
			Emit("Action_MULS");
			break;

		case INSTRUCTION_ACTION_MULU:
			Emit("Action_MULU");
			break;

		case INSTRUCTION_ACTION_ABCD:
			Emit("Action_ABCD");
			break;

		case INSTRUCTION_ACTION_EXG:
			Emit("Action_EXG");
			break;

		case INSTRUCTION_ACTION_ADDX:
			Emit("Action_ADDX");
			break;

		case INSTRUCTION_ACTION_ASD_MEMORY:
			Emit("Action_ASD_MEMORY");
			break;

		case INSTRUCTION_ACTION_ASD_REGISTER:
			Emit("Action_ASD_REGISTER");
			break;

		case INSTRUCTION_ACTION_LSD_MEMORY:
			Emit("Action_LSD_MEMORY");
			break;

		case INSTRUCTION_ACTION_LSD_REGISTER:
			Emit("Action_LSD_REGISTER");
			break;

		case INSTRUCTION_ACTION_ROD_MEMORY:
			Emit("Action_ROD_MEMORY");
			break;

		case INSTRUCTION_ACTION_ROD_REGISTER:
			Emit("Action_ROD_REGISTER");
			break;

		case INSTRUCTION_ACTION_ROXD_MEMORY:
			Emit("Action_ROXD_MEMORY");
			break;

		case INSTRUCTION_ACTION_ROXD_REGISTER:
			Emit("Action_ROXD_REGISTER");
			break;

		case INSTRUCTION_ACTION_UNIMPLEMENTED_1:
			Emit("Action_UNIMPLEMENTED_1");
			break;

		case INSTRUCTION_ACTION_UNIMPLEMENTED_2:
			Emit("Action_UNIMPLEMENTED_2");
			break;

		case INSTRUCTION_ACTION_NOP:
			Emit("Action_NOP");
			break;
	}

	Emit(",\n\t");
}

void EmitInstructionWriteDestinationOperand(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandWritten(instruction))
	{
		/* Write destination operand. */
		Emit("WriteDestination,\n\t");
	}
}

void EmitInstructionConditionCodes(const Instruction instruction)
{
	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	switch (Instruction_GetCarryModifier(instruction))
	{
		case INSTRUCTION_CARRY_STANDARD_CARRY:
			Emit("Carry_StandardCarry,\n\t");
			break;

		case INSTRUCTION_CARRY_STANDARD_BORROW:
			Emit("Carry_StandardBorrow,\n\t");
			break;

		case INSTRUCTION_CARRY_NEG:
			Emit("Carry_NEG,\n\t");
			break;

		case INSTRUCTION_CARRY_CLEAR:
			Emit("Carry_Clear,\n\t");
			break;

		case INSTRUCTION_CARRY_UNDEFINED:
			/* Undefined */
			break;

		case INSTRUCTION_CARRY_UNAFFECTED:
			/* Unaffected */
			break;
	}

	/* Update OVERFLOW condition code */
	switch (Instruction_GetOverflowModifier(instruction))
	{
		case INSTRUCTION_OVERFLOW_ADD:
			Emit("Overflow_ADD,\n\t");
			break;

		case INSTRUCTION_OVERFLOW_SUB:
			Emit("Overflow_SUB,\n\t");
			break;

		case INSTRUCTION_OVERFLOW_NEG:
			Emit("Overflow_NEG,\n\t");
			break;

		case INSTRUCTION_OVERFLOW_CLEARED:
			Emit("Overflow_Clear,\n\t");
			break;

		case INSTRUCTION_OVERFLOW_UNDEFINED:
			/* Undefined */
			break;

		case INSTRUCTION_OVERFLOW_UNAFFECTED:
			/* Unaffected */
			break;
	}

	/* Update ZERO condition code */
	switch (Instruction_GetZeroModifier(instruction))
	{
		case INSTRUCTION_ZERO_CLEAR_IF_NONZERO_UNAFFECTED_OTHERWISE:
			Emit("Zero_ClearIfNonZeroUnaffectedOtherwise,\n\t");
			break;

		case INSTRUCTION_ZERO_SET_IF_ZERO_CLEAR_OTHERWISE:
			Emit("Zero_SetIfZeroClearOtherwise,\n\t");
			break;

		case INSTRUCTION_ZERO_UNDEFINED:
			/* Undefined */
			break;

		case INSTRUCTION_ZERO_UNAFFECTED:
			/* Unaffected */
			break;
	}

	/* Update NEGATIVE condition code */
	switch (Instruction_GetNegativeModifier(instruction))
	{
		case INSTRUCTION_NEGATIVE_SET_IF_NEGATIVE_CLEAR_OTHERWISE:
			Emit("Negative_SetIfNegativeClearOtherwise,\n\t");
			break;

		case INSTRUCTION_NEGATIVE_UNAFFECTED:
			/* Unaffected */
			break;
	}

	/* Update EXTEND condition code */
	switch (Instruction_GetExtendModifier(instruction))
	{
		case INSTRUCTION_EXTEND_SET_TO_CARRY:
			Emit("Extend_SetToCarry,\n\t");
			break;

		case INSTRUCTION_EXTEND_UNAFFECTED:
			/* Unaffected */
			break;
	}
}
