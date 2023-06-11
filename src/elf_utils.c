#include "wooody.h"

Elf64_Shdr   *get_elf_section(char *file, char *seg)
{
    Elf64_Ehdr *header = (Elf64_Ehdr *) file;
    Elf64_Shdr *sects = (Elf64_Shdr *) (file + header->e_shoff);
    char *shstrtab = file + (&sects[header->e_shstrndx])->sh_offset;

    for (size_t i = 0; i < header->e_shnum; i++)
    {
        if (!strcmp(shstrtab + sects[i].sh_name, seg))
            return (&sects[i]);
    }
    return NULL;
}

int check_load_seg(Elf64_Ehdr *header, Elf64_Phdr *seg, int i)
{
    if (i >= header->e_phnum || seg == NULL || seg->p_type != PT_LOAD)
        return (1);
    return (0);
}

Elf64_Phdr   *get_load_segment(char *file)
{
    Elf64_Ehdr *header = (Elf64_Ehdr *) file;
    int i, j;
    Elf64_Phdr *seg = (Elf64_Phdr *) (file + header->e_phoff);

    i = -1;
    while (++i < header->e_phnum)
        if (seg[i].p_type == PT_LOAD)
            break ;
    j = i;
    while (++j < header->e_phnum)
        if (seg[j].p_type == PT_LOAD)
            break ;
    if (check_load_seg(header, &seg[i], i) || check_load_seg(header, &seg[j], j))
        error("ELF Format error (PT_LOAD segments missing)");
    if ((unsigned long)(seg[j].p_offset - (seg[i].p_offset + seg[i].p_filesz)) < sizeof(PAYLOAD) - 1)
        return (NULL);
    return &seg[i];
}

void parse_elf(woody* w)
{
    if (check_elf(w->file))
        error("File architecture not suported. x86_64 only");
    w->header = (Elf64_Ehdr *) w->file;
    w->text = get_elf_section(w->file, ".text");
}

int check_elf(char *c)
{
    if (c[0] == (char)127 && c[1] == 'E' && c[2] == 'L' && c[3] == 'F' && c[4] == 2)
        return (0);
    return (1);
}