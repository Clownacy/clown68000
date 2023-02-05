case INSTRUCTION_ABCD:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ABCD;

	break;

case INSTRUCTION_ADD:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADD;

	break;

case INSTRUCTION_ADDA:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDA;

	break;

case INSTRUCTION_ADDAQ:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDQ;

	break;

case INSTRUCTION_ADDI:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADD;

	break;

case INSTRUCTION_ADDQ:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDQ;

	break;

case INSTRUCTION_ADDX:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDX;

	break;

case INSTRUCTION_AND:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	break;

case INSTRUCTION_ANDI:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	break;

case INSTRUCTION_ANDI_TO_CCR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	break;

case INSTRUCTION_ANDI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	break;

case INSTRUCTION_ASD_MEMORY:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ASD_MEMORY;

	break;

case INSTRUCTION_ASD_REGISTER:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ASD_REGISTER;

	break;

case INSTRUCTION_BCC_SHORT:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCC_SHORT;

	break;

case INSTRUCTION_BCC_WORD:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCC_WORD;

	break;

case INSTRUCTION_BCHG_DYNAMIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCHG;

	break;

case INSTRUCTION_BCHG_STATIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCHG;

	break;

case INSTRUCTION_BCLR_DYNAMIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCLR;

	break;

case INSTRUCTION_BCLR_STATIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCLR;

	break;

case INSTRUCTION_BRA_SHORT:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BRA_SHORT;

	break;

case INSTRUCTION_BRA_WORD:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BRA_WORD;

	break;

case INSTRUCTION_BSET_DYNAMIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSET;

	break;

case INSTRUCTION_BSET_STATIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSET;

	break;

case INSTRUCTION_BSR_SHORT:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSR_SHORT;

	break;

case INSTRUCTION_BSR_WORD:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSR_WORD;

	break;

case INSTRUCTION_BTST_DYNAMIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BTST;

	break;

case INSTRUCTION_BTST_STATIC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BTST;

	break;

case INSTRUCTION_CHK:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_CHK;

	break;

case INSTRUCTION_CLR:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_CLR;

	break;

case INSTRUCTION_CMP:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	break;

case INSTRUCTION_CMPA:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBA;

	break;

case INSTRUCTION_CMPI:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	break;

case INSTRUCTION_CMPM:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	break;

case INSTRUCTION_DBCC:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_DBCC;

	break;

case INSTRUCTION_DIVS:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_DIVS;

	break;

case INSTRUCTION_DIVU:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_DIVU;

	break;

case INSTRUCTION_EOR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	break;

case INSTRUCTION_EORI:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	break;

case INSTRUCTION_EORI_TO_CCR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	break;

case INSTRUCTION_EORI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	break;

case INSTRUCTION_EXG:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EXG;

	break;

case INSTRUCTION_EXT:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EXT;

	break;

case INSTRUCTION_ILLEGAL:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ILLEGAL;

	break;

case INSTRUCTION_JMP:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_JMP;

	break;

case INSTRUCTION_JSR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_JSR;

	break;

case INSTRUCTION_LEA:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	break;

case INSTRUCTION_LINK:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_LINK;

	break;

case INSTRUCTION_LSD_MEMORY:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_LSD_MEMORY;

	break;

case INSTRUCTION_LSD_REGISTER:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_LSD_REGISTER;

	break;

case INSTRUCTION_MOVE:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	break;

case INSTRUCTION_MOVE_FROM_SR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	break;

case INSTRUCTION_MOVE_TO_CCR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	break;

case INSTRUCTION_MOVE_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	break;

case INSTRUCTION_MOVE_USP:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE_USP;

	break;

case INSTRUCTION_MOVEA:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEA;

	break;

case INSTRUCTION_MOVEM:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEM;

	break;

case INSTRUCTION_MOVEP:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEP;

	break;

case INSTRUCTION_MOVEQ:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEQ;

	break;

case INSTRUCTION_MULS:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MULS;

	break;

case INSTRUCTION_MULU:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MULU;

	break;

case INSTRUCTION_NBCD:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NBCD;

	break;

case INSTRUCTION_NEG:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NEG;

	break;

case INSTRUCTION_NEGX:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NEGX;

	break;

case INSTRUCTION_NOP:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NOP;

	break;

case INSTRUCTION_NOT:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NOT;

	break;

case INSTRUCTION_OR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	break;

case INSTRUCTION_ORI:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	break;

case INSTRUCTION_ORI_TO_CCR:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	break;

case INSTRUCTION_ORI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	break;

case INSTRUCTION_PEA:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_PEA;

	break;

case INSTRUCTION_RESET:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RESET;

	break;

case INSTRUCTION_ROD_MEMORY:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROD_MEMORY;

	break;

case INSTRUCTION_ROD_REGISTER:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROD_REGISTER;

	break;

case INSTRUCTION_ROXD_MEMORY:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROXD_MEMORY;

	break;

case INSTRUCTION_ROXD_REGISTER:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROXD_REGISTER;

	break;

case INSTRUCTION_RTE:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RTE;

	break;

case INSTRUCTION_RTR:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RTR;

	break;

case INSTRUCTION_RTS:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RTS;

	break;

case INSTRUCTION_SBCD:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SBCD;

	break;

case INSTRUCTION_SCC:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SCC;

	break;

case INSTRUCTION_STOP:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_STOP;

	break;

case INSTRUCTION_SUB:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	break;

case INSTRUCTION_SUBA:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBA;

	break;

case INSTRUCTION_SUBAQ:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBQ;

	break;

case INSTRUCTION_SUBI:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	break;

case INSTRUCTION_SUBQ:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBQ;

	break;

case INSTRUCTION_SUBX:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBX;

	break;

case INSTRUCTION_SWAP:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SWAP;

	break;

case INSTRUCTION_TAS:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Read destination operand. */
	closure.destination_value = GetValueUsingDecodedAddressMode(&closure.stuff, &closure.destination_decoded_address_mode);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_TAS;

	break;

case INSTRUCTION_TRAP:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_TRAP;

	break;

case INSTRUCTION_TRAPV:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_TRAPV;

	break;

case INSTRUCTION_TST:
	/* Decode source address mode. */
	DecodeAddressMode(&closure.stuff, &source_decoded_address_mode, &closure.decoded_opcode.operands[0]);

	/* Read source operand. */
	closure.source_value = GetValueUsingDecodedAddressMode(&closure.stuff, &source_decoded_address_mode);

	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	break;

case INSTRUCTION_UNLK:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_UNLK;

	break;

case INSTRUCTION_UNIMPLEMENTED_1:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1;

	break;

case INSTRUCTION_UNIMPLEMENTED_2:
	/* Decode destination address mode. */
	DecodeAddressMode(&closure.stuff, &closure.destination_decoded_address_mode, &closure.decoded_opcode.operands[1]);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2;

	break;

