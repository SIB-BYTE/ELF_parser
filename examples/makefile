# Compiler choice:
GCC  = gcc
LLVM = clang

# Source code(s):
ELF_SRC   = ../src/elf.c
MISC_SRC  = ../src/misc.c
BUILD_SRC = $(ELF_SRC) $(MISC_SRC)

# External source codes:
ELF_HEADER_SRC     = dump_elf_header.c
SECTION_HEADER_SRC = dump_section_header.c
PROGRAM_HEADER_SRC = dump_program_header.c
SYMBOL_TABLE_SRC   = dump_symbol_table.c

# Compiler flags:
FLAGS       = -Wall -Wextra -Wpedantic -O3 -g3 -o
DEBUG_FLAGS = -Wall -Wextra -Wpedantic -O3 -g3 -fsanitize=address,undefined -o

# Build emitted binary names:
DUMP_ELF_BIN     = dump_elf_header
DUMP_SECTION_BIN = dump_section_header
DUMP_PROGRAM_BIN = dump_program_header
DUMP_SYM_TBL_BIN = dump_symbol_table

build_elf_header:
	$(GCC) $(ELF_HEADER_SRC) $(BUILD_SRC) $(FLAGS) $(DUMP_ELF_BIN)

build_elf_header_debug:
	$(GCC) $(ELF_HEADER_SRC) $(BUILD_SRC) $(DEBUG_FLAGS) $(DUMP_ELF_BIN)

build_section_header:
	$(GCC) $(SECTION_HEADER_SRC) $(BUILD_SRC) $(FLAGS) $(DUMP_SECTION_BIN)

build_section_header_debug:
	$(GCC) $(SECTION_HEADER_SRC) $(BULD_SRC) $(DEBUG_FLAGS) $(DUMP_SECTION_BIN)

build_program_header:
	$(GCC) $(PROGRAM_HEADER_SRC) $(BUILD_SRC) $(FLAGS) $(DUMP_PROGRAM_BIN)

build_program_header_debug:
	$(GCC) $(PROGRAM_HEADER_SRC) $(BUILD_SRC) $(DEBUG_FLAGS) $(DUMP_PROGRAM_BIN)

build_symbol_table:
	$(GCC) $(SYMBOL_TABLE_SRC) $(BUILD_SRC) $(FLAGS) $(DUMP_SYM_TBL_BIN)

build_symbol_table_debug:
	$(GCC) $(SYMBOL_TABLE_SRC) $(BUILD_SRC) $(DEBUG_FLAGS) $(DUMP_SYM_TBL_BIN)

