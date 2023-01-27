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

#ifndef M68K_H
#define M68K_H

#include "clowncommon/clowncommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct M68k_State
{
	cc_u32l data_registers[8];
	cc_u32l address_registers[8];
	cc_u32l supervisor_stack_pointer;
	cc_u32l user_stack_pointer;
	cc_u32l program_counter;
	cc_u16l status_register;
	cc_u16l instruction_register;
	cc_bool halted;

	cc_u8l cycles_left_in_instruction;
} M68k_State;

typedef struct M68k_ReadWriteCallbacks
{
	cc_u16f (*read_callback)(const void *user_data, cc_u32f address, cc_bool do_high_byte, cc_bool do_low_byte);
	void (*write_callback)(const void *user_data, cc_u32f address, cc_bool do_high_byte, cc_bool do_low_byte, cc_u16f value);
	const void *user_data;
} M68k_ReadWriteCallbacks;

void M68k_Reset(M68k_State *state, const M68k_ReadWriteCallbacks *callbacks);
void M68k_Interrupt(M68k_State *state, const M68k_ReadWriteCallbacks *callbacks, cc_u16f level);
void M68k_DoCycle(M68k_State *state, const M68k_ReadWriteCallbacks *callbacks);

#ifdef __cplusplus
}
#endif

#endif /* M68K_H */
