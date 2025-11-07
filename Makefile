CC			= cc

CFLAGS		= 

SRCS		=	main.c \
				get_flags.c \
				algorithm/md5/ft_ssl_md5.c

NAME		=	ft_ssl

OBJS		=	$(addprefix .build/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

-include $(OBJS:.o=.d)

.phony: all clean fclean re

