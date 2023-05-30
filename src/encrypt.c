#include "wooody.h"


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
    int i = 0;
    while (i < KEY_SIZE)
    {
        if (read(fd, key + i, 1) == -1)
            close(fd), error(strerror(errno));
        if ((key[i] > '0' && key[i] < '9') || (key[i] > 'a' && key[i] < 'z') || (key[i] > 'A' && key[i] < 'Z'))
            i++;
    }
    printf("Key : %s\n", key);
    close(fd);
    return key;
}

char *encryption(char *text)
{
    printf("%s\n", text);
    char *key = generate_key();
    char *ckey = key;
    char *new = strdup(text);
    //free(text);
    if (!new)
        error(strerror(errno));
    int i = 0;
    while (new[i])
    {
        if (!*key)
            key = ckey;
        new[i] ^= *(key++);
        i++;
    }
    printf("%s\n", new);
    //free(key);
    return new;
}