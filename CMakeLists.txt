cmake_minimum_required(VERSION 3.0...3.12)

option(CC_USE_C99_INTEGERS "Use C99 integer types instead of the original C89 ones. May save RAM depending on the platform's data model." OFF)

project(clown68000 LANGUAGES C)

# Common

add_library(clown68000-common STATIC
	"common/clowncommon/clowncommon.h"
	"common/instruction.h"
	"common/opcode.c"
	"common/opcode.h"	
)

if(CC_USE_C99_INTEGERS)
	target_compile_definitions(clown68000-common PUBLIC CC_USE_C99_INTEGERS)
endif()

# Interpreter

add_library(clown68000-interpreter STATIC
	"interpreter/clown68000.c"
	"interpreter/clown68000.h"
)

target_link_libraries(clown68000-interpreter PRIVATE clown68000-common) 

if(CC_USE_C99_INTEGERS)
	target_compile_definitions(clown68000-interpreter PUBLIC CC_USE_C99_INTEGERS)
endif()

# Microcode Generator

add_executable(clown68000-microcode-generator
	"interpreter/microcode-generator/emit.c"
	"interpreter/microcode-generator/emit.h"
	"interpreter/microcode-generator/emit-instructions.c"
	"interpreter/microcode-generator/emit-instructions.h"
	"interpreter/microcode-generator/instruction-properties.c"
	"interpreter/microcode-generator/instruction-properties.h"
	"interpreter/microcode-generator/generator.c"
)

if(CC_USE_C99_INTEGERS)
	target_compile_definitions(clown68000-microcode-generator PUBLIC CC_USE_C99_INTEGERS)
endif()

# Disassembler

add_library(clown68000-disassembler STATIC
	"disassembler/disassembler.c"
	"disassembler/disassembler.h"
	"disassembler/opcode.c"
	"disassembler/opcode.h"
)

target_link_libraries(clown68000-disassembler PRIVATE clown68000-common) 

if(CC_USE_C99_INTEGERS)
	target_compile_definitions(clown68000-disassembler PUBLIC CC_USE_C99_INTEGERS)
endif()

# Disassembler Tool

add_executable(clown68000-disassembler-tool
	"disassembler/main.c"
)

target_link_libraries(clown68000-disassembler-tool PRIVATE clown68000-disassembler) 

if(CC_USE_C99_INTEGERS)
	target_compile_definitions(clown68000-disassembler-tool PUBLIC CC_USE_C99_INTEGERS)
endif()
