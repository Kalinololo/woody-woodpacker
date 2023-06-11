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
	Elf64_Phdr  *load;
    int         new;
}               woody;

void		encryption(woody *w);
char        *map_file(char *file, int *size);
void		error(char *s);
void		inject(woody *w);
void		parse_elf(woody *w);
int			check_elf(char *c);
Elf64_Shdr	*get_elf_section(char *file, char *seg);
Elf64_Phdr	*get_load_segment(char *file);

#endif