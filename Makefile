CC := cc

INCLUDES := .
LIBFT_DIR := ./libft
LIBFT_LIB := $(LIBFT_DIR)/libft.a
BUILTINS_DIR := ./builtins
GRAMMAR_DIR := ./grammar

NAME := minishell
SRCS := minishell.c signals.c \
		$(BUILTINS_DIR)/env.c \
		$(GRAMMAR_DIR)/token.c $(GRAMMAR_DIR)/lexer.c \
		$(GRAMMAR_DIR)/validator.c

CFLAGS := -Wall -Wextra -Werror -g -I$(INCLUDES) -I$(LIBFT_DIR)
LDFLAGS := -L$(LIBFT_DIR) -lft -lreadline

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS)
	@echo "Linking $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(LIBFT_LIB):
	@echo "Building $@"
	@make -C $(LIBFT_DIR) -s

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Removing objects"
	@rm -f $(OBJS)

fclean: clean
	@echo "Removing binaries"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) -s fclean

re: fclean all

.PHONY: all fclean re clean
