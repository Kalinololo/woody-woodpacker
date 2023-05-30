NAME= woody_woodpacker
FNAME=woody

CFLAGS=-Wall -Wextra -Werror
SRCS= src/main.c src/encrypt.c 

OBJ=$(SRCS:.c=.o)

all:$(NAME)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(NAME):$(OBJ)
	gcc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(FNAME)

re: fclean all