#include "disassembler.h"

#include <assert.h>
#include <string.h>

#include "../m68k/opcode.h"

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
			return "ABCD";

		case INSTRUCTION_ADD:
			return "ADD";

		case INSTRUCTION_ADDA:
			return "ADDA";

		case INSTRUCTION_ADDAQ:
			return "ADDQ";

		case INSTRUCTION_ADDI:
			return "ADDI";

		case INSTRUCTION_ADDQ:
			return "ADDQ";

		case INSTRUCTION_ADDX:
			return "ADDX";

		case INSTRUCTION_AND:
			return "AND";

		case INSTRUCTION_ANDI:
			return "ANDI";

		case INSTRUCTION_ANDI_TO_CCR:
			return "ANDI";

		case INSTRUCTION_ANDI_TO_SR:
			return "ANDI";

		case INSTRUCTION_ASD_MEMORY:
			return "ASd";

		case INSTRUCTION_ASD_REGISTER:
			return "ASd";

		case INSTRUCTION_BCC_SHORT:
			return "Bcc";

		case INSTRUCTION_BCC_WORD:
			return "Bcc";

		case INSTRUCTION_BCHG_DYNAMIC:
			return "BCHG";

		case INSTRUCTION_BCHG_STATIC:
			return "BCHG";

		case INSTRUCTION_BCLR_DYNAMIC:
			return "BCLR";

		case INSTRUCTION_BCLR_STATIC:
			return "BCLR";

		case INSTRUCTION_BRA_SHORT:
			return "BRA";

		case INSTRUCTION_BRA_WORD:
			return "BRA";

		case INSTRUCTION_BSET_DYNAMIC:
			return "BSET";

		case INSTRUCTION_BSET_STATIC:
			return "BSET";

		case INSTRUCTION_BSR_SHORT:
			return "BSR";

		case INSTRUCTION_BSR_WORD:
			return "BSR";

		case INSTRUCTION_BTST_DYNAMIC:
			return "BTST";

		case INSTRUCTION_BTST_STATIC:
			return "BTST";

		case INSTRUCTION_CHK:
			return "CHK";

		case INSTRUCTION_CLR:
			return "CLR";

		case INSTRUCTION_CMP:
			return "CMP";

		case INSTRUCTION_CMPA:
			return "CMPA";

		case INSTRUCTION_CMPI:
			return "CMPI";

		case INSTRUCTION_CMPM:
			return "CMPM";

		case INSTRUCTION_DBCC:
			return "DBcc";

		case INSTRUCTION_DIVS:
			return "DIVS";

		case INSTRUCTION_DIVU:
			return "DIVU";

		case INSTRUCTION_EOR:
			return "EOR";

		case INSTRUCTION_EORI:
			return "EORI";

		case INSTRUCTION_EORI_TO_CCR:
			return "EORI";

		case INSTRUCTION_EORI_TO_SR:
			return "EORI";

		case INSTRUCTION_EXG:
			return "EXG";

		case INSTRUCTION_EXT:
			return "EXT";

		case INSTRUCTION_ILLEGAL:
			return "ILLEGAL";

		case INSTRUCTION_JMP:
			return "JMP";

		case INSTRUCTION_JSR:
			return "JSR";

		case INSTRUCTION_LEA:
			return "LEA";

		case INSTRUCTION_LINK:
			return "LINK";

		case INSTRUCTION_LSD_MEMORY:
			return "LSd";

		case INSTRUCTION_LSD_REGISTER:
			return "LSd";

		case INSTRUCTION_MOVE:
			return "MOVE";

		case INSTRUCTION_MOVE_FROM_SR:
			return "MOVE";

		case INSTRUCTION_MOVE_TO_CCR:
			return "MOVE";

		case INSTRUCTION_MOVE_TO_SR:
			return "MOVE";

		case INSTRUCTION_MOVE_USP:
			return "MOVE";

		case INSTRUCTION_MOVEA:
			return "MOVEA";

		case INSTRUCTION_MOVEM:
			return "MOVEM";

		case INSTRUCTION_MOVEP:
			return "MOVEP";

		case INSTRUCTION_MOVEQ:
			return "MOVEQ";

		case INSTRUCTION_MULS:
			return "MULS";

		case INSTRUCTION_MULU:
			return "MULU";

		case INSTRUCTION_NBCD:
			return "NBCD";

		case INSTRUCTION_NEG:
			return "NEG";

		case INSTRUCTION_NEGX:
			return "NEGX";

		case INSTRUCTION_NOP:
			return "NOP";

		case INSTRUCTION_NOT:
			return "NOT";

		case INSTRUCTION_OR:
			return "OR";

		case INSTRUCTION_ORI:
			return "ORI";

		case INSTRUCTION_ORI_TO_CCR:
			return "ORI";

		case INSTRUCTION_ORI_TO_SR:
			return "ORI";

		case INSTRUCTION_PEA:
			return "PEA";

		case INSTRUCTION_RESET:
			return "RESET";

		case INSTRUCTION_ROD_MEMORY:
			return "ROd";

		case INSTRUCTION_ROD_REGISTER:
			return "ROd";

		case INSTRUCTION_ROXD_MEMORY:
			return "ROXd";

		case INSTRUCTION_ROXD_REGISTER:
			return "ROXd";

		case INSTRUCTION_RTE:
			return "RTE";

		case INSTRUCTION_RTR:
			return "RTR";

		case INSTRUCTION_RTS:
			return "RTS";

		case INSTRUCTION_SBCD:
			return "SBCD";

		case INSTRUCTION_SCC:
			return "SCC";

		case INSTRUCTION_STOP:
			return "STOP";

		case INSTRUCTION_SUB:
			return "SUB";

		case INSTRUCTION_SUBA:
			return "SUBA";

		case INSTRUCTION_SUBAQ:
			return "SUBAQ";

		case INSTRUCTION_SUBI:
			return "SUBI";

		case INSTRUCTION_SUBQ:
			return "SUBQ";

		case INSTRUCTION_SUBX:
			return "SUBX";

		case INSTRUCTION_SWAP:
			return "SWAP";

		case INSTRUCTION_TAS:
			return "TAS";

		case INSTRUCTION_TRAP:
			return "TRAP";

		case INSTRUCTION_TRAPV:
			return "TRAPV";

		case INSTRUCTION_TST:
			return "TST";

		case INSTRUCTION_UNLK:
			return "UNLK";

		case INSTRUCTION_UNIMPLEMENTED_1:
			return "[UNIMPLEMENTED1]";

		case INSTRUCTION_UNIMPLEMENTED_2:
			return "[UNIMPLEMENTED2]";
	}

	return "[ERROR]";
}

static size_t GetOperandName(char* const buffer, const DecodedOpcode* const decoded_opcode, const cc_bool operand_is_destination, const Clown68000_Disassemble_ReadCallback read_callback, const void* const user_data)
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
					buffer[index++] = 'D';
					break;

				case ADDRESS_MODE_ADDRESS_REGISTER:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_POSTINCREMENT:
				case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_PREDECREMENT:
					buffer[index++] = 'A';
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
			const unsigned long data = read_callback((void*)user_data);
			index += SignedHexToString(&buffer[index], (data & 0x7FFF) - (data & 0x8000));
			buffer[index++] = '(';
			buffer[index++] = 'A';
			buffer[index++] = '0' + operand->address_mode_register;
			buffer[index++] = ')';
			break;
		}

		case ADDRESS_MODE_ADDRESS_REGISTER_INDIRECT_WITH_INDEX:
		{
			const unsigned long data = read_callback((void*)user_data);
			index += SignedHexToString(&buffer[index], (data & 0x7F) - (data & 0x80));
			buffer[index++] = '(';
			buffer[index++] = 'A';
			buffer[index++] = '0' + operand->address_mode_register;
			buffer[index++] = ',';
			buffer[index++] = (data & 0x8000) != 0 ? 'A' : 'D';
			buffer[index++] = '0' + ((data >> 12) & 7);
			buffer[index++] = '.';
			buffer[index++] = (data & 0x800) != 0 ? 'L' : 'W';
			buffer[index++] = ')';
			break;
		}

		case ADDRESS_MODE_SPECIAL:
		{
			const unsigned long data = read_callback((void*)user_data);

			switch (operand->address_mode_register)
			{
				case ADDRESS_MODE_REGISTER_SPECIAL_ABSOLUTE_SHORT:
					buffer[index++] = '(';
					index += UnsignedHexToString(&buffer[index], (data & 0x7FFF) - (data & 0x8000));
					buffer[index++] = ')';
					buffer[index++] = '.';
					buffer[index++] = 'w';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_ABSOLUTE_LONG:
					buffer[index++] = '(';
					index += UnsignedHexToString(&buffer[index], (data << 16) | read_callback((void*)user_data));
					buffer[index++] = ')';
					buffer[index++] = '.';
					buffer[index++] = 'l';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_DISPLACEMENT:
					index += SignedHexToString(&buffer[index], (data & 0x7FFF) - (data & 0x8000));
					buffer[index++] = '(';
					buffer[index++] = 'P';
					buffer[index++] = 'C';
					buffer[index++] = ')';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_PROGRAM_COUNTER_WITH_INDEX:
					index += SignedHexToString(&buffer[index], (data & 0x7F) - (data & 0x80));
					buffer[index++] = '(';
					buffer[index++] = 'P';
					buffer[index++] = 'C';
					buffer[index++] = ',';
					buffer[index++] = (data & 0x8000) != 0 ? 'A' : 'D';
					buffer[index++] = '0' + ((data >> 12) & 7);
					buffer[index++] = '.';
					buffer[index++] = (data & 0x800) != 0 ? 'L' : 'W';
					buffer[index++] = ')';
					break;

				case ADDRESS_MODE_REGISTER_SPECIAL_IMMEDIATE:
					buffer[index++] = '#';

					if (operand->operation_size_in_bytes == 4)
						index += SignedHexToString(&buffer[index], (data << 16) | read_callback((void*)user_data));
					else
						index += SignedHexToString(&buffer[index], (data & 0x7FFF) - (data & 0x8000));

					break;
			}

			break;
		}

		case ADDRESS_MODE_STATUS_REGISTER:
			buffer[index++] = 'S';
			buffer[index++] = 'P';
			break;

		case ADDRESS_MODE_CONDITION_CODE_REGISTER:
			buffer[index++] = 'C';
			buffer[index++] = 'C';
			buffer[index++] = 'R';
			break;

		case ADDRESS_MODE_NONE:
			assert(0);
			break;
	}

	return index;
}

void Clown68000_Disassemble(const Clown68000_Disassemble_ReadCallback read_callback, const Clown68000_Disassemble_PrintCallback print_callback, const void* const user_data)
{
	for (;;)
	{
		char buff_buffer_owo[0x100];
		size_t index;
		DecodedOpcode decoded_opcode;
		SplitOpcode split_opcode;

		const long opcode = read_callback((void*)user_data);

		if (opcode == -1)
			return;

		DecodeOpcode(&decoded_opcode, &split_opcode, opcode);

		if (decoded_opcode.instruction == INSTRUCTION_DBCC)
		{
			decoded_opcode.operands[1] = decoded_opcode.operands[0];
			decoded_opcode.operands[0].address_mode = ADDRESS_MODE_DATA_REGISTER;
			decoded_opcode.operands[0].address_mode_register = split_opcode.primary_register;
		}

		strcpy(buff_buffer_owo, GetInstructionName(decoded_opcode.instruction));

		index = strlen(buff_buffer_owo);

		if (decoded_opcode.operands[1].address_mode != ADDRESS_MODE_NONE)
		{
			buff_buffer_owo[index++] = '.';
			buff_buffer_owo[index++] = decoded_opcode.operands[1].operation_size_in_bytes == 1 ? 'B' : decoded_opcode.operands[1].operation_size_in_bytes == 2 ? 'W' : 'L';
		}
		else if (decoded_opcode.operands[0].address_mode != ADDRESS_MODE_NONE)
		{
			buff_buffer_owo[index++] = '.';
			buff_buffer_owo[index++] = decoded_opcode.operands[0].operation_size_in_bytes == 1 ? 'B' : decoded_opcode.operands[0].operation_size_in_bytes == 2 ? 'W' : 'L';
		}

		while (index != 10)
			buff_buffer_owo[index++] = ' ';

		if (decoded_opcode.instruction == INSTRUCTION_MOVEQ)
		{
			buff_buffer_owo[index++] = '#';
			index += SignedHexToString(&buff_buffer_owo[index], (opcode & 0x7F) - (opcode & 0x80));
			buff_buffer_owo[index++] = ',';
			index += GetOperandName(&buff_buffer_owo[index], &decoded_opcode, cc_true, read_callback, user_data);
		}
		else
		{
			if (decoded_opcode.operands[0].address_mode != ADDRESS_MODE_NONE)
				index += GetOperandName(&buff_buffer_owo[index], &decoded_opcode, cc_false, read_callback, user_data);

			if (decoded_opcode.operands[0].address_mode != ADDRESS_MODE_NONE && decoded_opcode.operands[1].address_mode != ADDRESS_MODE_NONE)
				buff_buffer_owo[index++] = ',';

			if (decoded_opcode.operands[1].address_mode != ADDRESS_MODE_NONE)
				index += GetOperandName(&buff_buffer_owo[index], &decoded_opcode, cc_true, read_callback, user_data);
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
