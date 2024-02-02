CC		= cc

CFLAGS		= -Wall -Wextra -Werror

FT_DIR		= ./libft

CFILES		= main.c utils.c

CFILES_B	= 

OFILES		= $(CFILES:.c=.o)

OFILES_B	= $(CFILES_B:.c=.o)

NAME		= pipex

NAME_B		= pipex_bonus

LFLAGS 		= -L$(FT_DIR) -lft

all: $(NAME)

bonus: $(NAME_B)

$(NAME): $(OFILES)
	make -C $(FT_DIR)
	$(CC) $(OFILES) -o $(NAME) $(LFLAGS)

$(NAME_B): $(OBJ_B)
	make -C $(FT_DIR)
	$(CC) $(OFILES_B) -o $(NAME_B) $(LFLAGS)

$(OFILES): %.o: %.c pipex.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OFILES_B): %.o: %.c pipex_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make clean -C $(FT_DIR)
	rm -f $(OFILES) $(OFILES_B)

fclean: clean
	make fclean -C $(FT_DIR)
	rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY : all bonus clean fclean re
