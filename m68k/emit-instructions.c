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
		Emit("	Group1Or2Exception(&closure.stuff, 8);");
		Emit("	longjmp(closure.stuff.exception.context, 1);");
		Emit("}");
		Emit("");
	}
}

void EmitInstructionSourceAddressMode(const Instruction instruction)
{
	if (Instruction_IsSourceOperandRead(instruction))
	{
		Emit("/* Decode source address mode. */");
		Emit("DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);");
		Emit("");
	}
}

void EmitInstructionDestinationAddressMode(void)
{
	Emit("/* Decode destination address mode. */");
	Emit("DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);");
	Emit("");
}

void EmitInstructionReadSourceOperand(const Instruction instruction)
{
	if (Instruction_IsSourceOperandRead(instruction))
	{
		Emit("/* Read source operand. */");
		Emit("closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);");
		Emit("");
	}
}

void EmitInstructionReadDestinationOperand(const Instruction instruction)
{
	if (Instruction_IsDestinationOperandRead(instruction))
	{
		Emit("/* Read destination operand. */");
		Emit("closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);");
		Emit("");
	}
}
