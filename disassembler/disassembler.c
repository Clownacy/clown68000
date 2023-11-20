#include "disassembler.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../m68k/opcode.h"

typedef struct Stuff
{
	unsigned long address;
	Clown68000_Disassemble_ReadCallback read_callback;
	void *user_data;
} Stuff;

static long ReadWord(Stuff* const stuff)
{
	stuff->address += 2;
	return stuff->read_callback(stuff->user_data);
}

static size_t UnsignedHexToString(char* const buffer, const unsigned long hex)
{
	static const char lut[0x10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	size_t index, subindex;
	unsigned int i;

	index = 0;

	if (hex >= 0xA)
		buffer[index++] = '$';

	subindex = 0;

	for (i = 0; i < 8; ++i)
	{
		const char character = lut[(hex >> (4 * (8 - 1 - i))) & 0xF];

		if (character != '0' || subindex != 0 || i == 7)
			buffer[index + subindex++] = character;
	}

	index += subindex;

	return index;
}

static size_t SignedHexToString(char* const buffer, const unsigned long hex)
{
	if ((hex & 0x80000000) != 0)
	{
		buffer[0] = '-';
		return 1 + UnsignedHexToString(&buffer[1], 0 - hex);
	}
	else
	{
		return UnsignedHexToString(buffer, hex);
	}
}

static const char* GetInstructionName(const Instruction instruction)
{
	switch (instruction)
	{
		case INSTRUCTION_ABCD:
			return "abcd";

		case INSTRUCTION_ADD:
			return "add";

		case INSTRUCTION_ADDA:
			return "adda";

		case INSTRUCTION_ADDAQ:
			return "addq";

		case INSTRUCTION_ADDI:
			return "addi";

		case INSTRUCTION_ADDQ:
			return "addq";

		case INSTRUCTION_ADDX:
			return "addx";

		case INSTRUCTION_AND:
			return "and";

		case INSTRUCTION_ANDI:
			return "andi";

		case INSTRUCTION_ANDI_TO_CCR:
			return "andi";

		case INSTRUCTION_ANDI_TO_SR:
			return "andi";

		case INSTRUCTION_ASD_MEMORY:
			return "as";

		case INSTRUCTION_ASD_REGISTER:
			return "as";

		case INSTRUCTION_BCC_SHORT:
		case INSTRUCTION_BCC_WORD:
			return "b";

		case INSTRUCTION_BCHG_DYNAMIC:
			return "bchg";

		case INSTRUCTION_BCHG_STATIC:
			return "bchg";

		case INSTRUCTION_BCLR_DYNAMIC:
			return "bclr";

		case INSTRUCTION_BCLR_STATIC:
			return "bclr";

		case INSTRUCTION_BRA_SHORT:
			return "bra";

		case INSTRUCTION_BRA_WORD:
			return "bra";

		case INSTRUCTION_BSET_DYNAMIC:
			return "bset";

		case INSTRUCTION_BSET_STATIC:
			return "bset";

		case INSTRUCTION_BSR_SHORT:
			return "bsr";

		case INSTRUCTION_BSR_WORD:
			return "bsr";

		case INSTRUCTION_BTST_DYNAMIC:
			return "btst";

		case INSTRUCTION_BTST_STATIC:
			return "btst";

		case INSTRUCTION_CHK:
			return "chk";

		case INSTRUCTION_CLR:
			return "clr";

		case INSTRUCTION_CMP:
			return "cmp";

		case INSTRUCTION_CMPA:
			return "cmpa";

		case INSTRUCTION_CMPI:
			return "cmpi";

		case INSTRUCTION_CMPM:
			return "cmpm";

		case INSTRUCTION_DBCC:
			return "db";

		case INSTRUCTION_DIVS:
			return "divs";

		case INSTRUCTION_DIVU:
			return "divu";

		case INSTRUCTION_EOR:
			return "eor";

		case INSTRUCTION_EORI:
			return "eori";

		case INSTRUCTION_EORI_TO_CCR:
			return "eori";

		case INSTRUCTION_EORI_TO_SR:
			return "eori";

		case INSTRUCTION_EXG:
			return "exg";

		case INSTRUCTION_EXT:
			return "ext";

		case INSTRUCTION_ILLEGAL:
			return "illegal";

		case INSTRUCTION_JMP:
			return "jmp";

		case INSTRUCTION_JSR:
			return "jsr";

		case INSTRUCTION_LEA:
			return "lea";

		case INSTRUCTION_LINK:
			return "link";

		case INSTRUCTION_LSD_MEMORY:
			return "ls";

		case INSTRUCTION_LSD_REGISTER:
			return "ls";

		case INSTRUCTION_MOVE:
			return "move";

		case INSTRUCTION_MOVE_FROM_SR:
			return "move";

		case INSTRUCTION_MOVE_TO_CCR:
			return "move";

		case INSTRUCTION_MOVE_TO_SR:
			return "move";

		case INSTRUCTION_MOVE_USP:
			return "move";

		case INSTRUCTION_MOVEA:
			return "movea";

		case INSTRUCTION_MOVEM:
			return "movem";

		case INSTRUCTION_MOVEP:
			return "movep";

		case INSTRUCTION_MOVEQ:
			return "moveq";

		case INSTRUCTION_MULS:
			return "muls";

		case INSTRUCTION_MULU:
			return "mulu";

		case INSTRUCTION_NBCD:
			return "nbcd";

		case INSTRUCTION_NEG:
			return "neg";

		case INSTRUCTION_NEGX:
			return "negx";

		case INSTRUCTION_NOP:
			return "nop";

		case INSTRUCTION_NOT:
			return "not";

		case INSTRUCTION_OR:
			return "or";

		case INSTRUCTION_ORI:
			return "ori";

		case INSTRUCTION_ORI_TO_CCR:
			return "ori";

		case INSTRUCTION_ORI_TO_SR:
			return "ori";

		case INSTRUCTION_PEA:
			return "pea";

		case INSTRUCTION_RESET:
			return "reset";

		case INSTRUCTION_ROD_MEMORY:
			return "ro";

		case INSTRUCTION_ROD_REGISTER:
			return "ro";

		case INSTRUCTION_ROXD_MEMORY:
			return "rox";

		case INSTRUCTION_ROXD_REGISTER:
			return "rox";

		case INSTRUCTION_RTE:
			return "rte";

		case INSTRUCTION_RTR:
			return "rtr";

		case INSTRUCTION_RTS:
			return "rts";

		case INSTRUCTION_SBCD:
			return "sbcd";

		case INSTRUCTION_SCC:
			return "s";

		case INSTRUCTION_STOP:
			return "stop";

		case INSTRUCTION_SUB:
			return "sub";

		case INSTRUCTION_SUBA:
			return "suba";

		case INSTRUCTION_SUBAQ:
			return "subq";

		case INSTRUCTION_SUBI:
			return "subi";

		case INSTRUCTION_SUBQ:
			return "subq";

		case INSTRUCTION_SUBX:
			return "subx";

		case INSTRUCTION_SWAP:
			return "swap";

		case INSTRUCTION_TAS:
			return "tas";

		case INSTRUCTION_TRAP:
			return "trap";

		case INSTRUCTION_TRAPV:
			return "trapv";

		case INSTRUCTION_TST:
			return "tst";

		case INSTRUCTION_UNLK:
			return "unlk";

		case INSTRUCTION_UNIMPLEMENTED_1:
			return "[UNIMPLEMENTED1]";

		case INSTRUCTION_UNIMPLEMENTED_2:
			return "[UNIMPLEMENTED2]";
	}

	assert(cc_false);

	return "[ERROR]";
}

static size_t GetOperandName(Stuff* const stuff, char* const buffer, const DecodedOpcode* const decoded_opcode, const cc_bool operand_is_destination)
{
	size_t index = 0;
	const Operand* const operand = &decoded_opcode->operands[operand_is_destination ? 1 : 0];

	switch (operand->address_mode)
	{
		case ADDRESS_MODE_DATA_REGISTER:
		case ADDRESS_MODE_ADDRESS_REGISTER:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT:
		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT:
			if (operand->address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT)
				buffer[index++] = '-';

			switch (operand->address_mode)
			{
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT:
					buffer[index++] = '(';
					break;

				default:
					break;
			}

			switch (operand->address_mode)
			{
				case ADDRESS_MODE_DATA_REGISTER:
					buffer[index++] = 'd';
					break;

				case ADDRESS_MODE_ADDRESS_REGISTER:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT:
					buffer[index++] = 'a';
					break;

				default:
					break;
			}

			buffer[index++] = '0' + operand->address_mode_register;

			switch (operand->address_mode)
			{
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT:
					buffer[index++] = ')';
					break;

				default:
					break;
			}

			if (operand->address_mode == ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT)
				buffer[index++] = '+';

			break;

		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
		{
			const unsigned long data = ReadWord(stuff);
			index += SignedHexToString(&buffer[index], CC_SIGN_EXTEND_ULONG(15, data));
			buffer[index++] = '(';
			buffer[index++] = 'a';
			buffer[index++] = '0' + operand->address_mode_register;
			buffer[index++] = ')';
			break;
		}

		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_INDEX:
		{
			const unsigned long data = ReadWord(stuff);
			index += SignedHexToString(&buffer[index], CC_SIGN_EXTEND_ULONG(7, data));
			buffer[index++] = '(';
			buffer[index++] = 'a';
			buffer[index++] = '0' + operand->address_mode_register;
			buffer[index++] = ',';
			buffer[index++] = (data & 0x8000) != 0 ? 'a' : 'd';
			buffer[index++] = '0' + ((data >> 12) & 7);
			buffer[index++] = '.';
			buffer[index++] = (data & 0x800) != 0 ? 'l' : 'w';
			buffer[index++] = ')';
			break;
		}

		case ADDRESS_MODE_SPECIAL:
		{
			const unsigned long address = stuff->address;
			const unsigned long data = ReadWord(stuff);

			switch (operand->address_mode_register)
			{
				case ADDRESS_MODE_REGISTER_SPECIAL_ABSOLUTE_SHORT:
					buffer[index++] = '(';
					index += UnsignedHexToString(&buffer[index], CC_SIGN_EXTEND_ULONG(15, data));
					buffer[index++] = ')';
					buffer[index++] = '.';
					buffer[index++] = 'w';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_ABSOLUTE_LONG:
					buffer[index++] = '(';
					index += UnsignedHexToString(&buffer[index], (data << 16) | ReadWord(stuff));
					buffer[index++] = ')';
					buffer[index++] = '.';
					buffer[index++] = 'l';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_DISPLACEMENT:
					index += UnsignedHexToString(&buffer[index], address + CC_SIGN_EXTEND_ULONG(15, data));
					buffer[index++] = '(';
					buffer[index++] = 'p';
					buffer[index++] = 'c';
					buffer[index++] = ')';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_INDEX:
					index += UnsignedHexToString(&buffer[index], address + CC_SIGN_EXTEND_ULONG(7, data));
					buffer[index++] = '(';
					buffer[index++] = 'p';
					buffer[index++] = 'c';
					buffer[index++] = ',';
					buffer[index++] = (data & 0x8000) != 0 ? 'a' : 'd';
					buffer[index++] = '0' + ((data >> 12) & 7);
					buffer[index++] = '.';
					buffer[index++] = (data & 0x800) != 0 ? 'l' : 'w';
					buffer[index++] = ')';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE:
					buffer[index++] = '#';

					if (operand->operation_size == OPERATION_SIZE_LONGWORD)
						index += SignedHexToString(&buffer[index], (data << 16) | ReadWord(stuff));
					else
						index += SignedHexToString(&buffer[index], CC_SIGN_EXTEND_ULONG(15, data));

					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE_ADDRESS:
					if (operand->operation_size == OPERATION_SIZE_LONGWORD)
						index += UnsignedHexToString(&buffer[index], address + ((data << 16) | ReadWord(stuff)));
					else
						index += UnsignedHexToString(&buffer[index], address + CC_SIGN_EXTEND_ULONG(15, data));

					break;
			}

			break;
		}

		case ADDRESS_MODE_STATUS_REGISTER:
			buffer[index++] = 's';
			buffer[index++] = 'p';
			break;

		case ADDRESS_MODE_CONDITION_CODE_REGISTER:
			buffer[index++] = 'c';
			buffer[index++] = 'c';
			buffer[index++] = 'r';
			break;

		case ADDRESS_MODE_NONE:
			assert(0);
			break;

		case ADDRESS_MODE_EMBEDDED_IMMEDIATE:
			buffer[index++] = '#';
			index += SignedHexToString(&buffer[index], CC_SIGN_EXTEND_ULONG(operand->operation_size == OPERATION_SIZE_BYTE ? 7 : operand->operation_size == OPERATION_SIZE_WORD ? 15 : 31, operand->address_mode_register));
			break;

		case ADDRESS_MODE_EMBEDDED_IMMEDIATE_ADDRESS:
			index += UnsignedHexToString(&buffer[index], stuff->address + CC_SIGN_EXTEND_ULONG(operand->operation_size == OPERATION_SIZE_BYTE ? 7 : operand->operation_size == OPERATION_SIZE_WORD ? 15 : 31, operand->address_mode_register));
			break;
	}

	return index;
}

static const char* GetOpcodeConditionName(const unsigned int opcode)
{
	switch ((opcode >> 8) & 0xF)
	{
		case 0x0:
			/* True */
			return "t";

		case 0x1:
			/* False */
			return "f";

		case 0x2:
			/* Higher */
			return "hi";

		case 0x3:
			/* Lower or same */
			return "ls";

		case 0x4:
			/* Carry clear */
			return "cc";

		case 0x5:
			/* Carry set */
			return "cs";

		case 0x6:
			/* Not equal */
			return "ne";

		case 0x7:
			/* Equal */
			return "eq";

		case 0x8:
			/* Overflow clear */
			return "vc";

		case 0x9:
			/* Overflow set */
			return "vs";

		case 0xA:
			/* Plus */
			return "pl";

		case 0xB:
			/* Minus */
			return "mi";

		case 0xC:
			/* Greater or equal */
			return "ge";

		case 0xD:
			/* Less than */
			return "lt";

		case 0xE:
			/* Greater than */
			return "gt";

		case 0xF:
			/* Less or equal */
			return "le";
	}

	assert(cc_false);

	return "[ERROR]";
}

void Clown68000_Disassemble(const unsigned long address, const Clown68000_Disassemble_ReadCallback read_callback, const Clown68000_Disassemble_PrintCallback print_callback, const void* const user_data)
{
	Stuff stuff;
	stuff.address = address;
	stuff.read_callback = read_callback;
	stuff.user_data = (void*)user_data;

	for (;;)
	{
		char buff_buffer_owo[0x100];
		size_t index;
		DecodedOpcode decoded_opcode;
		SplitOpcode split_opcode;

		sprintf(buff_buffer_owo, "%08lX: ", stuff.address);

		const long opcode = ReadWord(&stuff);

		if (opcode == -1)
			return;

		DecodeOpcode(&decoded_opcode, &split_opcode, opcode);

		strcat(buff_buffer_owo, GetInstructionName(decoded_opcode.instruction));

		/* Instruction name suffix and special operands. */
		switch (decoded_opcode.instruction)
		{
			case INSTRUCTION_BCC_SHORT:
			case INSTRUCTION_BCC_WORD:
			case INSTRUCTION_DBCC:
			case INSTRUCTION_SCC:
				strcat(buff_buffer_owo, GetOpcodeConditionName(opcode));
				break;

			case INSTRUCTION_ASD_MEMORY:
			case INSTRUCTION_ASD_REGISTER:
			case INSTRUCTION_LSD_MEMORY:
			case INSTRUCTION_LSD_REGISTER:
			case INSTRUCTION_ROD_MEMORY:
			case INSTRUCTION_ROD_REGISTER:
			case INSTRUCTION_ROXD_MEMORY:
			case INSTRUCTION_ROXD_REGISTER:
				strcat(buff_buffer_owo, split_opcode.bit_8 ? "l" : "r");
				break;

			default:
				break;
		}

		index = strlen(buff_buffer_owo);

		switch (decoded_opcode.instruction)
		{
			case INSTRUCTION_DBCC:
			case INSTRUCTION_LEA:
			case INSTRUCTION_MOVEQ:
			case INSTRUCTION_PEA:
			case INSTRUCTION_EXG:
			case INSTRUCTION_SWAP:
			case INSTRUCTION_LINK:
			case INSTRUCTION_STOP:
				break;

			default:
				if (decoded_opcode.size != OPERATION_SIZE_NONE)
				{
					buff_buffer_owo[index++] = '.';
					buff_buffer_owo[index++] = decoded_opcode.size == OPERATION_SIZE_SHORT ? 's' : decoded_opcode.size == OPERATION_SIZE_BYTE ? 'b' : decoded_opcode.size == OPERATION_SIZE_WORD ? 'w' : 'l';
				}

				break;
		}

		while (index != 18)
			buff_buffer_owo[index++] = ' ';

		switch (decoded_opcode.instruction)
		{
			default:
				if (decoded_opcode.operands[0].address_mode != ADDRESS_MODE_NONE)
					index += GetOperandName(&stuff, &buff_buffer_owo[index], &decoded_opcode, cc_false);

				if (decoded_opcode.operands[0].address_mode != ADDRESS_MODE_NONE && decoded_opcode.operands[1].address_mode != ADDRESS_MODE_NONE)
					buff_buffer_owo[index++] = ',';

				if (decoded_opcode.operands[1].address_mode != ADDRESS_MODE_NONE)
					index += GetOperandName(&stuff, &buff_buffer_owo[index], &decoded_opcode, cc_true);

				break;
		}

		buff_buffer_owo[index++] = '\0';

		print_callback((void*)user_data, buff_buffer_owo);

		/* Halt when encountering a terminating instruction. */
		switch (decoded_opcode.instruction)
		{
			case INSTRUCTION_BRA_SHORT:
			case INSTRUCTION_BRA_WORD:
			case INSTRUCTION_JMP:
			case INSTRUCTION_RESET:
			case INSTRUCTION_RTE:
			case INSTRUCTION_RTR:
			case INSTRUCTION_RTS:
			case INSTRUCTION_STOP:
			case INSTRUCTION_TRAP:
			case INSTRUCTION_TRAPV:
				return;

			case INSTRUCTION_ABCD:
			case INSTRUCTION_ADD:
			case INSTRUCTION_ADDA:
			case INSTRUCTION_ADDAQ:
			case INSTRUCTION_ADDI:
			case INSTRUCTION_ADDQ:
			case INSTRUCTION_ADDX:
			case INSTRUCTION_AND:
			case INSTRUCTION_ANDI:
			case INSTRUCTION_ANDI_TO_CCR:
			case INSTRUCTION_ANDI_TO_SR:
			case INSTRUCTION_ASD_MEMORY:
			case INSTRUCTION_ASD_REGISTER:
			case INSTRUCTION_BCC_SHORT:
			case INSTRUCTION_BCC_WORD:
			case INSTRUCTION_BCHG_DYNAMIC:
			case INSTRUCTION_BCHG_STATIC:
			case INSTRUCTION_BCLR_DYNAMIC:
			case INSTRUCTION_BCLR_STATIC:
			case INSTRUCTION_BSET_DYNAMIC:
			case INSTRUCTION_BSET_STATIC:
			case INSTRUCTION_BSR_SHORT:
			case INSTRUCTION_BSR_WORD:
			case INSTRUCTION_BTST_DYNAMIC:
			case INSTRUCTION_BTST_STATIC:
			case INSTRUCTION_CHK:
			case INSTRUCTION_CLR:
			case INSTRUCTION_CMP:
			case INSTRUCTION_CMPA:
			case INSTRUCTION_CMPI:
			case INSTRUCTION_CMPM:
			case INSTRUCTION_DBCC:
			case INSTRUCTION_DIVS:
			case INSTRUCTION_DIVU:
			case INSTRUCTION_EOR:
			case INSTRUCTION_EORI:
			case INSTRUCTION_EORI_TO_CCR:
			case INSTRUCTION_EORI_TO_SR:
			case INSTRUCTION_EXG:
			case INSTRUCTION_EXT:
			case INSTRUCTION_ILLEGAL:
			case INSTRUCTION_JSR:
			case INSTRUCTION_LEA:
			case INSTRUCTION_LINK:
			case INSTRUCTION_LSD_MEMORY:
			case INSTRUCTION_LSD_REGISTER:
			case INSTRUCTION_MOVE:
			case INSTRUCTION_MOVE_FROM_SR:
			case INSTRUCTION_MOVE_TO_CCR:
			case INSTRUCTION_MOVE_TO_SR:
			case INSTRUCTION_MOVE_USP:
			case INSTRUCTION_MOVEA:
			case INSTRUCTION_MOVEM:
			case INSTRUCTION_MOVEP:
			case INSTRUCTION_MOVEQ:
			case INSTRUCTION_MULS:
			case INSTRUCTION_MULU:
			case INSTRUCTION_NBCD:
			case INSTRUCTION_NEG:
			case INSTRUCTION_NEGX:
			case INSTRUCTION_NOP:
			case INSTRUCTION_NOT:
			case INSTRUCTION_OR:
			case INSTRUCTION_ORI:
			case INSTRUCTION_ORI_TO_CCR:
			case INSTRUCTION_ORI_TO_SR:
			case INSTRUCTION_PEA:
			case INSTRUCTION_ROD_MEMORY:
			case INSTRUCTION_ROD_REGISTER:
			case INSTRUCTION_ROXD_MEMORY:
			case INSTRUCTION_ROXD_REGISTER:
			case INSTRUCTION_SBCD:
			case INSTRUCTION_SCC:
			case INSTRUCTION_SUB:
			case INSTRUCTION_SUBA:
			case INSTRUCTION_SUBAQ:
			case INSTRUCTION_SUBI:
			case INSTRUCTION_SUBQ:
			case INSTRUCTION_SUBX:
			case INSTRUCTION_SWAP:
			case INSTRUCTION_TAS:
			case INSTRUCTION_TST:
			case INSTRUCTION_UNLK:
			case INSTRUCTION_UNIMPLEMENTED_1:
			case INSTRUCTION_UNIMPLEMENTED_2:
				break;
		}
	}
}
