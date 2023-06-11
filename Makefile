NAME=woody_woodpacker
FNAME=woody

DNAME=src/payload/decrypt

CFLAGS=-Wall -Wextra -Werror

SRCS= src/main.c src/encrypt.c src/elf_utils.c src/utils.c

OBJ=$(SRCS:.c=.o)

all:$(NAME)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@ -DPAYLOAD=\"`/usr/bin/hexdump -v -e '"\\\x" 1/1 "%02x"' src/payload.o`\"

$(NAME):$(OBJ)
	gcc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(FNAME)

re: fclean all