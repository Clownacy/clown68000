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

#ifndef INSTRUCTION_ACTIONS_H
#define INSTRUCTION_ACTIONS_H


#ifdef EMIT


#include "emit-instructions.h"

#define DO_INSTRUCTION_ACTION_OR\
	Emit("DO_INSTRUCTION_ACTION_OR;")

#define DO_INSTRUCTION_ACTION_AND\
	Emit("DO_INSTRUCTION_ACTION_AND;")

#define DO_INSTRUCTION_ACTION_SUBA\
	Emit("DO_INSTRUCTION_ACTION_SUBA;")

#define DO_INSTRUCTION_ACTION_SUBQ\
	Emit("DO_INSTRUCTION_ACTION_SUBQ;")

#define DO_INSTRUCTION_ACTION_SUB\
	Emit("DO_INSTRUCTION_ACTION_SUB;")

#define DO_INSTRUCTION_ACTION_ADDA\
	Emit("DO_INSTRUCTION_ACTION_ADDA;")

#define DO_INSTRUCTION_ACTION_ADDQ\
	Emit("DO_INSTRUCTION_ACTION_ADDQ;")

#define DO_INSTRUCTION_ACTION_ADD\
	Emit("DO_INSTRUCTION_ACTION_ADD;")

#define DO_INSTRUCTION_ACTION_EOR\
	Emit("DO_INSTRUCTION_ACTION_EOR;")

#define DO_INSTRUCTION_ACTION_BTST\
	Emit("DO_INSTRUCTION_ACTION_BTST;")

#define DO_INSTRUCTION_ACTION_BCHG\
	Emit("DO_INSTRUCTION_ACTION_BCHG;")

#define DO_INSTRUCTION_ACTION_BCLR\
	Emit("DO_INSTRUCTION_ACTION_BCLR;")

#define DO_INSTRUCTION_ACTION_BSET\
	Emit("DO_INSTRUCTION_ACTION_BSET;")

#define DO_INSTRUCTION_ACTION_MOVEP\
	Emit("DO_INSTRUCTION_ACTION_MOVEP;")

#define DO_INSTRUCTION_ACTION_MOVEA\
	Emit("DO_INSTRUCTION_ACTION_MOVEA;")

#define DO_INSTRUCTION_ACTION_MOVE\
	Emit("DO_INSTRUCTION_ACTION_MOVE;")

#define DO_INSTRUCTION_ACTION_LINK\
	Emit("DO_INSTRUCTION_ACTION_LINK;")

#define DO_INSTRUCTION_ACTION_UNLK\
	Emit("DO_INSTRUCTION_ACTION_UNLK;")

#define DO_INSTRUCTION_ACTION_NEGX\
	Emit("DO_INSTRUCTION_ACTION_NEGX;")

#define DO_INSTRUCTION_ACTION_CLR\
	Emit("DO_INSTRUCTION_ACTION_CLR;")

#define DO_INSTRUCTION_ACTION_NEG\
	Emit("DO_INSTRUCTION_ACTION_NEG;")

#define DO_INSTRUCTION_ACTION_NOT\
	Emit("DO_INSTRUCTION_ACTION_NOT;")

#define DO_INSTRUCTION_ACTION_EXT\
	Emit("DO_INSTRUCTION_ACTION_EXT;")

#define DO_INSTRUCTION_ACTION_NBCD\
	Emit("DO_INSTRUCTION_ACTION_NBCD;")

#define DO_INSTRUCTION_ACTION_SWAP\
	Emit("DO_INSTRUCTION_ACTION_SWAP;")

#define DO_INSTRUCTION_ACTION_PEA\
	Emit("DO_INSTRUCTION_ACTION_PEA;")

#define DO_INSTRUCTION_ACTION_ILLEGAL\
	Emit("DO_INSTRUCTION_ACTION_ILLEGAL;")

#define DO_INSTRUCTION_ACTION_TAS\
	Emit("DO_INSTRUCTION_ACTION_TAS;")

#define DO_INSTRUCTION_ACTION_TRAP\
	Emit("DO_INSTRUCTION_ACTION_TRAP;")

#define DO_INSTRUCTION_ACTION_MOVE_USP\
	Emit("DO_INSTRUCTION_ACTION_MOVE_USP;")

#define DO_INSTRUCTION_ACTION_RESET\
	Emit("DO_INSTRUCTION_ACTION_RESET;")

#define DO_INSTRUCTION_ACTION_STOP\
	Emit("DO_INSTRUCTION_ACTION_STOP;")

#define DO_INSTRUCTION_ACTION_RTE\
	Emit("DO_INSTRUCTION_ACTION_RTE;")

#define DO_INSTRUCTION_ACTION_RTS\
	Emit("DO_INSTRUCTION_ACTION_RTS;")

#define DO_INSTRUCTION_ACTION_TRAPV\
	Emit("DO_INSTRUCTION_ACTION_TRAPV;")

#define DO_INSTRUCTION_ACTION_RTR\
	Emit("DO_INSTRUCTION_ACTION_RTR;")

#define DO_INSTRUCTION_ACTION_JSR\
	Emit("DO_INSTRUCTION_ACTION_JSR;")

#define DO_INSTRUCTION_ACTION_JMP\
	Emit("DO_INSTRUCTION_ACTION_JMP;")

#define DO_INSTRUCTION_ACTION_MOVEM\
	Emit("DO_INSTRUCTION_ACTION_MOVEM;")

#define DO_INSTRUCTION_ACTION_CHK\
	Emit("DO_INSTRUCTION_ACTION_CHK;")

#define DO_INSTRUCTION_ACTION_SCC\
	Emit("DO_INSTRUCTION_ACTION_SCC;")

#define DO_INSTRUCTION_ACTION_DBCC\
	Emit("DO_INSTRUCTION_ACTION_DBCC;")

#define DO_INSTRUCTION_ACTION_BRA_SHORT\
	Emit("DO_INSTRUCTION_ACTION_BRA_SHORT;")

#define DO_INSTRUCTION_ACTION_BRA_WORD\
	Emit("DO_INSTRUCTION_ACTION_BRA_WORD;")

#define DO_INSTRUCTION_ACTION_BSR_SHORT\
	Emit("DO_INSTRUCTION_ACTION_BSR_SHORT;")

#define DO_INSTRUCTION_ACTION_BSR_WORD\
	Emit("DO_INSTRUCTION_ACTION_BSR_WORD;")

#define DO_INSTRUCTION_ACTION_BCC_SHORT\
	Emit("DO_INSTRUCTION_ACTION_BCC_SHORT;")

#define DO_INSTRUCTION_ACTION_BCC_WORD\
	Emit("DO_INSTRUCTION_ACTION_BCC_WORD;")

#define DO_INSTRUCTION_ACTION_MOVEQ\
	Emit("DO_INSTRUCTION_ACTION_MOVEQ;")

#define DO_INSTRUCTION_ACTION_DIVS\
	Emit("DO_INSTRUCTION_ACTION_DIVS;")

#define DO_INSTRUCTION_ACTION_DIVU\
	Emit("DO_INSTRUCTION_ACTION_DIVU;")

#define DO_INSTRUCTION_ACTION_SBCD\
	Emit("DO_INSTRUCTION_ACTION_SBCD;")

#define DO_INSTRUCTION_ACTION_SUBX\
	Emit("DO_INSTRUCTION_ACTION_SUBX;")

#define DO_INSTRUCTION_ACTION_MULS\
	Emit("DO_INSTRUCTION_ACTION_MULS;")

#define DO_INSTRUCTION_ACTION_MULU\
	Emit("DO_INSTRUCTION_ACTION_MULU;")

#define DO_INSTRUCTION_ACTION_ABCD\
	Emit("DO_INSTRUCTION_ACTION_ABCD;")

#define DO_INSTRUCTION_ACTION_EXG\
	Emit("DO_INSTRUCTION_ACTION_EXG;")

#define DO_INSTRUCTION_ACTION_ADDX\
	Emit("DO_INSTRUCTION_ACTION_ADDX;")

#define DO_INSTRUCTION_ACTION_ASD_MEMORY\
	Emit("DO_INSTRUCTION_ACTION_ASD_MEMORY;")

#define DO_INSTRUCTION_ACTION_ASD_REGISTER\
	Emit("DO_INSTRUCTION_ACTION_ASD_REGISTER;")

#define DO_INSTRUCTION_ACTION_LSD_MEMORY\
	Emit("DO_INSTRUCTION_ACTION_LSD_MEMORY;")

#define DO_INSTRUCTION_ACTION_LSD_REGISTER\
	Emit("DO_INSTRUCTION_ACTION_LSD_REGISTER;")

#define DO_INSTRUCTION_ACTION_ROD_MEMORY\
	Emit("DO_INSTRUCTION_ACTION_ROD_MEMORY;")

#define DO_INSTRUCTION_ACTION_ROD_REGISTER\
	Emit("DO_INSTRUCTION_ACTION_ROD_REGISTER;")

#define DO_INSTRUCTION_ACTION_ROXD_MEMORY\
	Emit("DO_INSTRUCTION_ACTION_ROXD_MEMORY;")

#define DO_INSTRUCTION_ACTION_ROXD_REGISTER\
	Emit("DO_INSTRUCTION_ACTION_ROXD_REGISTER;")

#define DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1\
	Emit("DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1;")

#define DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2\
	Emit("DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2;")

#define DO_INSTRUCTION_ACTION_NOP\
	Emit("DO_INSTRUCTION_ACTION_NOP;")


#else


#include <stdio.h>

#define UNIMPLEMENTED_INSTRUCTION(instruction) Clown68000_PrintError("Unimplemented instruction " instruction " used at 0x%" CC_PRIXLEAST32, state->program_counter)

#define DO_INSTRUCTION_ACTION_OR\
	closure.result_value = closure.destination_value | closure.source_value

#define DO_INSTRUCTION_ACTION_AND\
	closure.result_value = closure.destination_value & closure.source_value

#define DO_INSTRUCTION_ACTION_SUBA\
	if (!opcode.bit_8)\
		closure.source_value = CC_SIGN_EXTEND_ULONG(15, closure.source_value);\
\
	DO_INSTRUCTION_ACTION_SUB

#define DO_INSTRUCTION_ACTION_SUBQ\
	closure.source_value = ((opcode.secondary_register - 1u) & 7u) + 1u; /* A little math trick to turn 0 into 8. */\
	DO_INSTRUCTION_ACTION_SUB

#define DO_INSTRUCTION_ACTION_SUB\
	closure.result_value = closure.destination_value - closure.source_value

#define DO_INSTRUCTION_ACTION_ADDA\
	if (!opcode.bit_8)\
		closure.source_value = CC_SIGN_EXTEND_ULONG(15, closure.source_value);\
\
	DO_INSTRUCTION_ACTION_ADD

#define DO_INSTRUCTION_ACTION_ADDQ\
	closure.source_value = ((opcode.secondary_register - 1u) & 7u) + 1u; /* A little math trick to turn 0 into 8. */\
	DO_INSTRUCTION_ACTION_ADD

#define DO_INSTRUCTION_ACTION_ADD\
	closure.result_value = closure.destination_value + closure.source_value

#define DO_INSTRUCTION_ACTION_EOR\
	closure.result_value = closure.destination_value ^ closure.source_value

#define DO_INSTRUCTION_ACTION_BTST\
	/* Modulo the source value */\
	closure.source_value &= operation_size * 8 - 1;\
\
	/* Set the zero flag to the specified bit */\
	state->status_register &= ~CONDITION_CODE_ZERO;\
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((closure.destination_value & (1ul << closure.source_value)) == 0))

#define DO_INSTRUCTION_ACTION_BCHG\
	DO_INSTRUCTION_ACTION_BTST;\
	closure.result_value = closure.destination_value ^ (1ul << closure.source_value)

#define DO_INSTRUCTION_ACTION_BCLR\
	DO_INSTRUCTION_ACTION_BTST;\
	closure.result_value = closure.destination_value & ~(1ul << closure.source_value)

#define DO_INSTRUCTION_ACTION_BSET\
	DO_INSTRUCTION_ACTION_BTST;\
	closure.result_value = closure.destination_value | (1ul << closure.source_value)

#define DO_INSTRUCTION_ACTION_MOVEP\
	{\
	cc_u32f memory_address = closure.destination_value; /* TODO: Maybe get rid of this alias? */\
\
	switch (opcode.bits_6_and_7)\
	{\
		case 0:\
			/* Memory to register (word) */\
			state->data_registers[opcode.secondary_register] &= ~0xFFFFul;\
			state->data_registers[opcode.secondary_register] |= ReadByte(&closure.stuff, memory_address + 2 * 0) << 8 * 1;\
			state->data_registers[opcode.secondary_register] |= ReadByte(&closure.stuff, memory_address + 2 * 1) << 8 * 0;\
			break;\
\
		case 1:\
			/* Memory to register (longword) */\
			state->data_registers[opcode.secondary_register] = 0;\
			state->data_registers[opcode.secondary_register] |= ReadByte(&closure.stuff, memory_address + 2 * 0) << 8 * 3;\
			state->data_registers[opcode.secondary_register] |= ReadByte(&closure.stuff, memory_address + 2 * 1) << 8 * 2;\
			state->data_registers[opcode.secondary_register] |= ReadByte(&closure.stuff, memory_address + 2 * 2) << 8 * 1;\
			state->data_registers[opcode.secondary_register] |= ReadByte(&closure.stuff, memory_address + 2 * 3) << 8 * 0;\
			break;\
\
		case 2:\
			/* Register to memory (word) */\
			WriteByte(&closure.stuff, memory_address + 2 * 0, (state->data_registers[opcode.secondary_register] >> 8 * 1) & 0xFF);\
			WriteByte(&closure.stuff, memory_address + 2 * 1, (state->data_registers[opcode.secondary_register] >> 8 * 0) & 0xFF);\
			break;\
\
		case 3:\
			/* Register to memory (longword) */\
			WriteByte(&closure.stuff, memory_address + 2 * 0, (state->data_registers[opcode.secondary_register] >> 8 * 3) & 0xFF);\
			WriteByte(&closure.stuff, memory_address + 2 * 1, (state->data_registers[opcode.secondary_register] >> 8 * 2) & 0xFF);\
			WriteByte(&closure.stuff, memory_address + 2 * 2, (state->data_registers[opcode.secondary_register] >> 8 * 1) & 0xFF);\
			WriteByte(&closure.stuff, memory_address + 2 * 3, (state->data_registers[opcode.secondary_register] >> 8 * 0) & 0xFF);\
			break;\
	}\
	}

#define DO_INSTRUCTION_ACTION_MOVEA\
	closure.result_value = operation_size == 2 ? CC_SIGN_EXTEND_ULONG(15, closure.source_value) : closure.source_value

#define DO_INSTRUCTION_ACTION_MOVE\
	closure.result_value = closure.source_value

#define DO_INSTRUCTION_ACTION_LINK\
	/* Push address register to stack */\
	state->address_registers[7] -= 4;\
	WriteLongWord(&closure.stuff, state->address_registers[7], state->address_registers[opcode.primary_register]);\
\
	/* Copy stack pointer to address register */\
	state->address_registers[opcode.primary_register] = state->address_registers[7];\
\
	/* Offset the stack pointer by the immediate value */\
	state->address_registers[7] += CC_SIGN_EXTEND_ULONG(15, closure.source_value)

#define DO_INSTRUCTION_ACTION_UNLK\
	{\
	cc_u32l value;\
\
	state->address_registers[7] = state->address_registers[opcode.primary_register];\
	value = ReadLongWord(&closure.stuff, state->address_registers[7]);\
	state->address_registers[7] += 4;\
\
	/* We need to do this last in case we're writing to A7. */\
	state->address_registers[opcode.primary_register] = value;\
	}

#define DO_INSTRUCTION_ACTION_NEGX\
	closure.result_value = 0 - closure.destination_value - ((state->status_register & CONDITION_CODE_EXTEND) != 0 ? 1 : 0)

#define DO_INSTRUCTION_ACTION_CLR\
	closure.result_value = 0

#define DO_INSTRUCTION_ACTION_NEG\
	closure.result_value = 0 - closure.destination_value

#define DO_INSTRUCTION_ACTION_NOT\
	closure.result_value = ~closure.destination_value

#define DO_INSTRUCTION_ACTION_EXT\
	closure.result_value = CC_SIGN_EXTEND_ULONG((opcode.raw & 0x0040) != 0 ? 15 : 7, closure.destination_value)

#define DO_INSTRUCTION_ACTION_NBCD\
	/* TODO */\
	UNIMPLEMENTED_INSTRUCTION("NBCD")

#define DO_INSTRUCTION_ACTION_SWAP\
	closure.result_value = ((closure.destination_value & 0x0000FFFF) << 16) | ((closure.destination_value & 0xFFFF0000) >> 16)

#define DO_INSTRUCTION_ACTION_PEA\
	state->address_registers[7] -= 4;\
	WriteLongWord(&closure.stuff, state->address_registers[7], closure.source_value)

#define DO_INSTRUCTION_ACTION_ILLEGAL\
	/* Illegal instruction. */\
	Group1Or2Exception(&closure.stuff, 4)

#define DO_INSTRUCTION_ACTION_TAS\
	/* TODO - This instruction doesn't work properly on memory on the Mega Drive */\
	state->status_register &= ~(CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO);\
	state->status_register |= CONDITION_CODE_NEGATIVE & (0 - ((closure.destination_value & 0x80) != 0));\
	state->status_register |= CONDITION_CODE_ZERO & (0 - (closure.destination_value == 0));\
\
	closure.result_value = closure.destination_value | 0x80

#define DO_INSTRUCTION_ACTION_TRAP\
	closure.source_value = opcode.raw & 0xF;\
	Group1Or2Exception(&closure.stuff, 32 + closure.source_value)

#define DO_INSTRUCTION_ACTION_MOVE_USP\
	if ((opcode.raw & 8) != 0)\
		state->address_registers[opcode.primary_register] = state->user_stack_pointer;\
	else\
		state->user_stack_pointer = state->address_registers[opcode.primary_register]

#define DO_INSTRUCTION_ACTION_RESET\
	/* TODO */\
	UNIMPLEMENTED_INSTRUCTION("RESET")

#define DO_INSTRUCTION_ACTION_STOP\
	/* TODO */\
	UNIMPLEMENTED_INSTRUCTION("STOP")

#define DO_INSTRUCTION_ACTION_RTE\
	{\
	const cc_u16f new_status = ReadWord(&closure.stuff, state->address_registers[7]) & STATUS_REGISTER_MASK;\
\
	state->status_register = new_status;\
	state->address_registers[7] += 2;\
	state->program_counter = ReadLongWord(&closure.stuff, state->address_registers[7]);\
	state->address_registers[7] += 4;\
\
	/* Restore the previous supervisor bit so we can toggle properly. */\
	/* TODO: Maybe redesign SetSupervisorMode so that it isn't so clunky to use here. */\
	state->status_register |= STATUS_SUPERVISOR;\
	SetSupervisorMode(closure.stuff.state, (new_status & STATUS_SUPERVISOR) != 0);\
	}

#define DO_INSTRUCTION_ACTION_RTS\
	state->program_counter = ReadLongWord(&closure.stuff, state->address_registers[7]);\
	state->address_registers[7] += 4

#define DO_INSTRUCTION_ACTION_TRAPV\
	if ((state->status_register & CONDITION_CODE_OVERFLOW) != 0)\
		Group1Or2Exception(&closure.stuff, 7)

#define DO_INSTRUCTION_ACTION_RTR\
	state->status_register &= ~CONDITION_CODE_REGISTER_MASK;\
	state->status_register |= ReadByte(&closure.stuff, state->address_registers[7] + 1) & CONDITION_CODE_REGISTER_MASK;\
	state->address_registers[7] += 2;\
	state->program_counter = ReadLongWord(&closure.stuff, state->address_registers[7]);\
	state->address_registers[7] += 4;

#define DO_INSTRUCTION_ACTION_JSR\
	state->address_registers[7] -= 4;\
	WriteLongWord(&closure.stuff, state->address_registers[7], state->program_counter);\
	DO_INSTRUCTION_ACTION_JMP

#define DO_INSTRUCTION_ACTION_JMP\
	state->program_counter = closure.source_value

#define DO_INSTRUCTION_ACTION_MOVEM\
	{\
	/* Hot damn is this a mess */\
	cc_u32f memory_address = closure.destination_value; /* TODO: Maybe get rid of this alias? */\
	cc_u16f i;\
	cc_u16f bitfield;\
	\
	int delta;\
	void (*write_function)(Stuff *stuff, cc_u32f address, cc_u32f value);\
	\
	if ((opcode.raw & 0x0040) != 0)\
	{\
		delta = 4;\
		write_function = WriteLongWord;\
	}\
	else\
	{\
		delta = 2;\
		write_function = WriteWord;\
	}\
	\
	if (opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)\
		delta = -delta;\
	\
	bitfield = closure.source_value;\
	\
	/* First group of registers */\
	for (i = 0; i < 8; ++i)\
	{\
		if ((bitfield & 1) != 0)\
		{\
			if ((opcode.raw & 0x0400) != 0)\
			{\
				/* Memory to register */\
				if ((opcode.raw & 0x0040) != 0)\
					state->data_registers[i] = ReadLongWord(&closure.stuff, memory_address);\
				else\
					state->data_registers[i] = CC_SIGN_EXTEND_ULONG(15, ReadWord(&closure.stuff, memory_address));\
			}\
			else\
			{\
				/* Register to memory */\
				if (opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)\
					write_function(&closure.stuff, memory_address + delta, state->address_registers[7 - i]);\
				else\
					write_function(&closure.stuff, memory_address, state->data_registers[i]);\
			}\
	\
			memory_address += delta;\
		}\
	\
		bitfield >>= 1;\
	}\
	\
	/* Second group of registers */\
	for (i = 0; i < 8; ++i)\
	{\
		if ((bitfield & 1) != 0)\
		{\
			if ((opcode.raw & 0x0400) != 0)\
			{\
				/* Memory to register */\
				if ((opcode.raw & 0x0040) != 0)\
					state->address_registers[i] = ReadLongWord(&closure.stuff, memory_address);\
				else\
					state->address_registers[i] = CC_SIGN_EXTEND_ULONG(15, ReadWord(&closure.stuff, memory_address));\
			}\
			else\
			{\
				/* Register to memory */\
				if (opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)\
					write_function(&closure.stuff, memory_address + delta, state->data_registers[7 - i]);\
				else\
					write_function(&closure.stuff, memory_address, state->address_registers[i]);\
			}\
	\
			memory_address += delta;\
		}\
	\
		bitfield >>= 1;\
	}\
	\
	if (opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT || opcode.primary_address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT)\
		state->address_registers[opcode.primary_register] = memory_address;\
	}

#define DO_INSTRUCTION_ACTION_CHK\
	{\
	const cc_u32f value = state->data_registers[opcode.secondary_register] & 0xFFFF;\
	\
	if ((value & 0x8000) != 0)\
	{\
		/* Value is smaller than 0. */\
		state->status_register |= CONDITION_CODE_NEGATIVE;\
		Group1Or2Exception(&closure.stuff, 6);\
	}\
	else\
	{\
		const cc_u32f delta = value - closure.source_value;\
\
		if ((delta & 0x8000) == 0 && delta != 0)\
		{\
			/* Value is greater than upper bound. */\
			state->status_register &= ~CONDITION_CODE_NEGATIVE;\
			Group1Or2Exception(&closure.stuff, 6);\
		}\
	}\
	}

#define DO_INSTRUCTION_ACTION_SCC\
	closure.result_value = IsOpcodeConditionTrue(state, opcode.raw) ? 0xFF : 0

#define DO_INSTRUCTION_ACTION_DBCC\
	if (!IsOpcodeConditionTrue(state, opcode.raw))\
	{\
		cc_u16f loop_counter = state->data_registers[opcode.primary_register] & 0xFFFF;\
	\
		if (loop_counter-- != 0)\
		{\
			state->program_counter -= 2;\
			state->program_counter += CC_SIGN_EXTEND_ULONG(15, closure.source_value);\
		}\
	\
		state->data_registers[opcode.primary_register] &= ~0xFFFFul;\
		state->data_registers[opcode.primary_register] |= loop_counter & 0xFFFF;\
	}

#define DO_INSTRUCTION_ACTION_BRA_SHORT\
	state->program_counter += CC_SIGN_EXTEND_ULONG(7, opcode.raw)

#define DO_INSTRUCTION_ACTION_BRA_WORD\
	state->program_counter -= 2;\
	state->program_counter += CC_SIGN_EXTEND_ULONG(15, closure.source_value)

#define DO_INSTRUCTION_ACTION_BSR(SUB_ACTION)\
	state->address_registers[7] -= 4;\
	WriteLongWord(&closure.stuff, state->address_registers[7], state->program_counter);\
	SUB_ACTION

#define DO_INSTRUCTION_ACTION_BSR_SHORT\
	DO_INSTRUCTION_ACTION_BSR(DO_INSTRUCTION_ACTION_BRA_SHORT)	

#define DO_INSTRUCTION_ACTION_BSR_WORD\
	DO_INSTRUCTION_ACTION_BSR(DO_INSTRUCTION_ACTION_BRA_WORD)	

#define DO_INSTRUCTION_ACTION_BCC(SUB_ACTION)\
	if (IsOpcodeConditionTrue(state, opcode.raw))\
	{\
		SUB_ACTION;\
	}

#define DO_INSTRUCTION_ACTION_BCC_SHORT\
	DO_INSTRUCTION_ACTION_BCC(DO_INSTRUCTION_ACTION_BRA_SHORT)	

#define DO_INSTRUCTION_ACTION_BCC_WORD\
	DO_INSTRUCTION_ACTION_BCC(DO_INSTRUCTION_ACTION_BRA_WORD)	

#define DO_INSTRUCTION_ACTION_MOVEQ\
	closure.result_value = CC_SIGN_EXTEND_ULONG(7, opcode.raw)

#define DO_INSTRUCTION_ACTION_DIVS\
	if (closure.source_value == 0)\
	{\
		Group1Or2Exception(&closure.stuff, 5);\
		longjmp(closure.stuff.exception.context, 1);\
	}\
	else\
	{\
		const cc_bool source_is_negative = (closure.source_value & 0x8000) != 0;\
		const cc_bool destination_is_negative = (closure.destination_value & 0x80000000) != 0;\
		const cc_bool result_is_negative = source_is_negative != destination_is_negative;\
\
		const cc_u32f absolute_source_value = source_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(15, closure.source_value) : closure.source_value;\
		const cc_u32f absolute_destination_value = destination_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(31, closure.destination_value) : closure.destination_value;\
\
		const cc_u32f absolute_quotient = absolute_destination_value / absolute_source_value;\
		const cc_u32f quotient = result_is_negative ? 0 - absolute_quotient : absolute_quotient;\
\
		/* Overflow detection */\
		if (absolute_quotient > (result_is_negative ? (cc_u32f)0x8000 : (cc_u32f)0x7FFF))\
		{\
			state->status_register |= CONDITION_CODE_OVERFLOW;\
\
			closure.result_value = closure.destination_value;\
		}\
		else\
		{\
			const cc_u32f absolute_remainder = absolute_destination_value % absolute_source_value;\
			const cc_u32f remainder = destination_is_negative ? 0 - absolute_remainder : absolute_remainder;\
\
			closure.result_value = (quotient & 0xFFFF) | ((remainder & 0xFFFF) << 16);\
\
			state->status_register &= ~(CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO | CONDITION_CODE_OVERFLOW);\
			state->status_register |= CONDITION_CODE_NEGATIVE & (0 - ((quotient & 0x8000) != 0));\
			state->status_register |= CONDITION_CODE_ZERO & (0 - (quotient == 0));\
		}\
	}

#define DO_INSTRUCTION_ACTION_DIVU\
	if (closure.source_value == 0)\
	{\
		Group1Or2Exception(&closure.stuff, 5);\
		longjmp(closure.stuff.exception.context, 1);\
	}\
	else\
	{\
		const cc_u32f quotient = closure.destination_value / closure.source_value;\
\
		/* Overflow detection */\
		if (quotient > (cc_u32f)0xFFFF)\
		{\
			state->status_register |= CONDITION_CODE_OVERFLOW;\
\
			closure.result_value = closure.destination_value;\
		}\
		else\
		{\
			const cc_u32f remainder = closure.destination_value % closure.source_value;\
\
			closure.result_value = (quotient & 0xFFFF) | ((remainder & 0xFFFF) << 16);\
\
			state->status_register &= ~(CONDITION_CODE_NEGATIVE | CONDITION_CODE_ZERO | CONDITION_CODE_OVERFLOW);\
			state->status_register |= CONDITION_CODE_NEGATIVE & (0 - ((quotient & 0x8000) != 0));\
			state->status_register |= CONDITION_CODE_ZERO & (0 - (quotient == 0));\
		}\
	}

#define DO_INSTRUCTION_ACTION_SBCD\
	/* TODO */\
	UNIMPLEMENTED_INSTRUCTION("SBCD")

#define DO_INSTRUCTION_ACTION_SUBX\
	closure.result_value = closure.destination_value - closure.source_value - ((state->status_register & CONDITION_CODE_EXTEND) != 0 ? 1 : 0)

#define DO_INSTRUCTION_ACTION_MULS\
	{\
	const cc_bool multiplier_is_negative = (closure.source_value & 0x8000) != 0;\
	const cc_bool multiplicand_is_negative = (closure.destination_value & 0x8000) != 0;\
	const cc_bool result_is_negative = multiplier_is_negative != multiplicand_is_negative;\
\
	const cc_u32f multiplier = multiplier_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(15, closure.source_value) : closure.source_value;\
	const cc_u32f multiplicand = multiplicand_is_negative ? 0 - CC_SIGN_EXTEND_ULONG(15, closure.destination_value) : closure.destination_value & 0xFFFF;\
\
	const cc_u32f absolute_result = multiplicand * multiplier;\
\
	closure.result_value = result_is_negative ? 0 - absolute_result : absolute_result;\
	}

#define DO_INSTRUCTION_ACTION_MULU\
	closure.result_value = (closure.destination_value & 0xFFFF) * closure.source_value;\

#define DO_INSTRUCTION_ACTION_ABCD\
	/* TODO */\
	UNIMPLEMENTED_INSTRUCTION("ABCD")

#define DO_INSTRUCTION_ACTION_EXG\
	{\
	cc_u32f temp;\
\
	switch (opcode.raw & 0x00F8)\
	{\
		/* TODO: What should happen when an invalid bit pattern occurs? */\
		case 0x0040:\
			temp = state->data_registers[opcode.secondary_register];\
			state->data_registers[opcode.secondary_register] = state->data_registers[opcode.primary_register];\
			state->data_registers[opcode.primary_register] = temp;\
			break;\
\
		case 0x0048:\
			temp = state->address_registers[opcode.secondary_register];\
			state->address_registers[opcode.secondary_register] = state->address_registers[opcode.primary_register];\
			state->address_registers[opcode.primary_register] = temp;\
			break;\
\
		case 0x0088:\
			temp = state->data_registers[opcode.secondary_register];\
			state->data_registers[opcode.secondary_register] = state->address_registers[opcode.primary_register];\
			state->address_registers[opcode.primary_register] = temp;\
			break;\
	}\
	}

#define DO_INSTRUCTION_ACTION_ADDX\
	closure.result_value = closure.destination_value + closure.source_value + ((state->status_register & CONDITION_CODE_EXTEND) != 0 ? 1 : 0)

#define DO_INSTRUCTION_ACTION_SHIFT_1_ASD\
	const unsigned long original_sign_bit = closure.destination_value & sign_bit_bitmask;

#define DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD

#define DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY\
	count = 1;

#define DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER\
	count = (opcode.raw & 0x0020) != 0 ? state->data_registers[opcode.secondary_register] % 64 : ((opcode.secondary_register - 1u) & 7u) + 1u; /* A little math trick to turn 0 into 8 */

#define DO_INSTRUCTION_ACTION_SHIFT_3_ASD\
	closure.result_value <<= 1;\
	state->status_register |= CONDITION_CODE_OVERFLOW & (0 - ((closure.result_value & sign_bit_bitmask) != original_sign_bit));

#define DO_INSTRUCTION_ACTION_SHIFT_3_LSD\
	closure.result_value <<= 1;

#define DO_INSTRUCTION_ACTION_SHIFT_3_ROXD\
	closure.result_value <<= 1;\
	closure.result_value |= (state->status_register & CONDITION_CODE_EXTEND) != 0;

#define DO_INSTRUCTION_ACTION_SHIFT_3_ROD\
	closure.result_value = (closure.result_value << 1) | ((closure.result_value & sign_bit_bitmask) != 0);

#define DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD\
	state->status_register &= ~CONDITION_CODE_EXTEND;\
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

#define DO_INSTRUCTION_ACTION_SHIFT_4_ROD

#define DO_INSTRUCTION_ACTION_SHIFT_5_ASD\
	closure.result_value >>= 1;\
	closure.result_value |= original_sign_bit;

#define DO_INSTRUCTION_ACTION_SHIFT_5_LSD\
	closure.result_value >>= 1;

#define DO_INSTRUCTION_ACTION_SHIFT_5_ROXD\
	closure.result_value >>= 1;\
	closure.result_value |= sign_bit_bitmask & (0 - ((state->status_register & CONDITION_CODE_EXTEND) != 0));

#define DO_INSTRUCTION_ACTION_SHIFT_5_ROD\
	closure.result_value = (closure.result_value >> 1) | (sign_bit_bitmask & (0 - ((closure.result_value & 1) != 0)));

#define DO_INSTRUCTION_ACTION_SHIFT_6_ROXD\
	state->status_register |= CONDITION_CODE_CARRY & (0 - ((state->status_register & CONDITION_CODE_EXTEND) != 0));

#define DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD

#define DO_INSTRUCTION_ACTION_SHIFT(SUB_ACTION_1, SUB_ACTION_2, SUB_ACTION_3, SUB_ACTION_4, SUB_ACTION_5, SUB_ACTION_6)\
	{\
	const cc_u32f sign_bit_bitmask = 1ul << (operation_size * 8 - 1);\
\
	SUB_ACTION_1\
\
	cc_u16f i;\
	cc_u16f count;\
\
	closure.result_value = closure.destination_value;\
\
	SUB_ACTION_2;\
\
	state->status_register &= ~(CONDITION_CODE_OVERFLOW | CONDITION_CODE_CARRY);\
\
	SUB_ACTION_6;\
\
	if (opcode.bit_8)\
	{\
		/* Left */\
		for (i = 0; i < count; ++i)\
		{\
			state->status_register &= ~CONDITION_CODE_CARRY;\
			state->status_register |= CONDITION_CODE_CARRY & (0 - ((closure.result_value & sign_bit_bitmask) != 0));\
\
			SUB_ACTION_3;\
\
			SUB_ACTION_4;\
		}\
	}\
	else\
	{\
		/* Right */\
		for (i = 0; i < count; ++i)\
		{\
			state->status_register &= ~CONDITION_CODE_CARRY;\
			state->status_register |= CONDITION_CODE_CARRY & (0 - ((closure.result_value & 1) != 0));\
\
			SUB_ACTION_5;\
\
			SUB_ACTION_4;\
		}\
	}\
	}

#define DO_INSTRUCTION_ACTION_ASD_MEMORY\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_ASD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ASD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)

#define DO_INSTRUCTION_ACTION_ASD_REGISTER\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_ASD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ASD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)

#define DO_INSTRUCTION_ACTION_LSD_MEMORY\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_LSD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_LSD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)

#define DO_INSTRUCTION_ACTION_LSD_REGISTER\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_LSD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_LSD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)

#define DO_INSTRUCTION_ACTION_ROD_MEMORY\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_ROD, DO_INSTRUCTION_ACTION_SHIFT_4_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)

#define DO_INSTRUCTION_ACTION_ROD_REGISTER\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_ROD, DO_INSTRUCTION_ACTION_SHIFT_4_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROD, DO_INSTRUCTION_ACTION_SHIFT_6_NOT_ROXD)

#define DO_INSTRUCTION_ACTION_ROXD_MEMORY\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_MEMORY, DO_INSTRUCTION_ACTION_SHIFT_3_ROXD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROXD, DO_INSTRUCTION_ACTION_SHIFT_6_ROXD)

#define DO_INSTRUCTION_ACTION_ROXD_REGISTER\
	DO_INSTRUCTION_ACTION_SHIFT(DO_INSTRUCTION_ACTION_SHIFT_1_NOT_ASD, DO_INSTRUCTION_ACTION_SHIFT_2_REGISTER, DO_INSTRUCTION_ACTION_SHIFT_3_ROXD, DO_INSTRUCTION_ACTION_SHIFT_4_NOT_ROD, DO_INSTRUCTION_ACTION_SHIFT_5_ROXD, DO_INSTRUCTION_ACTION_SHIFT_6_ROXD)

#define DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1\
	Group1Or2Exception(&closure.stuff, 10)

#define DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2\
	Group1Or2Exception(&closure.stuff, 11)

#define DO_INSTRUCTION_ACTION_NOP\
	/* Doesn't do anything */

#endif


#endif /* INSTRUCTION_ACTIONS_H */
