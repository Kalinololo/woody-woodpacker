#include "wooody.h"

void    ft_memcpy(void *dst, void *src, int size)
{
    int i = 0;
    char *d = (char *) dst;
    char *s = (char *) src;
    while (i < size)
    {
        *(d + i) = s[i];
        i++;
    }
}

void    enlarge_load_size(woody *w)
{
    size_t s = sizeof(PAYLOAD) - 1;
    char *new = malloc(w->size + s);
    Elf64_Phdr *ph = (Elf64_Phdr *) (w->file + w->header->e_phoff);
    Elf64_Shdr *sh = (Elf64_Shdr *) (w->file + w->header->e_shoff);
    int i = 0;

    while (i < w->header->e_phnum)
        if (ph[i++].p_type == PT_LOAD)
            break ;
    while (i < w->header->e_phnum)
        ph[i++].p_offset += s;
    i = -1;
    write(1, "1", 1);
    while (++i < w->header->e_shnum)
        if (sh[i].sh_offset > w->load->p_offset + w->load->p_filesz)
            break ;
    write(1, "1", 1);
    while (i < w->header->e_shnum)
        sh[i++].sh_offset += s;
    if (w->header->e_phoff > w->load->p_offset + w->load->p_filesz)
        w->header->e_phoff += s;
    if (w->header->e_shoff > w->load->p_offset + w->load->p_filesz)
        w->header->e_shoff += s;
    write(1, "1", 1);
    ft_memcpy(new, w->file, w->load->p_offset + w->load->p_filesz);
    ft_memcpy(new + w->load->p_offset + w->load->p_filesz, PAYLOAD, s);
    ft_memcpy(new + w->load->p_offset + w->load->p_filesz + s, w->file + w->load->p_offset + w->load->p_filesz, w->size - (w->load->p_offset + w->load->p_filesz));
    w->load->p_memsz += s;
    w->load->p_filesz += s;
    munmap(w->file, w->size);
    w->file = new;
    w->size += s;
}

int    check_space(woody *w)
{
    w->load = get_load_segment(w->file);
    if (w->load != NULL)
        return (0);
    enlarge_load_size(w);
    return (1);
}

void   inject(woody *w)
{
    char p[] = PAYLOAD;
    size_t s = sizeof(PAYLOAD) - 1;
    if (check_space(w))
        return ;
    Elf64_Off injection_offset = (w->load->p_offset + w->load->p_filesz);

    ft_memcpy(w->file + injection_offset, p, s);
    w->header->e_entry = (w->load->p_vaddr + w->load->p_filesz);
    w->load->p_memsz += s;
    w->load->p_filesz += s;
}