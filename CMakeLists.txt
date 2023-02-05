cmake_minimum_required(VERSION 3.0)

project(clown68000 LANGUAGES C)

add_library(clown68000 STATIC
	"clowncommon/clowncommon.h"
	"clown68000.c"
	"clown68000.h"
	"m68k/instruction-properties.c"
	"m68k/instruction-properties.h"
	"m68k/opcode.c"
	"m68k/opcode.h"
)

set_target_properties(clown68000 PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED OFF
	C_EXTENSIONS OFF
)
