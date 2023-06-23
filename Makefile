NAME=woody_woodpacker
FNAME=woody

DNAME=src/payload/decrypt

CFLAGS=-Wall -Wextra -Werror

SRCS= src/main.c src/encrypt.c src/elf_utils.c src/utils.c src/inject.c
ASRC=src/payload.s

OBJ=$(SRCS:.c=.o)
AOBJ=$(ASRC:.s=.o)

all:$(NAME)

%.o: %.s
	nasm -f elf64 $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(NAME):$(OBJ) $(AOBJ)
	ld -o src/payload $(AOBJ)
	gcc $(CFLAGS) $(OBJ) $(AOBJ) -o $(NAME)

clean:
	rm -f $(OBJ) $(AOBJ)

fclean: clean
	rm -f $(NAME) $(FNAME) src/payload

re: fclean all