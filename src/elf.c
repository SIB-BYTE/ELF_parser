#include "../includes/elf.h"

// Initializes the elf_parser_t structure:
elf_parser_t *init_parser(int fd, char *file_name)
{
	// Structure initlaization and file mapping:
	elf_parser_t *parser  = calloc(1, sizeof(elf_parser_t));
	LASSERT(parser, "memory allocation failure\n");

	parser->program_size  = get_elf_size(file_name);
	parser->program_buffer = mmap(NULL, parser->program_size, PROT_READ, MAP_PRIVATE, fd, 0);
	LASSERT(parser->program_buffer, "Failure to map file into memory");

	// Setting the ELF header to first 64-bytes of our program buffer:
	parser->elf_header     = (Elf64_Ehdr *) parser->program_buffer;

	// Accessing the program, section & symbol header with the information in the ELF header:
	parser->program_header = (Elf64_Phdr *) (parser->program_buffer + parser->elf_header->e_phoff);
	parser->section_header = (Elf64_Shdr *) (parser->program_buffer + parser->elf_header->e_shoff);
	parser->symbol_table   = (Elf64_Sym  *) (parser->program_buffer);

	// Setting the start of the string_table entry:
	parser->string_table_buffer = parser->program_buffer + parser->section_header[parser->elf_header->e_shstrndx].sh_offset;

	return (parser);
}

// Displays the ELF header information:
void dump_elf_header(elf_parser_t *parser)
{
	// Validates the first magic bytes aligns with what ELF format desires it to be:
	LASSERT(!validate_elf(parser), "Invalid ELF\n");

	// Displays the first 16 bytes of the file (so called magic bytes):
	printf("%sMagic: ", "\x1b[0;34m");
	for(int i = 0; i < EI_NIDENT; ++i)
		printf("%0x ", parser->elf_header->e_ident[i]);

	putchar(0xa);

	// Displays the type of file the binary is:
	printf("Storage class: ");
	switch(parser->elf_header->e_ident[EI_CLASS])
	{
		case ELFCLASS32:   puts("32-bit ELF"); break;
		case ELFCLASS64:   puts("64-bit ELF"); break;
		case ELFCLASSNONE: puts("Invalid storage class type"); break;
	}

	// Displays the data formation layout:
	printf("Data format: ");
	switch(parser->elf_header->e_ident[EI_DATA])
	{
		case ELFDATANONE: puts("Invalid data formation!"); break;
		case ELFDATA2LSB: puts("Little endian | 2's complement support"); break;
		case ELFDATA2MSB: puts("Big endian | 2's complement support");
	}

	// Displays file version:
	printf("ELF specifcation version: ");
	switch(parser->elf_header->e_ident[EI_VERSION])
	{
		case EV_NONE:    puts("Invalid version!"); break;
		case EV_CURRENT: printf("Valid version -> {%d}\n", parser->elf_header->e_ident[EI_VERSION]); break;
	}

	// Application binary interface specifications of your operating system:
	printf("ABI specifications: ");
	switch(parser->elf_header->e_ident[EI_OSABI])
	{
		case ELFOSABI_SYSV:    puts("System V ABI"); break;
		case ELFOSABI_HPUX:    puts("HP_UX ABI");    break;
		case ELFOSABI_NETBSD:  puts("NetBSD ABI");   break;
		case ELFOSABI_LINUX:   puts("Linux ABI");    break;
		case ELFOSABI_SOLARIS: puts("Solaris ABI");  break;
		case ELFOSABI_IRIX:    puts("IRIX ABI");     break;
		case ELFOSABI_FREEBSD: puts("Free BSD ABI"); break;
		case ELFOSABI_TRU64:   puts("TRU64 ABI");    break;
		case ELFOSABI_ARM:     puts("ARM ABI");	     break;
		default: puts("Stand alone || Invalid ABI"); break;
	}

	// Displays the file type:
	printf("File type: ");
	switch(parser->elf_header->e_type)
	{
		case ET_NONE: puts("Invalid type!");     break;
		case ET_REL:  puts("Relocatable file");   break;
		case ET_EXEC: puts("Executable file");    break;
		case ET_DYN:  puts("Shared object file"); break;
		case ET_CORE: puts("Core file");	  break;
	}

	// Displays the architecture type of the machine you're on:
	printf("Machine type: ");
	switch(parser->elf_header->e_machine)
	{
		case EM_NONE:    puts("Unknown machine!"); break;
		case EM_M32:     puts("AT&T WE 32100");    break;
		case EM_SPARC:   puts("Sun microsystems"); break;
		case EM_386:     puts("Intel 80386");      break;
		case EM_68K:     puts("Motorola 68000");   break;
		case EM_88K:     puts("Motorola 88000");   break;
		case EM_860:     puts("Intel 80860");      break;
		case EM_MIPS:    puts("MIPS RS3000");	   break;
		case EM_PARISC:  puts("HP || PA");	   break;
		case EM_PPC:     puts("PowerPC");	   break;
		case EM_PPC64:   puts("PowerPC 64-bit");   break;
		case EM_S390:    puts("IBM S/390");        break;
		case EM_ARM:     puts("RISC machines");    break;
		case EM_SH:      puts("Renesas superh");   break;
		case EM_SPARCV9: puts("SPARC V9 64-bit");  break;
		case EM_IA_64:   puts("Intel Itanium");    break;
		case EM_X86_64:  puts("x86-64");	   break;
		case EM_VAX:     puts("VAX machine");	   break;
	}

	// Display other ELF information:
	printf("ELF entry: {0x%08lx}\n"
	       "Program header offset: {0x%lu}\n"
	       "Section header offset: {0x%lu}\n"
	       "ELF flags: {%s}\n"
	       "ELF header size: {%hu}\n"
	       "Size of entries in program header table: {%hu}\n"
	       "Program header table entries: {%hu}\n"
	       "Size of each section header entry: {%u}\n"
	       "Section header table entry count: {%hu}\n",
		parser->elf_header->e_entry,
		parser->elf_header->e_phoff,
		parser->elf_header->e_shoff,
		dump_elf_flag(parser->elf_header->e_flags),
		parser->elf_header->e_ehsize,
		parser->elf_header->e_phentsize,
		parser->elf_header->e_phnum,
		parser->elf_header->e_shentsize,
		parser->elf_header->e_shnum);

	puts(CLR);
}

// Displays the program header table:
void dump_program_header(elf_parser_t *parser)
{
	for(int i = 0; i < parser->elf_header->e_phnum; ++i)
	{
		printf("%sType: {%s}\n"
		       "Flags: {%s}\n"
		       "Offset: {%#lx}\n"
		       "Virtual address: {%#lx}\n"
		       "Physical address: {%#lx}\n"
		       "File image size: {%#lx}\n"
		       "Memory image size: {%#lx}\n"
		       "Memory aligned offset: {%#lx}\n",
		       "\x1b[0;31m",
			dump_program_type(parser->program_header[i].p_type),
			dump_program_flag(parser->program_header[i].p_flags),
			parser->program_header[i].p_offset,
			parser->program_header[i].p_vaddr,
			parser->program_header[i].p_paddr,
			parser->program_header[i].p_filesz,
			parser->program_header[i].p_memsz,
			parser->program_header[i].p_align);

		putchar(0xa);
	}

	puts(CLR);
}

// Dumps the section header table:
void dump_section_header(elf_parser_t *parser)
{
	for(int i = 0; i < parser->elf_header->e_shnum; ++i)
	{
		printf("%sSection: (%s)\n"
		       "Type: {%s}\n"
		       "Flags: {%s}\n"
		       "Address: {%#lx}\n"
		       "Offset: {%#lx}\n"
		       "Size: {%lu bytes}\n"
		       "Index link: {%u}\n"
		       "Address alignment: {%#lx}\n"
		       "Entry size: {%lu bytes}\n",
		       "\x1b[0;32m",
			(parser->section_header[i].sh_name + parser->string_table_buffer),
			dump_section_type(parser->section_header[i].sh_type),
			dump_section_flag(parser->section_header[i].sh_flags),
			parser->section_header[i].sh_addr,
			parser->section_header[i].sh_offset,
			parser->section_header[i].sh_size,
			parser->section_header[i].sh_link,
			parser->section_header[i].sh_addralign,
			parser->section_header[i].sh_entsize);

		putchar(0xa);
	}

	puts(CLR);
}

// Displays the symbol table:
void dump_symbol_table(elf_parser_t *parser)
{
	for(int i = 0; i < parser->elf_header->e_shnum; ++i)
	{
		if(parser->section_header[i].sh_type != SHT_SYMTAB)
			continue;

		int index = parser->section_header[i].sh_link;
		size_t symbol_count = parser->section_header[i].sh_size / sizeof(Elf64_Sym);

		parser->symbol_table = (Elf64_Sym *) (parser->program_buffer + parser->section_header[i].sh_offset);
		parser->symbol_table_buffer = parser->program_buffer + parser->section_header[index].sh_offset;

		for(size_t i = 0; i < symbol_count; ++i)
		{
			printf("%sSymbol name: {%s}\n"
			       "Symbol value: {%#lx}\n"
			       "Symbol size: {%#lx}\n"
			       "Symbols type: {%s}\n"
			       "Symbols binding: {%s}\n"
			       "Symbols visibility: {%s}\n"
			       "Symbols section header table index: {%u}\n",
			       "\x1b[0;35m",
			       (parser->symbol_table_buffer + parser->symbol_table[i].st_name),
				parser->symbol_table[i].st_value,
				parser->symbol_table[i].st_size,
				dump_symbol_type(parser->symbol_table[i].st_info),
				dump_symbol_bind(parser->symbol_table[i].st_info),
				dump_symbol_visibility(parser->symbol_table[i].st_other),
				parser->symbol_table[i].st_shndx);

			putchar(0xa);
		}
	}

	puts(CLR);
}

// Deinitialization:
void destroy_parser(int fd, elf_parser_t *parser)
{
	munmap(parser->program_buffer, parser->program_size);
	free(parser);
	close(fd);
}

