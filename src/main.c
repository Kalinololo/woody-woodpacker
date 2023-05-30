#include "wooody.h"

void error(char *s)
{
    write(2, s, strlen(s));
    exit(1);
}

char *get_file(char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
        error(strerror(errno));
    int size = lseek(fd, 0, SEEK_END);
    if (size == -1)
        close(fd), error(strerror(errno));
    lseek(fd, 0, SEEK_SET);
    char *content = (char *)malloc(sizeof(char) * (size + 1));
    content[size] = '\0';
    if (!content)
        close(fd), error(strerror(errno));
    if (read(fd, content, size) == -1)
        close(fd), error(strerror(errno));
    close(fd);
    return content;
}

int check_elf(char *c)
{
    if (c[0] == (char)127 && c[1] == 'E' && c[2] == 'L' && c[3] == 'F' && c[4] == 2)
        return (0);
    return (1);
}

void create_new(char *s)
{   
    (void) s;
    int new = open("woody", O_CREAT | O_TRUNC | O_RDWR, 0777);
    if (new == -1)
        error(strerror(errno));
    if (write(new, s, strlen(s)) == -1)
        error(strerror(errno));
}

int main(int ac, char **av)
{
    if (ac != 2)
        error("One arg expected");
    char *c = get_file(av[1]);
    if (check_elf(c))
        error("File architecture not suported. x86_64 only");
    c = encryption(c);
    create_new(c);
    return 0;
}