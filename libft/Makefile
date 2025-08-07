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
		ft_strlen.c

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
