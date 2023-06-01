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
    Elf64_Shdr  *sects;
    Elf64_Shdr  text;
    char        *shstrtab;
}               woody;


void    encryption(woody *w);
void    error(char *s);