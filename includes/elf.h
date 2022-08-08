#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Debug macro-defined function:
#define LASSERT(condition, message, ...) \
    do \
    { \
        if(!(condition)) \
        { \
            fprintf(stderr, "[*] Process terminated! LASSERT(); failed!\nCondition\t: {%s}\nFunction\t: {%s}\nFailed in file\t: {%s}\nAt line \t: {%d}\n", #condition, __func__, __FILE__, __LINE__);\
            fprintf(stderr, message, ##__VA_ARGS__); \
            exit(1); \
        } \
    } while(0)

// Escape code to reset coloring:
#define CLR "\x1b[0m"

// ELF parser structure:
typedef struct elf_parser {
	char *program_buffer;		/* Mmaped file buffer */
	size_t program_size;		/* Size of mmaped file*/

	char *string_table_buffer;  /* 64-bit string table buffa */
	char *symbol_table_buffer;  /* 64-bit symbol string buffa */

	Elf64_Ehdr *elf_header;     /* 64-bit ELF header     */
	Elf64_Sym  *symbol_table;   /* 64-bit symbol table   */
	Elf64_Phdr *program_header; /* 64-bit program header */
	Elf64_Shdr *section_header; /* 64-bit section header */
} elf_parser_t;

/* Initalization && Deinitialization: */
elf_parser_t *init_parser(int, char *);
void destroy_parser(int, elf_parser_t *);

/* Display ELF information: */
void dump_elf_header(elf_parser_t *);
void dump_string_table(elf_parser_t *);
void dump_symbol_table(elf_parser_t *);
void dump_program_header(elf_parser_t *);
void dump_section_header(elf_parser_t *);

/* Flag & type mapping + dumping: */
// ELF flags:
const char *dump_elf_flag(int);

// Program header flags and types:
const char *dump_program_type(int);
const char *dump_program_flag(int);

// Section header:
const char *dump_section_type(int);
const char *dump_section_flag(unsigned long);

// Symbol table:
const char *dump_symbol_bind(unsigned char);
const char *dump_symbol_type(unsigned char);
const char *dump_symbol_visibility(unsigned char);

/* Misc: */
size_t get_elf_size(char *);
bool validate_elf(elf_parser_t *);

#endif

