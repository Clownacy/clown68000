cmake_minimum_required(VERSION 3.0)

project(clown68000 LANGUAGES C)

add_library(clown68000 STATIC
	"clowncommon/clowncommon.h"
	"m68k.c"
	"m68k.h"
	"m68k/opcode.c"
	"m68k/opcode.h"
)

set_target_properties(clown68000 PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED OFF
	C_EXTENSIONS OFF
)
