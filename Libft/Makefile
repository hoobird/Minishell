CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = ft_isalnum.c ft_isalpha.c ft_isdigit.c ft_isascii.c ft_isprint.c \
ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c \
ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

SRCB  = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

SRCGNL = get_next_line_bonus.c get_next_line_utils_bonus.c

OBJ = $(SRC:.c=.o)
OBJB = $(SRCB:.c=.o)
OBJGNL = $(SRCGNL:.c=.o)

NAME = libft.a
NAMEGNL = gnl.a
NEW = libftgnl.a

new: $(OBJGNL) bonus
	ar rcs $(NEW) $(OBJ) $(OBJB) $(OBJGNL)

$(NAMEGNL) : $(OBJGNL) bonus
	ar rcs $(NAMEGNL) $(OBJGNL)

all:	$(NAME)

$(NAME):	$(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJGNL): $(SRCGNL)
	$(CC)  $(CFLAGS) -c $(SRCGNL)

bonus: $(OBJ) $(OBJB)
	ar rcs $(NAME) $(OBJ) $(OBJB)

clean:
	rm -f $(OBJ) $(OBJB) $(OBJGNL)
	
fclean:	clean
	rm -f $(NAME) $(NEW) $(NAMEGNL)

re:	fclean new
	
.PHONY: all fclean clean re 
