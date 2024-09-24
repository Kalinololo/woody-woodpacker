#include "wooody.h"

void patch(woody *w)
{
    int e = (int) w->header->e_entry;
    char *f = w->p->file + w->p->text->sh_offset;
    // Offsets correspondant a la position des differentes clé dans le payload en asm
    int key_offset = 31;
    int addr_offset = 41;
    int size_offset = 51;
    int entry_offset = 81;
    /*long unsigned int i = 0;
    while (i < w->p->text->sh_size)
    {
        printf("index : %ld value --> char : %c  int : %d  hexa : %X\n", i, *(f + i), *(f + i), *(f + i));
        i++;
    }*/

    ft_memcpy(f + key_offset, w->key, KEY_SIZE);
    ft_memcpy(f + addr_offset, &w->text->sh_addr, sizeof(uint64_t));
    ft_memcpy(f + size_offset, &w->text->sh_size, sizeof(uint64_t));
    ft_memcpy(f + entry_offset, &e, sizeof(int));
}

void   inject(woody *w)
{
    w->psize = w->p->text->sh_size;
    Elf64_Off injection_offset = (w->load->p_offset + w->load->p_filesz);

    ft_memcpy(w->file + injection_offset, w->p->file + w->p->text->sh_offset, w->psize);
    w->header->e_entry = (w->load->p_vaddr + w->load->p_filesz);
    w->load->p_filesz += w->psize;
    w->load->p_memsz += w->psize;
    //w->load->p_flags |= PF_W;
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