#include "../includes/elf.h"

int main(int argc, char **argv)
{
	LASSERT(argc == 2, "Usage: %s <ELF file>\n", argv[0]);
	int fd = open(argv[1], O_RDONLY);

	LASSERT(fd, "Binary doesn't exist\n");

	elf_parser_t *parser = init_parser(fd, argv[1]);
	dump_symbol_table(parser);

	destroy_parser(fd, parser);
}

