CC := cc

INCLUDES := .
LIBFT_DIR := ./libft
LIBFT_LIB := $(LIBFT_DIR)/libft.a
BUILTINS_DIR := ./builtins
GRAMMAR_DIR := ./grammar
EXPANSION_DIR := ./expansion
EXECUTOR_DIR := ./executor
STRVEC_DIR := ./strvec

NAME := minishell
SRCS := minishell.c \
		signals.c \
		signals2.c \
		shell.c \
		safe_wrappers.c \
		safe_wrappers_2.c \
		cmd_resolve.c \
		cmd_resolve_utils.c \
		here.c \
		fancy_prompt.c \
		shlvl.c \
		$(STRVEC_DIR)/strvec.c \
		$(STRVEC_DIR)/strvec_op.c \
		$(BUILTINS_DIR)/env.c \
		$(BUILTINS_DIR)/echo.c \
		$(BUILTINS_DIR)/utils.c \
		$(BUILTINS_DIR)/pwd.c \
		$(BUILTINS_DIR)/cd.c \
		$(BUILTINS_DIR)/cd_error.c \
		$(BUILTINS_DIR)/canonical_path.c \
		$(BUILTINS_DIR)/reconstruct_path.c \
		$(BUILTINS_DIR)/unset.c \
		$(BUILTINS_DIR)/export.c \
		$(BUILTINS_DIR)/exit.c \
		$(EXECUTOR_DIR)/exec.c \
		$(EXPANSION_DIR)/variable_expansion.c \
		$(EXPANSION_DIR)/field_splitting.c \
		$(EXPANSION_DIR)/filename_expansion.c \
		$(EXPANSION_DIR)/quote_removal.c \
		$(EXPANSION_DIR)/variable_expansion_append.c \
		$(EXPANSION_DIR)/pattern_match.c \
		$(EXPANSION_DIR)/pattern_match_utils.c \
		$(EXPANSION_DIR)/split_helpers.c \
		$(GRAMMAR_DIR)/token.c \
		$(GRAMMAR_DIR)/lexer.c \
		$(GRAMMAR_DIR)/validator.c \
		$(GRAMMAR_DIR)/validator_utils.c \
		$(GRAMMAR_DIR)/command_formatter.c \
		$(GRAMMAR_DIR)/parser.c \
		$(GRAMMAR_DIR)/lexer_helpers.c \
		$(GRAMMAR_DIR)/skip_empty_vars.c \
		$(GRAMMAR_DIR)/cf_utils.c \
		$(EXECUTOR_DIR)/open_file.c \
		$(EXECUTOR_DIR)/handle_redir.c \
		$(EXECUTOR_DIR)/check_access.c \
		$(EXECUTOR_DIR)/apply_redir.c	\
		$(EXECUTOR_DIR)/exec_cmd_ex_helpers.c	\
		$(EXECUTOR_DIR)/exec_pipe_utils.c	\
		$(EXECUTOR_DIR)/exec_pipe_helpers.c	\
		$(EXECUTOR_DIR)/exec_cmd_bi_helpers.c	\
		$(EXECUTOR_DIR)/exec_entry.c	\
		$(EXECUTOR_DIR)/heredoc_file.c \
		$(EXECUTOR_DIR)/io_error.c \
		free_ast.c \
		$(EXECUTOR_DIR)/exec_pipe_utils2.c

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
