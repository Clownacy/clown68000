cmake_minimum_required(VERSION 3.0...3.5)

project(disassembler LANGUAGES C)

add_library(disassembler STATIC
	"disassembler.c"
	"disassembler.h"
	"opcode.c"
	"opcode.h"
	"../m68k/opcode.c"
	"../m68k/opcode.h"
)

set_target_properties(disassembler PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED NO
	C_EXTENSIONS OFF
)

add_executable(disassembler-tool
	"main.c"
)

target_link_libraries(disassembler-tool PRIVATE disassembler) 

set_target_properties(disassembler-tool PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED NO
	C_EXTENSIONS OFF
)
