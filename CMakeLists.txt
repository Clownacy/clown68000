cmake_minimum_required(VERSION 3.0)

option(CC_USE_C99_INTEGERS "Use C99 integer types instead of the original C89 ones. May save RAM depending on the platform's data model." OFF)

project(clown68000 LANGUAGES C)

add_library(clown68000 STATIC
	"clowncommon/clowncommon.h"
	"clown68000.c"
	"clown68000.h"
	"m68k/opcode.c"
	"m68k/opcode.h"
)

set_target_properties(clown68000 PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED OFF
	C_EXTENSIONS OFF
)

if(CC_USE_C99_INTEGERS)
	target_compile_definitions(clown68000 PUBLIC CC_USE_C99_INTEGERS)
endif()
