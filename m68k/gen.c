case INSTRUCTION_ABCD:
	break;

case INSTRUCTION_ADD:
	break;

case INSTRUCTION_ADDA:
	break;

case INSTRUCTION_ADDAQ:
	break;

case INSTRUCTION_ADDI:
	break;

case INSTRUCTION_ADDQ:
	break;

case INSTRUCTION_ADDX:
	break;

case INSTRUCTION_AND:
	break;

case INSTRUCTION_ANDI:
	break;

case INSTRUCTION_ANDI_TO_CCR:
	break;

case INSTRUCTION_ANDI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_ASD_MEMORY:
	break;

case INSTRUCTION_ASD_REGISTER:
	break;

case INSTRUCTION_BCC_SHORT:
	break;

case INSTRUCTION_BCC_WORD:
	break;

case INSTRUCTION_BCHG_DYNAMIC:
	break;

case INSTRUCTION_BCHG_STATIC:
	break;

case INSTRUCTION_BCLR_DYNAMIC:
	break;

case INSTRUCTION_BCLR_STATIC:
	break;

case INSTRUCTION_BRA_SHORT:
	break;

case INSTRUCTION_BRA_WORD:
	break;

case INSTRUCTION_BSET_DYNAMIC:
	break;

case INSTRUCTION_BSET_STATIC:
	break;

case INSTRUCTION_BSR_SHORT:
	break;

case INSTRUCTION_BSR_WORD:
	break;

case INSTRUCTION_BTST_DYNAMIC:
	break;

case INSTRUCTION_BTST_STATIC:
	break;

case INSTRUCTION_CHK:
	break;

case INSTRUCTION_CLR:
	break;

case INSTRUCTION_CMP:
	break;

case INSTRUCTION_CMPA:
	break;

case INSTRUCTION_CMPI:
	break;

case INSTRUCTION_CMPM:
	break;

case INSTRUCTION_DBCC:
	break;

case INSTRUCTION_DIVS:
	break;

case INSTRUCTION_DIVU:
	break;

case INSTRUCTION_EOR:
	break;

case INSTRUCTION_EORI:
	break;

case INSTRUCTION_EORI_TO_CCR:
	break;

case INSTRUCTION_EORI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_EXG:
	break;

case INSTRUCTION_EXT:
	break;

case INSTRUCTION_ILLEGAL:
	break;

case INSTRUCTION_JMP:
	break;

case INSTRUCTION_JSR:
	break;

case INSTRUCTION_LEA:
	break;

case INSTRUCTION_LINK:
	break;

case INSTRUCTION_LSD_MEMORY:
	break;

case INSTRUCTION_LSD_REGISTER:
	break;

case INSTRUCTION_MOVE:
	break;

case INSTRUCTION_MOVE_FROM_SR:
	break;

case INSTRUCTION_MOVE_TO_CCR:
	break;

case INSTRUCTION_MOVE_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_MOVE_USP:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_MOVEA:
	break;

case INSTRUCTION_MOVEM:
	break;

case INSTRUCTION_MOVEP:
	break;

case INSTRUCTION_MOVEQ:
	break;

case INSTRUCTION_MULS:
	break;

case INSTRUCTION_MULU:
	break;

case INSTRUCTION_NBCD:
	break;

case INSTRUCTION_NEG:
	break;

case INSTRUCTION_NEGX:
	break;

case INSTRUCTION_NOP:
	break;

case INSTRUCTION_NOT:
	break;

case INSTRUCTION_OR:
	break;

case INSTRUCTION_ORI:
	break;

case INSTRUCTION_ORI_TO_CCR:
	break;

case INSTRUCTION_ORI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_PEA:
	break;

case INSTRUCTION_RESET:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_ROD_MEMORY:
	break;

case INSTRUCTION_ROD_REGISTER:
	break;

case INSTRUCTION_ROXD_MEMORY:
	break;

case INSTRUCTION_ROXD_REGISTER:
	break;

case INSTRUCTION_RTE:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_RTR:
	break;

case INSTRUCTION_RTS:
	break;

case INSTRUCTION_SBCD:
	break;

case INSTRUCTION_SCC:
	break;

case INSTRUCTION_STOP:
	/* Only allow this instruction in supervisor mode. */
	if ((state->status_register & STATUS_SUPERVISOR) == 0)
	{
		Group1Or2Exception(&closure.stuff, 8);
		longjmp(closure.stuff.exception.context, 1);
	}

	break;

case INSTRUCTION_SUB:
	break;

case INSTRUCTION_SUBA:
	break;

case INSTRUCTION_SUBAQ:
	break;

case INSTRUCTION_SUBI:
	break;

case INSTRUCTION_SUBQ:
	break;

case INSTRUCTION_SUBX:
	break;

case INSTRUCTION_SWAP:
	break;

case INSTRUCTION_TAS:
	break;

case INSTRUCTION_TRAP:
	break;

case INSTRUCTION_TRAPV:
	break;

case INSTRUCTION_TST:
	break;

case INSTRUCTION_UNLK:
	break;

case INSTRUCTION_UNIMPLEMENTED_1:
	break;

case INSTRUCTION_UNIMPLEMENTED_2:
	break;

