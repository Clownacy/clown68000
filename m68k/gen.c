case INSTRUCTION_ABCD:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_BCDX(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_BCDX(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ABCD;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((stuff.source_value & stuff.destination_value & ~stuff.result_value) | (~stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Cleared if the result is nonzero; unchanged otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_ADD:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADD;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & stuff.destination_value) | ((stuff.source_value | stuff.destination_value) & ~stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((stuff.source_value & stuff.destination_value & ~stuff.result_value) | (~stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_ADDA:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeSized(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_AddressRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDA;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ADDAQ:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDQ;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ADDI:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADD;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & stuff.destination_value) | ((stuff.source_value | stuff.destination_value) & ~stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((stuff.source_value & stuff.destination_value & ~stuff.result_value) | (~stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_ADDQ:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDQ;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & stuff.destination_value) | ((stuff.source_value | stuff.destination_value) & ~stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((stuff.source_value & stuff.destination_value & ~stuff.result_value) | (~stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_ADDX:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_BCDX(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_BCDX(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ADDX;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & stuff.destination_value) | ((stuff.source_value | stuff.destination_value) & ~stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((stuff.source_value & stuff.destination_value & ~stuff.result_value) | (~stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Cleared if the result is nonzero; unchanged otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_AND:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ANDI:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ANDI_TO_CCR:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_ConditionCodeRegister(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ANDI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_StatusRegister(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_AND;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ASD_MEMORY:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ASD_MEMORY;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ASD_REGISTER:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterPrimary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ASD_REGISTER;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BCC_SHORT:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCC_SHORT;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BCC_WORD:
	/* Decode source address mode. */
	DecodeSource_ImmediateDataWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCC_WORD;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BCHG_DYNAMIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCHG;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BCHG_STATIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateDataByte(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCHG;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BCLR_DYNAMIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCLR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BCLR_STATIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateDataByte(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BCLR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BRA_SHORT:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BRA_SHORT;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BRA_WORD:
	/* Decode source address mode. */
	DecodeSource_ImmediateDataWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BRA_WORD;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BSET_DYNAMIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSET;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BSET_STATIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateDataByte(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSET;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BSR_SHORT:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSR_SHORT;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BSR_WORD:
	/* Decode source address mode. */
	DecodeSource_ImmediateDataWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BSR_WORD;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BTST_DYNAMIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BTST;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_BTST_STATIC:
	SetSize_LongwordRegisterByteMemory(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateDataByte(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_BTST;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_CHK:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_CHK;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Undefined */
	/* Update OVERFLOW condition code */
	/* Undefined */
	/* Update ZERO condition code */
	/* Undefined */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_CLR:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_CLR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_CMP:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_CMPA:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeSized(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_AddressRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBA;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_CMPI:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_CMPM:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_AddressRegisterPrimaryPostIncrement(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_AddressRegisterSecondaryPostIncrement(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_DBCC:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_DBCC;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_DIVS:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_DIV;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_DIVU:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_DIV;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_EOR:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_EORI:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_EORI_TO_CCR:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_ConditionCodeRegister(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_EORI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_StatusRegister(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EOR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_EXG:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EXG;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_EXT:
	SetSize_Ext(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterPrimary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_EXT;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ILLEGAL:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ILLEGAL;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_JMP:
	/* Decode source address mode. */
	DecodeSource_MemoryAddressPrimary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_JMP;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_JSR:
	/* Decode source address mode. */
	DecodeSource_MemoryAddressPrimary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_JSR;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_LEA:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_MemoryAddressPrimary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_AddressRegisterSecondary(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_LINK:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_LINK;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_LSD_MEMORY:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_LSD_MEMORY;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_LSD_REGISTER:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterPrimary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_LSD_REGISTER;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVE:
	SetSize_Move(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_SecondaryAddressMode(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVE_FROM_SR:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_StatusRegister(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVE_TO_CCR:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_ConditionCodeRegister(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVE_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_StatusRegister(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVE_USP:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE_USP;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVEA:
	SetSize_Move(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_AddressRegisterSecondaryFull(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEA;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVEM:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_MOVEM(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEM;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVEP:
	/* Decode destination address mode. */
	DecodeDestination_MOVEP(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEP;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MOVEQ:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondary(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVEQ;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MULS:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MUL;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_MULU:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeWord(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MUL;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_NBCD:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NBCD;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Cleared if the result is nonzero; unchanged otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_NEG:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NEG;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= ((stuff.destination_value | stuff.result_value) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= ((stuff.destination_value & stuff.result_value) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_NEGX:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NEGX;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= ((stuff.destination_value | stuff.result_value) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= ((stuff.destination_value & stuff.result_value) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Cleared if the result is nonzero; unchanged otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_NOP:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NOP;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_NOT:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_NOT;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_OR:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ORI:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ORI_TO_CCR:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_ConditionCodeRegister(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ORI_TO_SR:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_StatusRegister(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_OR;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_PEA:
	/* Decode source address mode. */
	DecodeSource_MemoryAddressPrimary(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_PEA;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_RESET:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RESET;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ROD_MEMORY:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROD_MEMORY;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ROD_REGISTER:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterPrimary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROD_REGISTER;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ROXD_MEMORY:
	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROXD_MEMORY;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_ROXD_REGISTER:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterPrimary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_ROXD_REGISTER;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_RTE:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RTE;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_RTR:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RTR;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_RTS:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_RTS;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_SBCD:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_BCDX(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_BCDX(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SBCD;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Cleared if the result is nonzero; unchanged otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_SCC:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SCC;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_STOP:
	/* Only allow this instruction in supervisor mode. */
	SupervisorCheck(&stuff);

	SetSize_Word(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_STOP;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_SUB:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_SUBA:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressModeSized(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_AddressRegisterSecondary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBA;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_SUBAQ:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBQ;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_SUBI:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_ImmediateData(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUB;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_SUBQ:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBQ;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_SUBX:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_BCDX(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_BCDX(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SUBX;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	state->status_register |= (((stuff.source_value & ~stuff.destination_value) | ((stuff.source_value | ~stuff.destination_value) & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_CARRY_BIT)) & CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	state->status_register |= (((~stuff.source_value & stuff.destination_value & ~stuff.result_value) | (stuff.source_value & ~stuff.destination_value & stuff.result_value)) >> (stuff.msb_bit_index - CONDITION_CODE_OVERFLOW_BIT)) & CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Cleared if the result is nonzero; unchanged otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO | (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Standard behaviour: set to CARRY */
	state->status_register &= ~CONDITION_CODE_EXTEND;
	state->status_register |= CONDITION_CODE_EXTEND & (0 - ((state->status_register & CONDITION_CODE_CARRY) != 0));

	break;

case INSTRUCTION_SWAP:
	SetSize_Longword(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_DataRegisterPrimary(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_SWAP;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_TAS:
	SetSize_Byte(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode destination address mode. */
	DecodeDestination_PrimaryAddressMode(&stuff);

	/* Read destination operand. */
	ReadDestination(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_TAS;

	/* Write destination operand. */
	WriteDestination(&stuff);

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_TRAP:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_TRAP;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_TRAPV:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_TRAPV;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_TST:
	SetSize_Standard(&stuff);
	SetMSBBitIndex(&stuff);

	/* Decode source address mode. */
	DecodeSource_PrimaryAddressMode(&stuff);

	/* Read source operand. */
	ReadSource(&stuff);

	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_MOVE;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	state->status_register &= ~CONDITION_CODE_CARRY;
	/* Update OVERFLOW condition code */
	state->status_register &= ~CONDITION_CODE_OVERFLOW;
	/* Update ZERO condition code */
	/* Standard behaviour: set if result is zero; clear otherwise */
	state->status_register &= ~CONDITION_CODE_ZERO;
	state->status_register |= CONDITION_CODE_ZERO & (0 - ((stuff.result_value & (0xFFFFFFFF >> (32 - stuff.msb_bit_index - 1))) == 0));
	/* Update NEGATIVE condition code */
	/* Standard behaviour: set if result value is negative; clear otherwise */
	state->status_register &= ~CONDITION_CODE_NEGATIVE;
	state->status_register |= CONDITION_CODE_NEGATIVE & (stuff.result_value >> (stuff.msb_bit_index - CONDITION_CODE_NEGATIVE_BIT)) & CONDITION_CODE_NEGATIVE;
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_UNLK:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_UNLK;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_UNIMPLEMENTED_1:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_UNIMPLEMENTED_1;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

case INSTRUCTION_UNIMPLEMENTED_2:
	/* Do the actual instruction. */
	DO_INSTRUCTION_ACTION_UNIMPLEMENTED_2;

	/* Update the condition codes in the following order: */
	/* CARRY, OVERFLOW, ZERO, NEGATIVE, EXTEND */

	/* Update CARRY condition code */
	/* Unaffected */
	/* Update OVERFLOW condition code */
	/* Unaffected */
	/* Update ZERO condition code */
	/* Unaffected */
	/* Update NEGATIVE condition code */
	/* Unaffected */
	/* Update EXTEND condition code */
	/* Unaffected */

	break;

