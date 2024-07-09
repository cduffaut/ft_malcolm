SRCS	= \
    program/ft_malcolm.c program/parsing.c \
    program/spoofing.c program/utils.c \
    program/sniffing.c

OBJS	= $(SRCS:.c=.o)

GCC		= gcc -Wall -Wextra -Werror

NAME	= ft_malcolm

LIBFT	= libft/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C libft

$(NAME): $(OBJS) $(LIBFT)
	$(GCC) -o $(NAME) $(OBJS) -Llibft -lft

.c.o:
	$(GCC) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: re fclean clean all
