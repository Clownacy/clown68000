#ifndef CLOWN68000_DISASSEMBLER_H
#define CLOWN68000_DISASSEMBLER_H

typedef long (*Clown68000_Disassemble_ReadCallback)(void *user_data);
typedef void (*Clown68000_Disassemble_PrintCallback)(void *user_data, const char *format, ...);

void Clown68000_Disassemble(unsigned long address, Clown68000_Disassemble_ReadCallback read_callback, Clown68000_Disassemble_PrintCallback print_callback, const void *user_data);

#endif /* CLOWN68000_DISASSEMBLER_H */
