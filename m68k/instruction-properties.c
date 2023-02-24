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

#include "instruction-properties.h"

#include <assert.h>

InstructionAction Instruction_GetAction(const Instruction instruction)
{
	/* Do the actual instruction */
	switch (instruction)
	{
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
			return INSTRUCTION_ACTION_OR;

		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
			return INSTRUCTION_ACTION_AND;

		case INSTRUCTION_CMPA:
		case INSTRUCTION_SUBA:
			return INSTRUCTION_ACTION_SUBA;

		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_SUBQ:
			return INSTRUCTION_ACTION_SUBQ;

		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
			return INSTRUCTION_ACTION_SUB;

		case INSTRUCTION_ADDA:
			return INSTRUCTION_ACTION_ADDA;

		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ADDQ:
			return INSTRUCTION_ACTION_ADDQ;

		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
			return INSTRUCTION_ACTION_ADD;

		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
			return INSTRUCTION_ACTION_EOR;

		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
			return INSTRUCTION_ACTION_BCHG;

		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
			return INSTRUCTION_ACTION_BCLR;

		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
			return INSTRUCTION_ACTION_BSET;

		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
			return INSTRUCTION_ACTION_BTST;

		case INSTRUCTION_MOVEP:
			return INSTRUCTION_ACTION_MOVEP;

		case INSTRUCTION_MOVEA:
			return INSTRUCTION_ACTION_MOVEA;

		case INSTRUCTION_LEA:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_TST:
			return INSTRUCTION_ACTION_MOVE;

		case INSTRUCTION_LINK:
			return INSTRUCTION_ACTION_LINK;

		case INSTRUCTION_UNLK:
			return INSTRUCTION_ACTION_UNLK;

		case INSTRUCTION_NEGX:
			return INSTRUCTION_ACTION_NEGX;

		case INSTRUCTION_CLR:
			return INSTRUCTION_ACTION_CLR;

		case INSTRUCTION_NEG:
			return INSTRUCTION_ACTION_NEG;

		case INSTRUCTION_NOT:
			return INSTRUCTION_ACTION_NOT;

		case INSTRUCTION_EXT:
			return INSTRUCTION_ACTION_EXT;

		case INSTRUCTION_NBCD:
			return INSTRUCTION_ACTION_NBCD;

		case INSTRUCTION_SWAP:
			return INSTRUCTION_ACTION_SWAP;

		case INSTRUCTION_PEA:
			return INSTRUCTION_ACTION_PEA;

		case INSTRUCTION_ILLEGAL:
			return INSTRUCTION_ACTION_ILLEGAL;

		case INSTRUCTION_TAS:
			return INSTRUCTION_ACTION_TAS;

		case INSTRUCTION_TRAP:
			return INSTRUCTION_ACTION_TRAP;

		case INSTRUCTION_MOVE_USP:
			return INSTRUCTION_ACTION_MOVE_USP;

		case INSTRUCTION_RESET:
			return INSTRUCTION_ACTION_RESET;

		case INSTRUCTION_STOP:
			return INSTRUCTION_ACTION_STOP;

		case INSTRUCTION_RTE:
			return INSTRUCTION_ACTION_RTE;

		case INSTRUCTION_RTS:
			return INSTRUCTION_ACTION_RTS;

		case INSTRUCTION_TRAPV:
			return INSTRUCTION_ACTION_TRAPV;

		case INSTRUCTION_RTR:
			return INSTRUCTION_ACTION_RTR;

		case INSTRUCTION_JSR:
			return INSTRUCTION_ACTION_JSR;

		case INSTRUCTION_JMP:
			return INSTRUCTION_ACTION_JMP;

		case INSTRUCTION_MOVEM:
			return INSTRUCTION_ACTION_MOVEM;

		case INSTRUCTION_CHK:
			return INSTRUCTION_ACTION_CHK;

		case INSTRUCTION_SCC:
			return INSTRUCTION_ACTION_SCC;

		case INSTRUCTION_DBCC:
			return INSTRUCTION_ACTION_DBCC;

		case INSTRUCTION_BRA_SHORT:
			return INSTRUCTION_ACTION_BRA_SHORT;

		case INSTRUCTION_BRA_WORD:
			return INSTRUCTION_ACTION_BRA_WORD;

		case INSTRUCTION_BSR_SHORT:
			return INSTRUCTION_ACTION_BSR_SHORT;

		case INSTRUCTION_BSR_WORD:
			return INSTRUCTION_ACTION_BSR_WORD;

		case INSTRUCTION_BCC_SHORT:
			return INSTRUCTION_ACTION_BCC_SHORT;

		case INSTRUCTION_BCC_WORD:
			return INSTRUCTION_ACTION_BCC_WORD;

		case INSTRUCTION_MOVEQ:
			return INSTRUCTION_ACTION_MOVEQ;

		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
			return INSTRUCTION_ACTION_DIV;

		case INSTRUCTION_SBCD:
			return INSTRUCTION_ACTION_SBCD;

		case INSTRUCTION_SUBX:
			return INSTRUCTION_ACTION_SUBX;

		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
			return INSTRUCTION_ACTION_MUL;

		case INSTRUCTION_ABCD:
			return INSTRUCTION_ACTION_ABCD;

		case INSTRUCTION_EXG:
			return INSTRUCTION_ACTION_EXG;

		case INSTRUCTION_ADDX:
			return INSTRUCTION_ACTION_ADDX;

		case INSTRUCTION_ASD_MEMORY:
			return INSTRUCTION_ACTION_ASD_MEMORY;

		case INSTRUCTION_ASD_REGISTER:
			return INSTRUCTION_ACTION_ASD_REGISTER;

		case INSTRUCTION_LSD_MEMORY:
			return INSTRUCTION_ACTION_LSD_MEMORY;

		case INSTRUCTION_LSD_REGISTER:
			return INSTRUCTION_ACTION_LSD_REGISTER;

		case INSTRUCTION_ROD_MEMORY:
			return INSTRUCTION_ACTION_ROD_MEMORY;

		case INSTRUCTION_ROD_REGISTER:
			return INSTRUCTION_ACTION_ROD_REGISTER;

		case INSTRUCTION_ROXD_MEMORY:
			return INSTRUCTION_ACTION_ROXD_MEMORY;

		case INSTRUCTION_ROXD_REGISTER:
			return INSTRUCTION_ACTION_ROXD_REGISTER;

		case INSTRUCTION_UNIMPLEMENTED_1:
			return INSTRUCTION_ACTION_UNIMPLEMENTED_1;

		case INSTRUCTION_UNIMPLEMENTED_2:
			return INSTRUCTION_ACTION_UNIMPLEMENTED_2;

		case INSTRUCTION_NOP:
			return INSTRUCTION_ACTION_NOP;
	}

	assert(cc_false);

	return INSTRUCTION_ACTION_NOP;
}

cc_bool Instruction_IsSourceOperandRead(const Instruction instruction)
{
	/* Obtain source value. */
	switch (instruction)
	{
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_STOP:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_EOR:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_ABCD:
		case INSTRUCTION_SUBX:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_OR:
		case INSTRUCTION_SUB:
		case INSTRUCTION_AND:
		case INSTRUCTION_ADD:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_CHK:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_MULU:
		case INSTRUCTION_MULS:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_TST:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_PEA:
		case INSTRUCTION_JSR:
		case INSTRUCTION_JMP:
		case INSTRUCTION_LEA:
			return cc_true;

		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_CLR:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NOT:
		case INSTRUCTION_EXT:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_TAS:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_RESET:
		case INSTRUCTION_NOP:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTS:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_RTR:
		case INSTRUCTION_SCC:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_EXG:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return cc_false;
	}

	assert(cc_false);

	return cc_false;
}

cc_bool Instruction_IsDestinationOperandRead(const Instruction instruction)
{
	/* Obtain destination value */
	switch (instruction)
	{
		case INSTRUCTION_CLR:
		case INSTRUCTION_SCC:
		/* For some reason, the above two instructions read from their destination even though they don't use it. */
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_EORI:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NOT:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_TAS:
		case INSTRUCTION_EXT:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_OR:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBX:
		case INSTRUCTION_EOR:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_ABCD:
		case INSTRUCTION_AND:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROD_REGISTER:
			return cc_true;

		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_CHK:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EXG:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_NOP:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_STOP:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_TST:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return cc_false;
	}

	assert(cc_false);

	return cc_false;
}

cc_bool Instruction_IsDestinationOperandWritten(const Instruction instruction)
{
	/* Write output to destination */
	switch (instruction)
	{
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_ABCD:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_CLR:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EXT:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_NOT:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_SCC:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_SUBX:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_TAS:
			return cc_true;

		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_CHK:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EXG:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_STOP:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_TST:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return cc_false;
	}

	assert(cc_false);

	return cc_false;
}

InstructionCarry Instruction_GetCarryModifier(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ABCD:
			return INSTRUCTION_CARRY_DECIMAL_CARRY;

		case INSTRUCTION_NBCD:
		case INSTRUCTION_SBCD:
			return INSTRUCTION_CARRY_DECIMAL_BORROW;

		case INSTRUCTION_ADDQ:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_ADDX:
			return INSTRUCTION_CARRY_STANDARD_CARRY;

		case INSTRUCTION_SUBQ:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SUBX:
			return INSTRUCTION_CARRY_STANDARD_BORROW;

		case INSTRUCTION_NEG:
		case INSTRUCTION_NEGX:
			return INSTRUCTION_CARRY_NEG;

		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_CLR:
		case INSTRUCTION_EXT:
		case INSTRUCTION_NOT:
		case INSTRUCTION_TAS:
		case INSTRUCTION_TST:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_SWAP:
			return INSTRUCTION_CARRY_CLEAR;

		case INSTRUCTION_CHK:
			return INSTRUCTION_CARRY_UNDEFINED;

		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_EXG:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_SCC:
		case INSTRUCTION_STOP:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return INSTRUCTION_CARRY_UNAFFECTED;
	}

	assert(cc_false);

	return INSTRUCTION_CARRY_UNAFFECTED;
}

InstructionOverflow Instruction_GetOverflowModifier(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ABCD:
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_ADDX:
			return INSTRUCTION_OVERFLOW_ADD;

		case INSTRUCTION_SUBQ:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SUBX:
			return INSTRUCTION_OVERFLOW_SUB;

		case INSTRUCTION_NEG:
		case INSTRUCTION_NEGX:
			return INSTRUCTION_OVERFLOW_NEG;

		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_CLR:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EXT:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_NOT:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_TAS:
		case INSTRUCTION_TST:
			return INSTRUCTION_OVERFLOW_CLEARED;

		case INSTRUCTION_CHK:
			return INSTRUCTION_OVERFLOW_UNDEFINED;

		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_EXG:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_SCC:
		case INSTRUCTION_STOP:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return INSTRUCTION_OVERFLOW_UNAFFECTED;
	}

	assert(cc_false);

	return INSTRUCTION_OVERFLOW_UNAFFECTED;
}

InstructionZero Instruction_GetZeroModifier(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ABCD:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_SUBX:
			return INSTRUCTION_ZERO_CLEAR_IF_NONZERO_UNAFFECTED_OTHERWISE;

		case INSTRUCTION_ADDQ:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_CLR:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EXT:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NOT:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_TST:
			return INSTRUCTION_ZERO_SET_IF_ZERO_CLEAR_OTHERWISE;

		case INSTRUCTION_CHK:
			return INSTRUCTION_ZERO_UNDEFINED;

		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_TAS:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_EXG:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_SCC:
		case INSTRUCTION_STOP:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return INSTRUCTION_ZERO_UNAFFECTED;
	}

	assert(cc_false);

	return INSTRUCTION_ZERO_UNAFFECTED;
}

InstructionNegative Instruction_GetNegativeModifier(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ABCD:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_CLR:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EXT:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_NOT:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SUBX:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_TST:
			return INSTRUCTION_NEGATIVE_SET_IF_NEGATIVE_CLEAR_OTHERWISE;

		case INSTRUCTION_CHK:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_TAS:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_EXG:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_SCC:
		case INSTRUCTION_STOP:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return INSTRUCTION_NEGATIVE_UNAFFECTED;
	}

	assert(cc_false);

	return INSTRUCTION_NEGATIVE_UNAFFECTED;
}

InstructionExtend Instruction_GetExtendModifier(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_ABCD:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SUBX:
			return INSTRUCTION_EXTEND_SET_TO_CARRY;

		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_CLR:
		case INSTRUCTION_CHK:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EXT:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_NOT:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_TAS:
		case INSTRUCTION_TST:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_EXG:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_SCC:
		case INSTRUCTION_STOP:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return INSTRUCTION_EXTEND_UNAFFECTED;
	}

	assert(cc_false);

	return INSTRUCTION_EXTEND_UNAFFECTED;
}

cc_bool Instruction_IsPrivileged(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ANDI_TO_SR:
		case INSTRUCTION_EORI_TO_SR:
		case INSTRUCTION_MOVE_TO_SR:
		case INSTRUCTION_MOVE_USP:
		case INSTRUCTION_ORI_TO_SR:
		case INSTRUCTION_RESET:
		case INSTRUCTION_RTE:
		case INSTRUCTION_STOP:
			return cc_true;

		case INSTRUCTION_AND:
		case INSTRUCTION_ANDI:
		case INSTRUCTION_CLR:
		case INSTRUCTION_CHK:
		case INSTRUCTION_CMP:
		case INSTRUCTION_CMPA:
		case INSTRUCTION_CMPI:
		case INSTRUCTION_CMPM:
		case INSTRUCTION_EOR:
		case INSTRUCTION_EORI:
		case INSTRUCTION_EXT:
		case INSTRUCTION_MOVE:
		case INSTRUCTION_MOVEQ:
		case INSTRUCTION_NOT:
		case INSTRUCTION_OR:
		case INSTRUCTION_ORI:
		case INSTRUCTION_SWAP:
		case INSTRUCTION_TAS:
		case INSTRUCTION_TST:
		case INSTRUCTION_ASD_REGISTER:
		case INSTRUCTION_ASD_MEMORY:
		case INSTRUCTION_DIVS:
		case INSTRUCTION_DIVU:
		case INSTRUCTION_LSD_REGISTER:
		case INSTRUCTION_LSD_MEMORY:
		case INSTRUCTION_MULS:
		case INSTRUCTION_MULU:
		case INSTRUCTION_ROD_REGISTER:
		case INSTRUCTION_ROD_MEMORY:
		case INSTRUCTION_ROXD_REGISTER:
		case INSTRUCTION_ROXD_MEMORY:
		case INSTRUCTION_ADDQ:
		case INSTRUCTION_SUBQ:
		case INSTRUCTION_ABCD:
		case INSTRUCTION_ADD:
		case INSTRUCTION_ADDI:
		case INSTRUCTION_ADDX:
		case INSTRUCTION_NBCD:
		case INSTRUCTION_NEG:
		case INSTRUCTION_NEGX:
		case INSTRUCTION_SBCD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_SUBI:
		case INSTRUCTION_SUBX:
		case INSTRUCTION_ADDA:
		case INSTRUCTION_ADDAQ:
		case INSTRUCTION_ANDI_TO_CCR:
		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
		case INSTRUCTION_BCHG_DYNAMIC:
		case INSTRUCTION_BCHG_STATIC:
		case INSTRUCTION_BCLR_DYNAMIC:
		case INSTRUCTION_BCLR_STATIC:
		case INSTRUCTION_BRA_SHORT:
		case INSTRUCTION_BRA_WORD:
		case INSTRUCTION_BSET_DYNAMIC:
		case INSTRUCTION_BSET_STATIC:
		case INSTRUCTION_BSR_SHORT:
		case INSTRUCTION_BSR_WORD:
		case INSTRUCTION_BTST_DYNAMIC:
		case INSTRUCTION_BTST_STATIC:
		case INSTRUCTION_DBCC:
		case INSTRUCTION_EORI_TO_CCR:
		case INSTRUCTION_EXG:
		case INSTRUCTION_JMP:
		case INSTRUCTION_JSR:
		case INSTRUCTION_ILLEGAL:
		case INSTRUCTION_LEA:
		case INSTRUCTION_LINK:
		case INSTRUCTION_MOVE_FROM_SR:
		case INSTRUCTION_MOVE_TO_CCR:
		case INSTRUCTION_MOVEA:
		case INSTRUCTION_MOVEM:
		case INSTRUCTION_MOVEP:
		case INSTRUCTION_NOP:
		case INSTRUCTION_ORI_TO_CCR:
		case INSTRUCTION_PEA:
		case INSTRUCTION_RTR:
		case INSTRUCTION_RTS:
		case INSTRUCTION_SCC:
		case INSTRUCTION_SUBA:
		case INSTRUCTION_SUBAQ:
		case INSTRUCTION_TRAP:
		case INSTRUCTION_TRAPV:
		case INSTRUCTION_UNLK:
		case INSTRUCTION_UNIMPLEMENTED_1:
		case INSTRUCTION_UNIMPLEMENTED_2:
			return cc_false;
	}

	assert(cc_false);

	return cc_false;
}
