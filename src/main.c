#include "wooody.h"

void error(char *s)
{
    write(2, s, strlen(s));
    exit(1);
}

int check_elf(char *c)
{
    if (c[0] == (char)127 && c[1] == 'E' && c[2] == 'L' && c[3] == 'F' && c[4] == 2)
        return (0);
    return (1);
}

woody get_file(char *file)
{
    woody w;
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        error(strerror(errno));
    w.size = lseek(fd, 0, SEEK_END);
    if (w.size == -1)
        close(fd), error(strerror(errno));
    w.file = (char *) mmap(NULL, w.size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    if (w.file == MAP_FAILED)
        error(strerror(errno));
    return w;
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

void parse_elf(woody *w)
{
    w->header = (Elf64_Ehdr *) w->file;
    w->sects = (Elf64_Shdr *) (w->file + w->header->e_shoff);
    w->shstrtab = w->file + (&w->sects[w->header->e_shstrndx])->sh_offset;

    for (size_t i = 0; i < w->header->e_shnum; i++)
    {
        if (!strcmp(w->shstrtab + w->sects[i].sh_name, ".text"))
            w->text = w->sects[i];
    }
}

int main(int ac, char **av)
{
    if (ac != 2)
        error("Usage : ./woody_woodpacker [FILE]");
    woody w = get_file(av[1]);
    if (check_elf(w.file))
        error("File architecture not suported. x86_64 only");
    parse_elf(&w);
    encryption(&w);
    create_new(w);
    return 0;
}