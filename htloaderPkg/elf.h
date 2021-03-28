#ifndef _ELF_H_
#define _ELF_H_

#include <stdint.h>

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef uint32_t Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Sxword;
typedef uint8_t  unsigned_char;

// File Header
typedef struct Elf64_Ehdr {
    unsigned_char e_ident[16];  // ELF identification
    Elf64_Half    e_type;       // Object file type
    Elf64_Half    e_machine;    // Machine type
    Elf64_Word    e_version;    // Object file version
    Elf64_Addr    e_entry;      // Entry point Address
    Elf64_Off     e_phoff;      // Program header offfset
    Elf64_Off     e_shoff;      // Section header offfset
    Elf64_Word    e_flags;      // Processor-specific flags
    Elf64_Half    e_ehsize;     // ELF header size
    Elf64_Half    e_phentsize;  // Size of program header entry
    Elf64_Half    e_phnum;      // Number of program header entries
    Elf64_Half    e_shentsize;  // Size of section header entry
    Elf64_Half    e_shnum;      // Number of section header entries
    Elf64_Half    e_shstrndx;   // Section name string table index
} __attribute__((packed)) Elf64_Ehdr;

// Program Header
typedef struct Elf64_Phdr {
    Elf64_Word  p_type;
    Elf64_Word  p_flags;
    Elf64_Off   p_offset;
    Elf64_Addr  p_vaddr;
    Elf64_Addr  p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
} __attribute__((packed)) Elf64_Phdr;

#define PT_LOAD 1

void elf64_load_segments(char *file);

#endif
