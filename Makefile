############################ Compilation ############################
CC = cc
CFLAGS = -Wall -Wextra -Werror


############################ File Paths #############################
SRCDIR = src/
OBJDIR = obj/
INCLUDES = includes/

############################### Name ################################
NAME = minishell

############################# Libraries #############################
LIBFT_PATH = Libft
LIBFT_A = $(LIBFT_PATH)/libft.a

####################### SOURCE/OBJECT FILES #########################
SRCFILES = 	main.c 
SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJS = $(addprefix $(OBJDIR), $(SRCFILES:.c=.o))

############################### RULES ###############################
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

# Generate libft.a library with GNL (Rule: new)
$(LIBFT_A):
	make -C $(LIBFT_PATH) new

clean:
	make -C $(LIBFT_PATH) clean
	rm -rf $(OBJDIR)

fclean: clean
	make -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re:	fclean all

.phony: all clean fclean re new libft minishell

############################### EOF #################################