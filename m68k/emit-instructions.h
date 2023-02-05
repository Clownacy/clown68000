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

#ifndef M68K_EMIT_INSTRUCTIONS_H
#define M68K_EMIT_INSTRUCTIONS_H

#include "instruction.h"

void EmitInstructionSupervisorCheck(const Instruction instruction);
void EmitInstructionSourceAddressMode(const Instruction instruction);
void EmitInstructionDestinationAddressMode(void);
void EmitInstructionReadSourceOperand(const Instruction instruction);
void EmitInstructionReadDestinationOperand(const Instruction instruction);
void EmitInstructionAction(const Instruction instruction);

#endif /* M68K_EMIT_INSTRUCTIONS_H */
