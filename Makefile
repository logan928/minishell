CC := cc

INCLUDES := .
LIBFT_DIR := ./libft
LIBFT_LIB := $(LIBFT_DIR)/libft.a
BUILTINS_DIR := ./builtins
GRAMMAR_DIR := ./grammar
EXPANSION_DIR := ./expansion
EXECUTOR_DIR := ./executor

NAME := minishell
SRCS := minishell.c signals.c shell.c safe_wrappers.c safe_wrappers_2.c \
		cmd_resolve.c here.c \
		$(BUILTINS_DIR)/env.c $(BUILTINS_DIR)/echo.c \
		$(GRAMMAR_DIR)/token.c $(GRAMMAR_DIR)/lexer.c \
		$(GRAMMAR_DIR)/validator.c \
		$(EXPANSION_DIR)/variable_expansion.c $(EXPANSION_DIR)/field_splitting.c \
		$(EXPANSION_DIR)/filename_expansion.c $(EXPANSION_DIR)/quote_removal.c \
		$(EXPANSION_DIR)/variable_expansion_append.c \
		$(EXPANSION_DIR)/pattern_match.c \
		$(GRAMMAR_DIR)/command_formatter.c $(GRAMMAR_DIR)/parser.c \
		$(EXPANSION_DIR)/split_helpers.c $(BUILTINS_DIR)/exit.c \
<<<<<<< HEAD
		$(BUILTINS_DIR)/utils.c $(BUILTINS_DIR)/pwd.c \
		$(EXECUTOR_DIR)/exec.c
=======
		$(BUILTINS_DIR)/utils.c $(BUILTINS_DIR)/pwd.c $(BUILTINS_DIR)/cd.c
>>>>>>> 4d16a78d4a01d75d33fc41b0aa1b7ce2ab2d4a94

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
