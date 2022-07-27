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
This array of structures makes up the program header entries that make up individual segments in our binary. An example of some of those segment sections are so:
```
01     .interp 
02     .interp .note.gnu.property .note.gnu.build-id .note.ABI-tag .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt 
03     .init .plt .text .fini 
04     .rodata .eh_frame_hdr .eh_frame 
05     .init_array .fini_array .dynamic .got .got.plt .data .bss 
06     .dynamic 
07     .note.gnu.property 
08     .note.gnu.build-id .note.ABI-tag 
09     .note.gnu.property 
10     .eh_frame_hdr 
11     
12     .init_array .fini_array .dynamic .got 
```

``Elf64_Phdr`` field linguistic definitions: 

``p_type`` - Indicates the type of segment that's being described at the current entry in the table. See ``man elf`` to see the types.

``p_flags`` - Flags of the current segment. (Executable, writable, readable, et cetera)

``p_offset`` - Offset in bytes from the beginning of the file from the current section table entry in memory.

``p_vaddr`` - Virtual address of the entry byte of the segment in virtual memory.

``p_paddr`` - Physical address of the entry byte of the segment in physcial memory.

``p_filesz`` - Number of bytes in the file image of the segment.

``p_memsz`` - Number of bytes in the memory image of the segment.

``p_align`` - The byte alignment in memory between segments loaded with ``PT_LOAD`` in memory.

# Section header table:
The section header table is an array of structures that looks like:
```c
typedef struct elf64_shdr {
    Elf64_Word  sh_name;
    Elf64_Word  sh_type;
    Elf64_Xword sh_flags;		 
    Elf64_Addr  sh_addr;		
    Elf64_Off    sh_offset;		 
    Elf64_Xword sh_size;		
    Elf64_Word  sh_link;		
    Elf64_Word  sh_info;		
    Elf64_Xword sh_addralign;	
    Elf64_Xword sh_entsize;
} Elf64_Shdr;
```
Each one of these entries represents a section in the section header table, like so:
```
0x0000000000000318 - 0x0000000000000334 is .interp
0x0000000000000338 - 0x0000000000000378 is .note.gnu.property
0x0000000000000378 - 0x000000000000039c is .note.gnu.build-id
0x000000000000039c - 0x00000000000003bc is .note.ABI-tag
0x00000000000003c0 - 0x00000000000003dc is .gnu.hash
0x00000000000003e0 - 0x0000000000000488 is .dynsym
0x0000000000000488 - 0x000000000000050a is .dynstr
0x000000000000050a - 0x0000000000000518 is .gnu.version
0x0000000000000518 - 0x0000000000000538 is .gnu.version_r
0x0000000000000538 - 0x00000000000005f8 is .rela.dyn
0x00000000000005f8 - 0x0000000000000610 is .rela.plt
0x0000000000001000 - 0x000000000000101b is .init
0x0000000000001020 - 0x0000000000001040 is .plt
0x0000000000001040 - 0x00000000000011d5 is .text
0x00000000000011d8 - 0x00000000000011e5 is .fini
0x0000000000002000 - 0x0000000000002048 is .rodata
0x0000000000002048 - 0x000000000000207c is .eh_frame_hdr
0x0000000000002080 - 0x0000000000002150 is .eh_frame
0x0000000000003de8 - 0x0000000000003df0 is .init_array
0x0000000000003df0 - 0x0000000000003df8 is .fini_array
0x0000000000003df8 - 0x0000000000003fd8 is .dynamic
0x0000000000003fd8 - 0x0000000000004000 is .got
0x0000000000004000 - 0x0000000000004020 is .got.plt
0x0000000000004020 - 0x0000000000004030 is .data
0x0000000000004030 - 0x0000000000004038 is .bss
```
``Elf64_Shdr`` field lingustic definitions:

``sh_name`` - Index into the string table for the specific section name.

``sh_type`` - Serves as a layer that gives you more insight on the semantical meaning of the section. See ``man elf`` for more information on the section types.

``sh_flags`` - Flags that serve as extra information regarding the section, such as if it's writable, readable, if it contains instructions, et cetera.

``sh_addr`` - If the section is loaded into memory, this field is populated with the entry address.

``sh_offset`` - Offset in bytes from the beginning of the file to the current section in memory.

``sh_size`` - Size of the section.

``sh_link`` - Holds the link to the section table index, the interpretation of how this is handled depends on the type of the section.

``sh_addralign`` - This is the byte alignment of constrainted section address alignments. Goes in increments of the powers of 2 and needs to be congruent to 0.

``sh_entsize`` - Size of the current section table entry.

# Symbol table:
The symbol table is an array of structures that look like:
```c
typedef struct {
        uint32_t      st_name;
        unsigned char st_info;
        unsigned char st_other;
        uint16_t      st_shndx;
        Elf64_Addr    st_value;
        uint64_t      st_size;
} Elf64_Sym;
```
In the programs memory it looks like:
```
Symbol table '.symtab' contains 41 entries:
Num:    Value          Size Type    Bind   Vis      Ndx Name
0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS abi-note.c
2: 000000000000039c    32 OBJECT  LOCAL  DEFAULT    4 __abi_tag
3: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS init.c
4: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
5: 0000000000001070     0 FUNC    LOCAL  DEFAULT   14 deregister_tm_clones
6: 00000000000010a0     0 FUNC    LOCAL  DEFAULT   14 register_tm_clones
7: 00000000000010e0     0 FUNC    LOCAL  DEFAULT   14 __do_global_dtors_aux
8: 0000000000004030     1 OBJECT  LOCAL  DEFAULT   25 completed.0
9: 0000000000003df0     0 OBJECT  LOCAL  DEFAULT   20 __do_global_dtor[...]
10: 0000000000001130     0 FUNC    LOCAL  DEFAULT   14 frame_dummy
11: 0000000000003de8     0 OBJECT  LOCAL  DEFAULT   19 __frame_dummy_in[...]
12: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS test_bin.c
13: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
14: 000000000000210c     0 OBJECT  LOCAL  DEFAULT   18 __FRAME_END__
15: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS 
16: 0000000000003df0     0 NOTYPE  LOCAL  DEFAULT   19 __init_array_end
17: 0000000000003df8     0 OBJECT  LOCAL  DEFAULT   21 _DYNAMIC
18: 0000000000003de8     0 NOTYPE  LOCAL  DEFAULT   19 __init_array_start
19: 000000000000200c     0 NOTYPE  LOCAL  DEFAULT   17 __GNU_EH_FRAME_HDR
20: 0000000000004000     0 OBJECT  LOCAL  DEFAULT   23 _GLOBAL_OFFSET_TABLE_
21: 0000000000001000     0 FUNC    LOCAL  DEFAULT   12 _init
22: 00000000000011d0     5 FUNC    GLOBAL DEFAULT   14 __libc_csu_fini
23: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterT[...]
24: 0000000000004020     0 NOTYPE  WEAK   DEFAULT   24 data_start
25: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND puts@GLIBC_2.2.5
26: 0000000000004030     0 NOTYPE  GLOBAL DEFAULT   24 _edata
27: 00000000000011d8     0 FUNC    GLOBAL HIDDEN    15 _fini
28: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_mai[...]
29: 0000000000004020     0 NOTYPE  GLOBAL DEFAULT   24 __data_start
30: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
31: 0000000000004028     0 OBJECT  GLOBAL HIDDEN    24 __dso_handle
32: 0000000000002000     4 OBJECT  GLOBAL DEFAULT   16 _IO_stdin_used
33: 0000000000001160   101 FUNC    GLOBAL DEFAULT   14 __libc_csu_init
34: 0000000000004038     0 NOTYPE  GLOBAL DEFAULT   25 _end
35: 0000000000001040    47 FUNC    GLOBAL DEFAULT   14 _start
36: 0000000000004030     0 NOTYPE  GLOBAL DEFAULT   25 __bss_start
37: 0000000000001139    38 FUNC    GLOBAL DEFAULT   14 main
38: 0000000000004030     0 OBJECT  GLOBAL HIDDEN    24 __TMC_END__
39: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMC[...]
40: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@G[...]
```

``Elf64_Sym`` field lingustic definitions:

``st_name`` - The index into the symbol string table the name resides.

``st_value`` - Value given to the symbol.

``st_size`` - Size of the symbol. Can be 0 or "unknown".

``st_info`` - Gives specifications regarding the type symbols that were resolved and defined, as well as the binding typing.

``st_other`` - Gives specifcations for the symbol definition scope aka the visibility.

``st_shndx`` - Offset between a symbol and another that's addressed relevant to the section header table index.

# The end!
If there are any typos or issues with this repository, please let me know in the form of writing an issue! 
