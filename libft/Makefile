# Compiler and flags
CC := cc
CFLAGS := -Wall -Wextra -Werror

# Colors
COLOR_BLUE := \033[34m
COLOR_CYAN := \033[36m
COLOR_YELLOW := \033[33m
COLOR_RESET := \033[0m

NAME := libft.a

SRCS := ft_isquote.c ft_isspace.c ft_strndup.c ft_strdup.c \
		ft_strlen.c ft_memcpy.c ft_strjoin.c ft_strncmp.c \
		ft_isdigit.c ft_itoa.c ft_strjoin_free.c ft_strcmp.c \
		ft_strchr.c ft_split.c ft_strlcpy.c ft_free_arr.c \
		ft_memmove.c ft_get_next_line.c ft_strtoll.c ft_tolower.c \
		ft_isupper.c ft_stack.c ft_strlcat.c ft_realloc_arr.c \
		ft_remove_at.c

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Creating library $(COLOR_BLUE)$@$(COLOR_RESET)."
	@ar rcs $@ $^

%.o: %.c
	@echo "Compiling $(COLOR_CYAN)$<$(COLOR_RESET)."
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning $(COLOR_YELLOW)$(NAME)$(COLOR_RESET) object files."
	@rm -f $(OBJS)

fclean: clean
	@echo "Removing the $(COLOR_YELLOW)$(NAME)$(COLOR_RESET) library."
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
