NAME= woody_woodpacker

CFLAGS=-Wall -Wextra -Werror
SRCS= main.c 

OBJ=$(SRCS:.c=.o)
OBJS=$(addprefix bin/, $(OBJ))

all:$(NAME)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $(addprefix bin/, $@)

$(NAME):$(OBJ)
	gcc $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all