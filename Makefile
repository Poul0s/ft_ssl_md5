CC			= cc

CFLAGS		= 

SRCS		=	main.c \
				get_flags.c \
				algorithm/md5/ft_ssl_md5.c

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
	rm -rf $(OBJS)

fclean: clean
	make -C $(LFT_DIR) fclean
	rm -rf $(NAME)

re: fclean all

-include $(OBJS:.o=.d)

.phony: all clean fclean re

