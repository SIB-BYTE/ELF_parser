#include "../includes/elf.h"

#include <elf.h>
#include <unistd.h>

// Returns the size of an ELF:
size_t get_elf_size(char *file_name)
{
	struct stat file_stats = {0};
	return (stat(file_name, &file_stats) == -1 ? -1 : file_stats.st_size);
}

// Checks the magic bytes and validates if the binary is an ELF file format and returns accordingly:
bool validate_elf(elf_parser_t *parser)
{
	if(parser->elf_header->e_ident[EI_MAG0] != EI_MAG0 ||
	   parser->elf_header->e_ident[EI_MAG1] != EI_MAG1 ||
	   parser->elf_header->e_ident[EI_MAG2] != EI_MAG2 ||
	   parser->elf_header->e_ident[EI_MAG3] != EI_MAG3) { return (false); }

	return (true);
}

// ELF header flag field binding:
const char *dump_elf_flag(int flags)
{
	switch(flags)
	{
		case 0:			    return ("0x0");
		case EF_ARM_RELEXEC:	    return ("RELEXEC");
		case EF_ARM_HASENTRY:	    return ("HASENTRY");
		case EF_ARM_INTERWORK:	    return ("INTERWORK");
		case EF_ARM_APCS_26:	    return ("APCS_26");
		case EF_ARM_APCS_FLOAT:	    return ("APCS FLOAT");
		case EF_ARM_PIC:	    return ("PIC");
		case EF_ARM_ALIGN8:	    return ("ALIGN8");
		case EF_ARM_OLD_ABI:        return ("OLD ABI");
		case EF_ARM_NEW_ABI:	    return ("NEW ABI");
		case EF_ARM_SOFT_FLOAT:     return ("SOFT FLOAT");
		case EF_ARM_MAVERICK_FLOAT: return ("MAVERICK FLOAT");
		default:		    return ("Invalid flag!");
	}
}

// Program header type field binding:
const char *dump_program_type(int type)
{
	switch(type)
	{
		case PT_NULL:	      return ("PT_NULL");
		case PT_LOAD:	      return ("PT_LOAD");
		case PT_DYNAMIC:      return ("PT_DYNAMIC");
		case PT_INTERP:	      return ("PT_INTERP");
		case PT_NOTE:	      return ("PT_NOTE");
		case PT_SHLIB:	      return ("PT_SHLIB");
		case PT_PHDR:	      return ("PT_PHDR");
		case PT_LOPROC:	      return ("PT_LOCPROC");
		case PT_HIPROC:	      return ("PT_HIPROC");
		case PT_TLS: 	      return ("PT_TLS");
		case PT_NUM:	      return ("PT_NUM");
		case PT_LOOS:	      return ("PT_LOOS");
		case PT_SUNWBSS:      return ("PT_SUNWBSS");
		case PT_HISUNW:	      return ("PT_HISUNW");
		case PT_SUNWSTACK:    return ("PT_SUNWSTACK");
		case PT_GNU_STACK:    return ("PT_GNU_STACK");
		case PT_GNU_RELRO:    return ("PT_GNU_RELRO");
		case PT_GNU_EH_FRAME: return ("PT_GNU_EH_FRAME");
		case PT_GNU_PROPERTY: return ("PT_GNU_PROPERTY");
		default:	      return ("Invalid flag!");
	}
}

// Program header flag field binding:
const char *dump_program_flag(int flag)
{
	switch(flag)
	{
		case PF_X:	  return ("Exec only");
		case PF_W:	  return ("Write only");
		case PF_R:	  return ("Read only");
		case PF_X + PF_W: return ("Exec + write only");
		case PF_X + PF_R: return ("Exec + read only");
		case PF_W + PF_R: return ("Write + read only");
		default:	  return ("Invalid permission(s)!");
	}
}

// Section header type field binding:
const char *dump_section_type(int type)
{
	switch(type)
	{
		case SHT_REL:		 return ("SHT_REL");
		case SHT_NULL:		 return ("SHT_NULL");
		case SHT_RELA:		 return ("SHT_RELA");
		case SHT_HASH:		 return ("SHT_HASH");
		case SHT_NOTE:		 return ("SHT_NOTE");
		case SHT_SHLIB:		 return ("SHT_SHLIB");
		case SHT_GROUP:	         return ("SHT_GROUP");
		case SHT_SYMTAB:	 return ("SHT_SYMTAB");
		case SHT_STRTAB:	 return ("SHT_STRTAB");
		case SHT_NOBITS:	 return ("SHT_NOBITS");
		case SHT_DYNSYM:	 return ("SHT_DYNSYM");
		case SHT_LOPROC:	 return ("SHT_LOPROC");
		case SHT_HIPROC:	 return ("SHT_HIPROC");
		case SHT_LOUSER:	 return ("SHT_LOUSER");
		case SHT_HIUSER:	 return ("SHT_HIUSER");
		case SHT_DYNAMIC:        return ("SHT_DYNAMIC");
		case SHT_PROGBITS:	 return ("SHT_PROGBITS");
		case SHT_CHECKSUM:	 return ("SHT_CHECKSUM");
		case SHT_GNU_HASH:	 return ("SHT_GNU_HASH");
		case SHT_FINI_ARRAY:     return ("SHT_FINI_ARRAY");
		case SHT_INIT_ARRAY:     return ("SHT_INIT_ARRAY");
		case SHT_GNU_verdef:	 return ("SHT_GNU_verdef");
		case SHT_GNU_versym:	 return ("SHT_GNU_versym");
		case SHT_GNU_verneed:    return ("SHT_GNU_verneed");
		case SHT_SYMTAB_SHNDX:   return ("SHT_SYMTAB_SHNDX");
		case SHT_PREINIT_ARRAY:  return ("SHT_PREINIT_ARRAY");
		case SHT_GNU_ATTRIBUTES: return ("SHT_GNU_ATTRIBUTES");
		default:		 return ("Invalid type!");
	}
}

// Section header flag field binding:
char *dump_section_flag(unsigned long flag)
{
	switch(flag)
	{
		case 0:			           return ("0");
		case SHF_TLS:			   return ("SHF_TLS");
		case SHF_GROUP:			   return ("SHF_GROUP");
		case SHF_WRITE:			   return ("SHF_WRITE");
		case SHF_ALLOC:			   return ("SHF_ALLOC");
		case SHF_MERGE:			   return ("SHF_MERGE");
		case SHF_STRINGS:	           return ("SHF_STRINGS");
		case SHF_MASKPROC:		   return ("SHF_MASKPROC");
		case SHF_INFO_LINK:		   return ("SHF_INO_LINK");
		case SHF_EXECINSTR:		   return ("SHF_EXECINSTR");
		case SHF_WRITE + SHF_ALLOC:	   return ("SHF_WRITE + SHF_ALLOC");
		case SHF_MERGE + SHF_STRINGS:      return ("SHF_MERGE + SHF_STRINGS");
		case SHF_WRITE + SHF_MASKPROC:     return ("SHF_WRITE + SHF_MASKPROC");
		case SHF_ALLOC + SHF_MASKPROC:     return ("SHF_ALLOC + SHF_MASKPROC");
		case SHF_ALLOC + SHF_EXECINSTR:    return ("SHF_ALLOC + SHF_EXECINSTR");
		case SHF_WRITE + SHF_EXECINSTR:    return ("SHF_WRITE + SHF_EXECINSTR");
		case SHF_ALLOC + SHF_INFO_LINK:    return ("SHF_ALLOC + SHF_INFO_LINK");
		case SHF_MASKPROC + SHF_EXECINSTR: return ("SHF_MASKPROC + SHF_EXECINSTR");
		default:			   return ("Invalid flag!");
	}
}

// Symbol table type field binding:
const char *dump_symbol_type(unsigned char type)
{
	switch(ELF64_ST_TYPE(type))
	{
		case STT_FUNC:    return ("STT_FUNC");
		case STT_FILE:	  return ("STT_FILE");
		case STT_LOOS:    return ("STT_LOOS");
		case STT_OBJECT:  return ("STT_OBJECT");
		case STT_NOTYPE:  return ("STT_NOTYPE");
		case STT_LOPROC:  return ("STT_LOPROC");
		case STT_HIPROC:  return ("STT_HIPROC");
		default:	  return ("Invalid type!");
	}
}

// Symbol table info field binding:
const char *dump_symbol_bind(unsigned char binding)
{
	switch(ELF64_ST_BIND(binding))
	{
		case STB_WEAK:   return ("STB_WEAK");
		case STB_LOOS:   return ("STB_LOOS");
		case STB_LOCAL:  return ("STB_LOCAL");
		case STB_GLOBAL: return ("STB_GLOBAL");
		case STB_LOPROC: return ("STB_LOPROC");
		case STB_HIPROC: return ("STB_HIPROC");
		default:	 return ("Invalid binding!");
	}
}

// Symbol table other field bindings:
const char *dump_symbol_visibility(unsigned char vision)
{
	switch(ELF64_ST_VISIBILITY(vision))
	{
		case STV_HIDDEN:    return ("STV_HIDDEN");
		case STV_DEFAULT:   return ("STV_DEFAULT");
		case STV_INTERNAL:  return ("STV_INTERNAL");
		case STV_PROTECTED: return ("STV_PROTECTED");
		default:	    return ("Invalid vision!");
	}
}

