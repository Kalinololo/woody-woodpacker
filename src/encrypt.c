#include "wooody.h"

char encryption_key[KEY_SIZE + 1];

char *generate_key()
{
    char *key;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
        error(strerror(errno));
    key = malloc(KEY_SIZE + 1);
    if (!key)
        close(fd), error(strerror(errno));
    key[KEY_SIZE] = '\0';
    encryption_key[KEY_SIZE] = '\0';
    int i = 0;
    while (i < KEY_SIZE)
    {
        if (read(fd, key + i, 1) == -1)
            close(fd), error(strerror(errno));
        if ((key[i] > '0' && key[i] < '9') || (key[i] > 'a' && key[i] < 'z') || (key[i] > 'A' && key[i] < 'Z'))
        {
            encryption_key[i] = *(key + i);
            i++;
        }
    }
    printf("Key : %s\n", key);
    close(fd);
    return key;
}

void    encryption(woody *w)
{
    char *key = generate_key();
    char *ckey = key;
    size_t i = 0;
    while (i < w->text->sh_size)
    {
        if (!*key)
            key = ckey;
        *(w->file + w->text->sh_offset + i) ^= *(key++);
        i++;
    }
}