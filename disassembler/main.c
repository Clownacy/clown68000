#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "disassembler.h"

static long ReadCallback(void *user_data)
{
	unsigned char bytes[2];

	(void)user_data;

	if (fread(bytes, 2, 1, (FILE*)user_data) != 1)
		return -1;

	return ((long)bytes[0] << 8) | bytes[1];
}

static void PrintCallback(void *user_data, const char *format, ...)
{
	va_list args;

	(void)user_data;

	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	fputc('\n', stdout);
}

int main(const int argc, char** const argv)
{
	int exit_code = EXIT_FAILURE;

	if (argc < 3)
	{
		fputs("Error: Path to file must be specified as file!\n", stderr);
	}
	else
	{
		FILE* const file = fopen(argv[1], "rb");

		if (file == NULL)
		{
			fputs("Error: Could not open file for reading!\n", stderr);
		}
		else
		{
			fseek(file, strtoul(argv[2], NULL, 0), SEEK_SET);
			Clown68000_Disassemble(ReadCallback, PrintCallback, file);
			fclose(file);

			exit_code = EXIT_SUCCESS;
		}
	}

	return exit_code;
}
