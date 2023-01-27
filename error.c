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

#include "error.h"

#include <stdarg.h>
#include <stddef.h>

static void (*error_callback)(const char *format, va_list arg);

void SetErrorCallback(void (*error_callback_)(const char *format, va_list arg))
{
	/* TODO - Shouldn't this use the regular state instead of global state? */
	error_callback = error_callback_;
}

void PrintError(const char *format, ...)
{
	if (error_callback != NULL)
	{
		va_list args;
		va_start(args, format);
		error_callback(format, args);
		va_end(args);
	}
}
