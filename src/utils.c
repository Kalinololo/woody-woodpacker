#include "wooody.h"

char    *map_file(char *file, size_t *size)
{
    int s;
    int fd = open(file, O_RDWR);
    if (fd == -1)
        error(strerror(errno));
    s = lseek(fd, 0, SEEK_END);
    if (s == -1)
        close(fd), error(strerror(errno));
    lseek(fd, 0, SEEK_SET);
    char *map = (char *) mmap(NULL, s, PROT_READ | PROT_WRITE | PROT_EXEC , MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED)
        error(strerror(errno));
    *size = s;
    return map;
}

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