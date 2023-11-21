static void (* const microcode_ABCD[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeSource_BCDX,
	ReadSource,
	DecodeDestination_BCDX,
	ReadDestination,
	Action_ABCD,
	WriteDestination,
	Overflow_ADD,
	Zero_ClearIfNonZeroUnaffectedOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_ADD[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadSource,
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadDestination,
	Action_ADD,
	WriteDestination,
	Carry_StandardCarry,
	Overflow_ADD,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_ADDA[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeSized,
	ReadSource,
	DecodeDestination_AddressRegisterSecondary,
	ReadDestination,
	Action_ADDA,
	WriteDestination,
	NULL
};

static void (* const microcode_ADDAQ[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_ADDQ,
	WriteDestination,
	NULL
};

static void (* const microcode_ADDI[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_ADD,
	WriteDestination,
	Carry_StandardCarry,
	Overflow_ADD,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_ADDQ[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_ADDQ,
	WriteDestination,
	Carry_StandardCarry,
	Overflow_ADD,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_ADDX[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_BCDX,
	ReadSource,
	DecodeDestination_BCDX,
	ReadDestination,
	Action_ADDX,
	WriteDestination,
	Carry_StandardCarry,
	Overflow_ADD,
	Zero_ClearIfNonZeroUnaffectedOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_AND[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadSource,
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadDestination,
	Action_AND,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ANDI[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_AND,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ANDI_TO_CCR[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_ConditionCodeRegister,
	ReadDestination,
	Action_AND,
	WriteDestination,
	NULL
};

static void (* const microcode_ANDI_TO_SR[])(Stuff* const stuff) = {
	SupervisorCheck,
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_StatusRegister,
	ReadDestination,
	Action_AND,
	WriteDestination,
	NULL
};

static void (* const microcode_ASD_MEMORY[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_ASD_MEMORY,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ASD_REGISTER[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterPrimary,
	ReadDestination,
	Action_ASD_REGISTER,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_BCC_SHORT[])(Stuff* const stuff) = {
	Action_BCC_SHORT,
	NULL
};

static void (* const microcode_BCC_WORD[])(Stuff* const stuff) = {
	DecodeSource_ImmediateDataWord,
	ReadSource,
	Action_BCC_WORD,
	NULL
};

static void (* const microcode_BCHG_DYNAMIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondary,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BCHG,
	WriteDestination,
	NULL
};

static void (* const microcode_BCHG_STATIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_ImmediateDataByte,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BCHG,
	WriteDestination,
	NULL
};

static void (* const microcode_BCLR_DYNAMIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondary,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BCLR,
	WriteDestination,
	NULL
};

static void (* const microcode_BCLR_STATIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_ImmediateDataByte,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BCLR,
	WriteDestination,
	NULL
};

static void (* const microcode_BRA_SHORT[])(Stuff* const stuff) = {
	Action_BRA_SHORT,
	NULL
};

static void (* const microcode_BRA_WORD[])(Stuff* const stuff) = {
	DecodeSource_ImmediateDataWord,
	ReadSource,
	Action_BRA_WORD,
	NULL
};

static void (* const microcode_BSET_DYNAMIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondary,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BSET,
	WriteDestination,
	NULL
};

static void (* const microcode_BSET_STATIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_ImmediateDataByte,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BSET,
	WriteDestination,
	NULL
};

static void (* const microcode_BSR_SHORT[])(Stuff* const stuff) = {
	Action_BSR_SHORT,
	NULL
};

static void (* const microcode_BSR_WORD[])(Stuff* const stuff) = {
	DecodeSource_ImmediateDataWord,
	ReadSource,
	Action_BSR_WORD,
	NULL
};

static void (* const microcode_BTST_DYNAMIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondary,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BTST,
	NULL
};

static void (* const microcode_BTST_STATIC[])(Stuff* const stuff) = {
	SetSize_LongwordRegisterByteMemory,
	SetMSBBitIndex,
	DecodeSource_ImmediateDataByte,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_BTST,
	NULL
};

static void (* const microcode_CHK[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	Action_CHK,
	NULL
};

static void (* const microcode_CLR[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_CLR,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_CMP[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	DecodeDestination_DataRegisterSecondary,
	ReadDestination,
	Action_SUB,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_CMPA[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeSized,
	ReadSource,
	DecodeDestination_AddressRegisterSecondary,
	ReadDestination,
	Action_SUBA,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_CMPI[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_SUB,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_CMPM[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_AddressRegisterPrimaryPostIncrement,
	ReadSource,
	DecodeDestination_AddressRegisterSecondaryPostIncrement,
	ReadDestination,
	Action_SUB,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_DBCC[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	Action_DBCC,
	NULL
};

static void (* const microcode_DIVS[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeWord,
	ReadSource,
	DecodeDestination_DataRegisterSecondary,
	ReadDestination,
	Action_DIVS,
	WriteDestination,
	Carry_Clear,
	NULL
};

static void (* const microcode_DIVU[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeWord,
	ReadSource,
	DecodeDestination_DataRegisterSecondary,
	ReadDestination,
	Action_DIVU,
	WriteDestination,
	Carry_Clear,
	NULL
};

static void (* const microcode_EOR[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondary,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_EOR,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_EORI[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_EOR,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_EORI_TO_CCR[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_ConditionCodeRegister,
	ReadDestination,
	Action_EOR,
	WriteDestination,
	NULL
};

static void (* const microcode_EORI_TO_SR[])(Stuff* const stuff) = {
	SupervisorCheck,
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_StatusRegister,
	ReadDestination,
	Action_EOR,
	WriteDestination,
	NULL
};

static void (* const microcode_EXG[])(Stuff* const stuff) = {
	Action_EXG,
	NULL
};

static void (* const microcode_EXT[])(Stuff* const stuff) = {
	SetSize_Ext,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterPrimary,
	ReadDestination,
	Action_EXT,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ILLEGAL[])(Stuff* const stuff) = {
	Action_ILLEGAL,
	NULL
};

static void (* const microcode_JMP[])(Stuff* const stuff) = {
	DecodeSource_MemoryAddressPrimary,
	ReadSource,
	Action_JMP,
	NULL
};

static void (* const microcode_JSR[])(Stuff* const stuff) = {
	DecodeSource_MemoryAddressPrimary,
	ReadSource,
	Action_JSR,
	NULL
};

static void (* const microcode_LEA[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_MemoryAddressPrimary,
	ReadSource,
	DecodeDestination_AddressRegisterSecondary,
	Action_MOVE,
	WriteDestination,
	NULL
};

static void (* const microcode_LINK[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	Action_LINK,
	NULL
};

static void (* const microcode_LSD_MEMORY[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_LSD_MEMORY,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_LSD_REGISTER[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterPrimary,
	ReadDestination,
	Action_LSD_REGISTER,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_MOVE[])(Stuff* const stuff) = {
	SetSize_Move,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	DecodeDestination_SecondaryAddressMode,
	Action_MOVE,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_MOVE_FROM_SR[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_StatusRegister,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	Action_MOVE,
	WriteDestination,
	NULL
};

static void (* const microcode_MOVE_TO_CCR[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	DecodeDestination_ConditionCodeRegister,
	Action_MOVE,
	WriteDestination,
	NULL
};

static void (* const microcode_MOVE_TO_SR[])(Stuff* const stuff) = {
	SupervisorCheck,
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	DecodeDestination_StatusRegister,
	Action_MOVE,
	WriteDestination,
	NULL
};

static void (* const microcode_MOVE_USP[])(Stuff* const stuff) = {
	SupervisorCheck,
	Action_MOVE_USP,
	NULL
};

static void (* const microcode_MOVEA[])(Stuff* const stuff) = {
	SetSize_Move,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	DecodeDestination_AddressRegisterSecondaryFull,
	Action_MOVEA,
	WriteDestination,
	NULL
};

static void (* const microcode_MOVEM[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_MOVEM,
	ReadDestination,
	Action_MOVEM,
	NULL
};

static void (* const microcode_MOVEP[])(Stuff* const stuff) = {
	DecodeDestination_MOVEP,
	ReadDestination,
	Action_MOVEP,
	NULL
};

static void (* const microcode_MOVEQ[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterSecondary,
	Action_MOVEQ,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_MULS[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeWord,
	ReadSource,
	DecodeDestination_DataRegisterSecondary,
	ReadDestination,
	Action_MULS,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_MULU[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeWord,
	ReadSource,
	DecodeDestination_DataRegisterSecondary,
	ReadDestination,
	Action_MULU,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_NBCD[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_NBCD,
	WriteDestination,
	Overflow_SUB,
	Zero_ClearIfNonZeroUnaffectedOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_NEG[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_NEG,
	WriteDestination,
	Carry_NEG,
	Overflow_NEG,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_NEGX[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_NEGX,
	WriteDestination,
	Carry_NEG,
	Overflow_NEG,
	Zero_ClearIfNonZeroUnaffectedOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_NOP[])(Stuff* const stuff) = {
	Action_NOP,
	NULL
};

static void (* const microcode_NOT[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_NOT,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_OR[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadSource,
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadDestination,
	Action_OR,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ORI[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_OR,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ORI_TO_CCR[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_ConditionCodeRegister,
	ReadDestination,
	Action_OR,
	WriteDestination,
	NULL
};

static void (* const microcode_ORI_TO_SR[])(Stuff* const stuff) = {
	SupervisorCheck,
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_StatusRegister,
	ReadDestination,
	Action_OR,
	WriteDestination,
	NULL
};

static void (* const microcode_PEA[])(Stuff* const stuff) = {
	DecodeSource_MemoryAddressPrimary,
	ReadSource,
	Action_PEA,
	NULL
};

static void (* const microcode_RESET[])(Stuff* const stuff) = {
	SupervisorCheck,
	Action_RESET,
	NULL
};

static void (* const microcode_ROD_MEMORY[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_ROD_MEMORY,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ROD_REGISTER[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterPrimary,
	ReadDestination,
	Action_ROD_REGISTER,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ROXD_MEMORY[])(Stuff* const stuff) = {
	SetSize_Word,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_ROXD_MEMORY,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_ROXD_REGISTER[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterPrimary,
	ReadDestination,
	Action_ROXD_REGISTER,
	WriteDestination,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_RTE[])(Stuff* const stuff) = {
	SupervisorCheck,
	Action_RTE,
	NULL
};

static void (* const microcode_RTR[])(Stuff* const stuff) = {
	Action_RTR,
	NULL
};

static void (* const microcode_RTS[])(Stuff* const stuff) = {
	Action_RTS,
	NULL
};

static void (* const microcode_SBCD[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeSource_BCDX,
	ReadSource,
	DecodeDestination_BCDX,
	ReadDestination,
	Action_SBCD,
	WriteDestination,
	Overflow_SUB,
	Zero_ClearIfNonZeroUnaffectedOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_SCC[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_SCC,
	WriteDestination,
	NULL
};

static void (* const microcode_STOP[])(Stuff* const stuff) = {
	SupervisorCheck,
	SetSize_Word,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	Action_STOP,
	NULL
};

static void (* const microcode_SUB[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadSource,
	DecodeDestination_DataRegisterSecondaryOrPrimaryAddressMode,
	ReadDestination,
	Action_SUB,
	WriteDestination,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_SUBA[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressModeSized,
	ReadSource,
	DecodeDestination_AddressRegisterSecondary,
	ReadDestination,
	Action_SUBA,
	WriteDestination,
	NULL
};

static void (* const microcode_SUBAQ[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_SUBQ,
	WriteDestination,
	NULL
};

static void (* const microcode_SUBI[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_ImmediateData,
	ReadSource,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_SUB,
	WriteDestination,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_SUBQ[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_SUBQ,
	WriteDestination,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_SUBX[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_BCDX,
	ReadSource,
	DecodeDestination_BCDX,
	ReadDestination,
	Action_SUBX,
	WriteDestination,
	Carry_StandardBorrow,
	Overflow_SUB,
	Zero_ClearIfNonZeroUnaffectedOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	Extend_SetToCarry,
	NULL
};

static void (* const microcode_SWAP[])(Stuff* const stuff) = {
	SetSize_Longword,
	SetMSBBitIndex,
	DecodeDestination_DataRegisterPrimary,
	ReadDestination,
	Action_SWAP,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_TAS[])(Stuff* const stuff) = {
	SetSize_Byte,
	SetMSBBitIndex,
	DecodeDestination_PrimaryAddressMode,
	ReadDestination,
	Action_TAS,
	WriteDestination,
	Carry_Clear,
	Overflow_Clear,
	NULL
};

static void (* const microcode_TRAP[])(Stuff* const stuff) = {
	Action_TRAP,
	NULL
};

static void (* const microcode_TRAPV[])(Stuff* const stuff) = {
	Action_TRAPV,
	NULL
};

static void (* const microcode_TST[])(Stuff* const stuff) = {
	SetSize_Standard,
	SetMSBBitIndex,
	DecodeSource_PrimaryAddressMode,
	ReadSource,
	Action_MOVE,
	Carry_Clear,
	Overflow_Clear,
	Zero_SetIfZeroClearOtherwise,
	Negative_SetIfNegativeClearOtherwise,
	NULL
};

static void (* const microcode_UNLK[])(Stuff* const stuff) = {
	Action_UNLK,
	NULL
};

static void (* const microcode_UNIMPLEMENTED_1[])(Stuff* const stuff) = {
	Action_UNIMPLEMENTED_1,
	NULL
};

static void (* const microcode_UNIMPLEMENTED_2[])(Stuff* const stuff) = {
	Action_UNIMPLEMENTED_2,
	NULL
};

