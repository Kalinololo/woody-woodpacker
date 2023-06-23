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

void    ft_memset(void *dst, int value, int size)
{
    int i = 0;
    char *d = (char *) dst;
    while (i < size)
    {
        *(d + i) = value;
        i++;
    }
}

void    enlarge_load_size(woody *w)
{
    size_t s = w->psize;
    char *new = malloc(w->size + s);
    Elf64_Phdr *ph = (Elf64_Phdr *) (w->file + w->header->e_phoff);
    Elf64_Shdr *sh = (Elf64_Shdr *) (w->file + w->header->e_shoff);
    int i;

    //Add s to the Phdr offset after our codecave
    i = w->load_index + 1;
    while (i < w->header->e_phnum)
        ph[i++].p_offset += s;
    //Add s to the Shdr offset after our codecave
    i = 0;
    while (i < w->header->e_shnum)
        if (sh[i++].sh_offset > w->load->p_offset + w->load->p_filesz)
            break ;
    while (i < w->header->e_shnum)
        sh[i++].sh_offset += s;
    //Add s to the header offset
    if (w->header->e_phoff > w->load->p_offset + w->load->p_filesz)
        w->header->e_phoff += s;
    if (w->header->e_shoff > w->load->p_offset + w->load->p_filesz)
        w->header->e_shoff += s;
    //Copy the orignal file with the codecave
    ft_memcpy(new, w->file, w->load->p_offset + w->load->p_filesz);
    ft_memset(new + w->load->p_offset + w->load->p_filesz, 0, s);
    ft_memcpy(new + w->load->p_offset + w->load->p_filesz + s, w->file + w->load->p_offset + w->load->p_filesz, w->size - (w->load->p_offset + w->load->p_filesz));
    munmap(w->file, w->size);
    w->file = new;
    w->size += s;
}

int    check_space(woody *w)
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
    return (1 + check_space(w));
}

void   inject(woody *w)
{
    w->psize = w->p->text->sh_size;
    w->new = check_space(w);
    Elf64_Off injection_offset = (w->load->p_offset + w->load->p_filesz);

    ft_memcpy(w->file + injection_offset, w->p->file + w->p->text->sh_offset, w->psize);
    w->header->e_entry = (w->load->p_vaddr + w->load->p_filesz);
    w->load->p_filesz += w->psize;
    w->load->p_memsz += w->psize;
}