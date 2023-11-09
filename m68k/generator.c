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

#include <stdio.h>
#include <stdlib.h>

#include "emit.h"
#include "emit-instructions.h"

int main(int argc, char **argv)
{
	static const char* const instruction_strings[] = {
		"ABCD",
		"ADD",
		"ADDA",
		"ADDAQ",
		"ADDI",
		"ADDQ",
		"ADDX",
		"AND",
		"ANDI",
		"ANDI_TO_CCR",
		"ANDI_TO_SR",
		"ASD_MEMORY",
		"ASD_REGISTER",
		"BCC_SHORT",
		"BCC_WORD",
		"BCHG_DYNAMIC",
		"BCHG_STATIC",
		"BCLR_DYNAMIC",
		"BCLR_STATIC",
		"BRA_SHORT",
		"BRA_WORD",
		"BSET_DYNAMIC",
		"BSET_STATIC",
		"BSR_SHORT",
		"BSR_WORD",
		"BTST_DYNAMIC",
		"BTST_STATIC",
		"CHK",
		"CLR",
		"CMP",
		"CMPA",
		"CMPI",
		"CMPM",
		"DBCC",
		"DIVS",
		"DIVU",
		"EOR",
		"EORI",
		"EORI_TO_CCR",
		"EORI_TO_SR",
		"EXG",
		"EXT",
		"ILLEGAL",
		"JMP",
		"JSR",
		"LEA",
		"LINK",
		"LSD_MEMORY",
		"LSD_REGISTER",
		"MOVE",
		"MOVE_FROM_SR",
		"MOVE_TO_CCR",
		"MOVE_TO_SR",
		"MOVE_USP",
		"MOVEA",
		"MOVEM",
		"MOVEP",
		"MOVEQ",
		"MULS",
		"MULU",
		"NBCD",
		"NEG",
		"NEGX",
		"NOP",
		"NOT",
		"OR",
		"ORI",
		"ORI_TO_CCR",
		"ORI_TO_SR",
		"PEA",
		"RESET",
		"ROD_MEMORY",
		"ROD_REGISTER",
		"ROXD_MEMORY",
		"ROXD_REGISTER",
		"RTE",
		"RTR",
		"RTS",
		"SBCD",
		"SCC",
		"STOP",
		"SUB",
		"SUBA",
		"SUBAQ",
		"SUBI",
		"SUBQ",
		"SUBX",
		"SWAP",
		"TAS",
		"TRAP",
		"TRAPV",
		"TST",
		"UNLK",

		"UNIMPLEMENTED_1",
		"UNIMPLEMENTED_2"
	};

	size_t i;

	(void)argc;
	(void)argv;

	emit_file = stdout;

	for (i = 0; i < CC_COUNT_OF(instruction_strings); ++i)
	{
		const Instruction instruction = (Instruction)i;

		EmitFormatted("static void (* const microcode_%s[])(Stuff* const stuff) = {\n\t", instruction_strings[i]);
		EmitInstructionSupervisorCheck(instruction);
		EmitInstructionSize(instruction);
		EmitInstructionSourceAddressMode(instruction);
		EmitInstructionReadSourceOperand(instruction);
		EmitInstructionDestinationAddressMode(instruction);
		EmitInstructionReadDestinationOperand(instruction);
		EmitInstructionAction(instruction);
		EmitInstructionWriteDestinationOperand(instruction);
		EmitInstructionConditionCodes(instruction);
		Emit("NULL\n};\n\n");
	}

	return EXIT_SUCCESS;
}
