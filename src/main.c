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
    munmap(w.file, w.size);
}

int main(int ac, char **av)
{
    if (ac != 2)
        error("Usage : ./woody_woodpacker [FILE]");
    woody w;
    w.file = map_file(av[1], &w.size);
    w = parse_elf(w);
    //w = encryption(w);
    w = inject(w);
    create_new(w);
    return 0;
}