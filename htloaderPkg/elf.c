#include <stdint.h>
#include "elf.h"
#include "mem.h"

void elf64_load_segment(char *file, Elf64_Phdr *phdr)
{
    Elf64_Word type = phdr->p_type;
    if (type != PT_LOAD) return;
    uint64_t offset = phdr->p_offset;
    uint64_t filesz = phdr->p_filesz;
    uint64_t *vaddr = (uint64_t *)phdr->p_vaddr;
    memcpy(vaddr, file + offset, filesz);
}

void elf64_load_segments(char *file)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file;
    int phnum = ehdr->e_phnum;
    uint64_t phoff = ehdr->e_phoff;
    for (int i = 0; i < phnum; i++) {
        Elf64_Phdr *phdr = (Elf64_Phdr *)(file + phoff) + i;
        elf64_load_segment(file, phdr);
    }
}
