#include "wooody.h"

void error(char *s)
{
    write(2, s, strlen(s));
    exit(1);
}

void create_new(woody w)
{   
    int new = open("woody", O_CREAT | O_TRUNC | O_RDWR, 0777);
    if (new == -1)
        error(strerror(errno));
    if (write(new, w.file, w.size) == -1)
        error(strerror(errno));
    if (w.new)
        free(w.file);
    else
        munmap(w.file, w.size);
}


void patch(woody *w)
{
    char *f = w->p->file + w->p->text->sh_offset;
    int key_offset = 88;
    int addr_offset = 96;
    int size_offset = 104;

    ft_memcpy(f + key_offset, w->key, KEY_SIZE);
    ft_memcpy(f + addr_offset, &w->text->sh_addr, sizeof(Elf64_Addr));
    ft_memcpy(f + size_offset, &w->text->sh_size, sizeof(uint64_t));
    long unsigned int i = 0;
    while (i < w->text->sh_size)
    {
        printf("index : %ld value --> char : %c  int : %d  hexa : %X\n", i, *(f + i), *(f + i), *(f + i));
        i++;
    }
}

int main(int ac, char **av)
{
    if (ac != 2)
        error("Usage : ./woody_woodpacker [FILE]");
    woody w, p;
    w.new = 0;
    w.p = &p;
    w.file = map_file(av[1], &w.size);
    parse_elf(&w);
    w.p->file = map_file(PAYLOAD, &w.p->size);
    parse_elf(w.p);
    encryption(&w);
    patch(&w);
    inject(&w);
    create_new(w);
    return 0;
}