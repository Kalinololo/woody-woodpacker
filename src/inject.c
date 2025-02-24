#include "wooody.h"

/*void patch(woody *w)
{
    int new_entry = (w->load->p_vaddr + w->load->p_filesz);
    int e = (int) w->header->e_entry;

    int entry_offset = 69;
    int key_offset = 113;
    int addr_offset = 122;
    int size_offset = 130;

    uint64_t size = ((w->load->p_offset + w->load->p_filesz) - w->text->sh_offset);
    uint64_t size_end = (size + w->text->sh_size);

    int entrypoint = e - (new_entry + 1 + sizeof(int));
    char *f = w->p->file + w->p->text->sh_offset;

    long unsigned int i = 0;
    while (i < w->p->text->sh_size)
    {
        printf("index : %ld value --> char : %c  int : %d  hexa : %X\n", i, *(f + i), *(f + i), *(f + i));
        i++;
    }

    ft_memcpy(f + entry_offset, &entrypoint, sizeof(int));
    ft_memcpy(f + key_offset, w->key, KEY_SIZE);
    ft_memcpy(f + addr_offset, &size, sizeof(uint64_t));
    ft_memcpy(f + size_offset, &size_end, sizeof(uint64_t));
}*/

void patch(woody *w)
{

    int size_offset = 123;
    int addr_off = 115;
    int key_offset = 131;

    uint64_t addr = ((w->load->p_offset + w->load->p_filesz) - w->text->sh_offset);
    uint64_t size_end = (w->text->sh_size);

    char *f = w->p->file + w->p->text->sh_offset;

    long unsigned int i = 0;
    while (i < w->p->text->sh_size)
    {
        printf("index : %ld value --> char : %c  int : %d  hexa : %X\n", i, *(f + i), *(f + i), *(f + i));
        i++;
    }

    ft_memcpy(f + size_offset, &size_end, sizeof(uint64_t));
    ft_memcpy(f + addr_off, &addr, sizeof(uint64_t));
    ft_memcpy(f + key_offset, w->key, KEY_SIZE);
}

void   inject(woody *w)
{
    w->psize = w->p->text->sh_size;
    Elf64_Off injection_offset = (w->load->p_offset + w->load->p_filesz);

    ft_memcpy(w->file + injection_offset, w->p->file + w->p->text->sh_offset, w->psize);
    w->header->e_entry = (w->load->p_vaddr + w->load->p_filesz);
    w->load->p_filesz += w->psize;
    w->load->p_memsz += w->psize;
    w->load->p_flags |= PF_W;
    w->load->p_flags |= PF_X;
}

// Fonction pas testée et peut-etre pas necessaire
// En theorie, c'est pour augmenter la taille de la section si jamais il y en a pas assez de base

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