#include "wooody.h"

char    *map_file(char *file, int *size)
{
    int fd = open(file, O_RDWR);
    if (fd == -1)
        error(strerror(errno));
    *size = lseek(fd, 0, SEEK_END);
    if (*size == -1)
        close(fd), error(strerror(errno));
    lseek(fd, 0, SEEK_SET);
    char *map = (char *) mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED)
        error(strerror(errno));
    return map;
}