CC			= cc

CFLAGS		= -Wall -Wextra -Werror -MMD

SRCS		=	main.c \
				get_flags.c \
				algorithm/tools.c \
				algorithm/md5/ft_ssl_md5.c \
				algorithm/sha2/ft_ssl_sha2.c \

NAME		=	ft_ssl

OBJS		=	$(addprefix .build/, $(SRCS:.c=.o))

LFT_DIR		=	libs/libft

LFT			=	$(LFT_DIR)/libft.a

LIBS		=	-L$(LFT_DIR) -lft

all: $(NAME)

$(LFT):
	make -C $(LFT_DIR)

$(NAME): $(LFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

.build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Ilibs -c $< -o $@

clean:
	rm -rf .build

fclean: clean
	make -C $(LFT_DIR) fclean
	rm -rf $(NAME)

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re

