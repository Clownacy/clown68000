cmake_minimum_required(VERSION 3.0...3.5)

project(clown68000_disassembler LANGUAGES C)

add_executable(clown68000_disassembler
	"main.c"
	"disassembler.c"
	"disassembler.h"
	"../m68k/opcode.c"
	"../m68k/opcode.h"
)
