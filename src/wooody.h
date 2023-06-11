#ifndef WOODY_H
# define WOODY_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <elf.h>

#define KEY_SIZE 16

typedef struct s_woody
{
    char        *file;
    int         size;
    Elf64_Ehdr  *header;
    Elf64_Shdr  *text;
}               woody;


woody		encryption(woody w);
char        *map_file(char *file, int *size);
void		error(char *s);
woody		inject(woody w);
woody		parse_elf(woody w);
int			check_elf(char *c);
Elf64_Shdr	*get_elf_section(char *file, char *seg);

#endif