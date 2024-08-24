############################ Compilation ############################
CC = cc
CFLAGS = -Wall -Wextra -Werror -g


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
SRCFILES =	\
		builtin_cd_pwd.c builtin_echo.c builtin_exit.c \
		builtin_env.c builtin_env_helper.c builtin_env_helper2.c builtin_env_helper3.c \
		token_linkedlist.c token_linkedlist2.c token_linkedlist3.c \
		token_linkedlist4.c \
		check_file_status.c \
		printerror.c \
		expand_shell_var.c \
		parsing.c \
		piping.c \
		redirection.c redirection2.c redirection3.c redirection4.c \
		execute.c execute2.c execute3.c execute4.c execute5.c execute6.c \
		main.c mainhelper.c mainhelper2.c \

SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJS = $(addprefix $(OBJDIR), $(SRCFILES:.c=.o))

############################### RULES ###############################
all: $(NAME)

debug: $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) -g $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

valgrind : debug
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp --track-fds=yes --trace-children=yes --track-origins=yes ./$(NAME)

valgrindlite : debug
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp --track-origins=yes ./$(NAME)

tester : re
	cd ./minishell_tester && ./tester && cd ..

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
