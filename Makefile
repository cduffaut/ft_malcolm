SRCS	= \
	program/ft_malcolm.c program/parsing.c

OBJS	= $(SRCS:.c=.o)

GCC	= gcc -Wall -Wextra -Werror

NAME	= ft_malcolm.a

all:	$(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

.c.o:
	$(GCC) -c $< -o $(<:.c=.o) 

clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY : re fclean clean all
