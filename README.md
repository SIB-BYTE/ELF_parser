# ELF_parser
64-bit ELF parser / dump that parses the: ELF header, program header, section header and symbol table!

# ELF file format:
A file format is a formation of bytes recognizable to the operating system for an ease of access, the ELF (executable / extensible and linkable format) file format is defined by linux and the formation looks like so:
![ELF](/imgs/ELF.png)
let's go ahead and analyze what this actually means.

# ELF header:
So the ELF header can be denoted with this structure:
```c
#define EI_NIDENT 16

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    ElfN_Addr     e_entry;
    ElfN_Off       e_phoff;
    ElfN_Off       e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    int16_t       e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} ElfN_Ehdr;
```
so let's analyze the attributes of this structure: 
``e_ident`` - An array of bytes at the beginning of the ELF executable that signifies certain information about the file, for example the first 4 bytes let's the operating system know if it's even an ELF, this is what's known as the "magic bytes". The magic bytes of the ELF header is: "\x7fELF" or "0x7f 0x45 0x4c 0x46". The next bytes of the array signify other things such as the architecture of the file, data specfifications of the system, ELF version, et cetera.

``e_type`` - The type of executable file it is. (relocatable file, executable file, shared object or core dumped file)

``e_machine`` - The type of machine the executable is running on.

``e_version`` - The version of the ELF binary.

``e_entry`` - The entry to the ``.text`` section or whenever code execution starts. 

``e_phoff`` - The offset of bytes from the beginning of the file to the start of the program header table.

``e_shoff`` - The offset of bytes from the beginning of the file to the start of the section header table.

``e_ehsize`` - ELF header size in bytes.

``e_phentsize`` - Size of program header entries.

``e_phnum`` - Number of entries in the program header table.

``e_shentsize`` - Size of section header entries.

``e_shnum`` - Number of entries in the section header table.

``e_shstrndx`` - Offset of bytes from the beginning of the file to the ``.strtab`` section in the section header table.

# Program header table:
The program header table is an array of ``Elf64_Phdr`` structures which look like this:
```c
typedef struct {
    uint32_t   p_type;
    uint32_t   p_flags;
    Elf64_Off   p_offset;
    Elf64_Addr p_vaddr;
    Elf64_Addr p_paddr;
    uint64_t   p_filesz;
    uint64_t   p_memsz;
    uint64_t   p_align;
} Elf64_Phdr;
```

``p_type`` - Indicates the type of segment that's being described at the current entry in the table. See ``man elf`` to see the types.

``p_flags`` - Flags of the current segment. (Executable, writable, readable, et cetera)

``p_offset`` - Offset in bytes from the beginning of the file from the current section table entry in memory.

``p_vaddr`` - Virtual address of the entry byte of the segment in virtual memory.

``p_paddr`` - Physical address of the entry byte of the segment in physcial memory.

# Section header table:

# Symbol table:
