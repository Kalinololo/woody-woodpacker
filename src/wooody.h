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
#define PAYLOAD "src/payload"

typedef struct s_woody
{
    char        	*file;
    size_t      	size;
    Elf64_Ehdr  	*header;
    Elf64_Shdr  	*text;
	Elf64_Phdr  	*load;
    int         	load_index;
    int         	new;
    size_t          psize;
    char            *key;
    struct s_woody	*p;
}               woody;

void		encryption(woody *w);
char        *map_file(char *file, size_t *size);
void		error(char *s);
void		inject(woody *w);
void		parse_elf(woody *w);
int			check_elf(char *c);
Elf64_Shdr	*get_elf_section(char *file, char *seg);
Elf64_Phdr	*get_load_segment(woody *w, int *space);
void        ft_memcpy(void *dst, void *src, int size);

#endif