SRCS    = \
	program/ft_malcolm.c program/parsing.c \
	program/spoofing.c program/utils.c \
	program/sniffing.c

OBJS	= $(SRCS:.c=.o)

GCC		= gcc -Wall -Wextra -Werror

NAME	= ft_malcolm

all:	$(NAME) libft

$(NAME): $(OBJS)
	$(GCC) -o $(NAME) $(OBJS) -Llibft -lft

libft:
	$(MAKE) -C libft

.c.o:
	$(GCC) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: re fclean clean all
