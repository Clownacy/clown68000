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

#ifndef M68K_INSTRUCTION_PROPERTIES_H
#define M68K_INSTRUCTION_PROPERTIES_H

#include "../clowncommon/clowncommon.h"

#include "instruction.h"

typedef enum InstructionAction
{
	INSTRUCTION_ACTION_OR,
	INSTRUCTION_ACTION_AND,
	INSTRUCTION_ACTION_SUBA,
	INSTRUCTION_ACTION_SUBQ,
	INSTRUCTION_ACTION_SUB,
	INSTRUCTION_ACTION_ADDA,
	INSTRUCTION_ACTION_ADDQ,
	INSTRUCTION_ACTION_ADD,
	INSTRUCTION_ACTION_EOR,
	INSTRUCTION_ACTION_BCHG,
	INSTRUCTION_ACTION_BCLR,
	INSTRUCTION_ACTION_BSET,
	INSTRUCTION_ACTION_BTST,
	INSTRUCTION_ACTION_MOVEP,
	INSTRUCTION_ACTION_MOVEA,
	INSTRUCTION_ACTION_MOVE,
	INSTRUCTION_ACTION_LINK,
	INSTRUCTION_ACTION_UNLK,
	INSTRUCTION_ACTION_NEGX,
	INSTRUCTION_ACTION_CLR,
	INSTRUCTION_ACTION_NEG,
	INSTRUCTION_ACTION_NOT,
	INSTRUCTION_ACTION_EXT,
	INSTRUCTION_ACTION_NBCD,
	INSTRUCTION_ACTION_SWAP,
	INSTRUCTION_ACTION_PEA,
	INSTRUCTION_ACTION_ILLEGAL,
	INSTRUCTION_ACTION_TAS,
	INSTRUCTION_ACTION_TRAP,
	INSTRUCTION_ACTION_MOVE_USP,
	INSTRUCTION_ACTION_RESET,
	INSTRUCTION_ACTION_STOP,
	INSTRUCTION_ACTION_RTE,
	INSTRUCTION_ACTION_RTS,
	INSTRUCTION_ACTION_TRAPV,
	INSTRUCTION_ACTION_RTR,
	INSTRUCTION_ACTION_JSR,
	INSTRUCTION_ACTION_JMP,
	INSTRUCTION_ACTION_MOVEM,
	INSTRUCTION_ACTION_CHK,
	INSTRUCTION_ACTION_SCC,
	INSTRUCTION_ACTION_DBCC,
	INSTRUCTION_ACTION_BRA_SHORT,
	INSTRUCTION_ACTION_BRA_WORD,
	INSTRUCTION_ACTION_BSR_SHORT,
	INSTRUCTION_ACTION_BSR_WORD,
	INSTRUCTION_ACTION_BCC_SHORT,
	INSTRUCTION_ACTION_BCC_WORD,
	INSTRUCTION_ACTION_MOVEQ,
	INSTRUCTION_ACTION_DIV,
	INSTRUCTION_ACTION_SBCD,
	INSTRUCTION_ACTION_SUBX,
	INSTRUCTION_ACTION_MUL,
	INSTRUCTION_ACTION_ABCD,
	INSTRUCTION_ACTION_EXG,
	INSTRUCTION_ACTION_ADDX,
	INSTRUCTION_ACTION_ASD_MEMORY,
	INSTRUCTION_ACTION_ASD_REGISTER,
	INSTRUCTION_ACTION_LSD_MEMORY,
	INSTRUCTION_ACTION_LSD_REGISTER,
	INSTRUCTION_ACTION_ROD_MEMORY,
	INSTRUCTION_ACTION_ROD_REGISTER,
	INSTRUCTION_ACTION_ROXD_MEMORY,
	INSTRUCTION_ACTION_ROXD_REGISTER,
	INSTRUCTION_ACTION_UNIMPLEMENTED_1,
	INSTRUCTION_ACTION_UNIMPLEMENTED_2,
	INSTRUCTION_ACTION_NOP
} InstructionAction;

typedef enum InstructionCarry
{
	INSTRUCTION_CARRY_STANDARD_CARRY,
	INSTRUCTION_CARRY_STANDARD_BORROW,
	INSTRUCTION_CARRY_NEG,
	INSTRUCTION_CARRY_CLEAR,
	INSTRUCTION_CARRY_UNDEFINED,
	INSTRUCTION_CARRY_UNAFFECTED
} InstructionCarry;

typedef enum InstructionOverflow
{
	INSTRUCTION_OVERFLOW_ADD,
	INSTRUCTION_OVERFLOW_SUB,
	INSTRUCTION_OVERFLOW_NEG,
	INSTRUCTION_OVERFLOW_CLEARED,
	INSTRUCTION_OVERFLOW_UNDEFINED,
	INSTRUCTION_OVERFLOW_UNAFFECTED
} InstructionOverflow;

typedef enum InstructionZero
{
	INSTRUCTION_ZERO_CLEAR_IF_NONZERO_UNAFFECTED_OTHERWISE,
	INSTRUCTION_ZERO_SET_IF_ZERO_CLEAR_OTHERWISE,
	INSTRUCTION_ZERO_UNDEFINED,
	INSTRUCTION_ZERO_UNAFFECTED
} InstructionZero;

typedef enum InstructionNegative
{
	INSTRUCTION_NEGATIVE_SET_IF_NEGATIVE_CLEAR_OTHERWISE,
	INSTRUCTION_NEGATIVE_UNAFFECTED
} InstructionNegative;

typedef enum InstructionExtend
{
	INSTRUCTION_EXTEND_SET_TO_CARRY,
	INSTRUCTION_EXTEND_UNAFFECTED
} InstructionExtend;

InstructionAction Instruction_GetAction(const Instruction instruction);
cc_bool Instruction_IsSourceOperandRead(const Instruction instruction);
cc_bool Instruction_IsDestinationOperandRead(const Instruction instruction);
cc_bool Instruction_IsDestinationOperandWritten(const Instruction instruction);
InstructionCarry Instruction_GetCarryModifier(const Instruction instruction);
InstructionOverflow Instruction_GetOverflowModifier(const Instruction instruction);
InstructionZero Instruction_GetZeroModifier(const Instruction instruction);
InstructionNegative Instruction_GetNegativeModifier(const Instruction instruction);
InstructionExtend Instruction_GetExtendModifier(const Instruction instruction);
cc_bool Instruction_IsPrivileged(const Instruction instruction);

#endif /* M68K_INSTRUCTION_PROPERTIES_H */
