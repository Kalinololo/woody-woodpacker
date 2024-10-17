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
    if (i > header->e_phnum || seg == NULL || seg->p_type != PT_LOAD)
        return (1);
    return (0);
}

Elf64_Phdr   *get_load_segment(woody *w, int *space)
{
    Elf64_Ehdr *header = (Elf64_Ehdr *) w->file;
    int i;
    Elf64_Phdr *seg = (Elf64_Phdr *) (w->file + header->e_phoff);
    Elf64_Phdr *first = NULL;

    i = -1;
    while (++i < header->e_phnum)
        if (seg[i].p_type == PT_LOAD && (seg[i].p_flags & PF_X))
            break ;
    first = &seg[i];
    if (check_load_seg(header, first, i) || check_load_seg(header, first + 1, i + 1))
        return (NULL);
    w->load_index = i;
    uint64_t size = w->p->text->sh_size;
    uint64_t space_size = seg[i + 1].p_offset - (seg[i].p_offset + seg[i].p_filesz);
    if (space_size >= size)
        *space = 1;
    return first;
}

void parse_elf(woody* w)
{
    if (check_elf(w->file))
        error("File architecture not suported. x86_64 only");
    w->header = (Elf64_Ehdr *) w->file;
    w->text = get_elf_section(w->file, ".text");
    //w->text->sh_flags |= SHF_WRITE;
}

int check_elf(char *c)
{
    if (c[0] == (char)127 && c[1] == 'E' && c[2] == 'L' && c[3] == 'F' && c[4] == 2)
        return (0);
    return (1);
}

int    get_load(woody *w)
{
    int     space = 0;
    w->load_index = 0;
    w->load = get_load_segment(w, &space);
    if (w->load == NULL)
        error("Wrong ELF format.");
    if (space)
        return (0);
    write(1, "1", 1);
    enlarge_load_size(w);
    parse_elf(w);
    return (1 + get_load(w));
}