#include "../minishell.h"

/*
static char	*str_append(char *s1, const char *s2) //ft_strjoin doesn't work for this. 
{
	size_t	len1; 
	size_t	len2; 
	char	*res; 


	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if(s2)
		len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (NULL);
	if (s1)
		ft_memcpy(res, s1, len1);
	if (s2)
		ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	free(s1);
	return (res);
}
*/

static char	**argv_add(char **argv, int *argc, const char *word)
{
	char	**newv;
	int		i;

	newv = malloc(sizeof(char *) * (*argc + 2));
	if (!newv)
		return (NULL);
	i = 0;
	while (i < *argc)
	{
		newv[i] = argv[i];
		i++;
	}
	newv[i++] = strdup(word);
	newv[i] = NULL;
	free(argv);
	(*argc)++;
	return (newv);
}

static t_command	*command_new(void)
{
	t_command	*lx = calloc(1, sizeof(t_command)); // TODO: replace with ft_calloc;
	if (!lx)
		return (NULL);
	lx->command_kind = EXTERNAL; // default, refine later
	lx->args = NULL;
	//lx->op = NULL;
	//lx->file = NULL;
	return (lx);
}

static t_redir	*redir_new(t_redir_type kind, const char *file)
{
	t_redir	*r = calloc(1, sizeof(t_redir)); // TODO: replace with ft_calloc;
	if (!r)
		return (NULL);
	r->kind = kind;
	r->file = strdup(file);
	r->next = NULL;
	return (r);
}

static void	redir_add(t_command *cmd, t_redir *redir)
{
	t_redir	*cur;

	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		cur = cmd->redirs;
		while (cur->next)
			cur = cur->next;
		cur->next = redir;
	}
}

static int is_builtin(const char *cmd)
{
	size_t len;
	if (!cmd)
		return 0;
	len = ft_strlen(cmd);
	return (
		ft_strncmp(cmd, "echo", len) == 0 ||
		ft_strncmp(cmd, "cd", len) == 0 ||
		ft_strncmp(cmd, "pwd", len) == 0 ||
		ft_strncmp(cmd, "export", len) == 0 ||
		ft_strncmp(cmd, "unset", len) == 0 ||
		ft_strncmp(cmd, "env", len) == 0 ||
		ft_strncmp(cmd, "exit", len) == 0
	);
}

t_command	*command_formatter(t_token **tokptr)
{
	t_token	*tok = *tokptr;
	t_command	*cmd = command_new();
	int		argc = 0;

	while (tok)
	{
		if (tok->token_kind == WORD)
		{
			cmd->args = argv_add(cmd->args, &argc, tok->data);
		}
		else if (tok->token_kind == LESS || tok->token_kind == GREAT
			|| tok->token_kind == DGREAT || tok->token_kind == DLESS)
		{
			t_redir_type kind;
			if (tok->token_kind == LESS)
				kind = R_IN;
			else if (tok->token_kind == GREAT)
				kind = R_OUT;
			else if (tok->token_kind == DGREAT)
				kind = R_APP;
			else
				kind = R_HDOC;

			tok = tok->next;
			if (!tok || tok->token_kind != WORD)
			{
				printf("minishell: syntax error near redirection\n");//ft_printf()
				break;//TODO: fix this bug. Need to clear everything and return to prompt. 
			}
			redir_add(cmd, redir_new(kind, tok->data));
			
		}
		else if (tok->token_kind == PIPE || tok->token_kind == AND_IF
			|| tok->token_kind == OR_IF || tok->token_kind == L_PAREN
			|| tok->token_kind == R_PAREN || tok->token_kind == NL)
		{
			break;
		}
		tok = tok->next;
	}
	*tokptr = tok; // tell caller where we stopped. Useful when integrating the Parser. consider, passing this as a pointer instead of a local variable. 
	cmd->command_kind = EXTERNAL;
	if(cmd->args[0] && is_builtin(cmd->args[0]))
		cmd->command_kind = BUILTIN;
	return (cmd);
}

void	print_lexem(t_command *cmd) //TODO: temporary function. Remove once the Parser is integrated. 
{
	if (!cmd)
		return;
	printf("=== Command ===\n");

	if (cmd->args)
	{
		printf("argv:");
		for (int i = 0; cmd->args[i]; i++)
			printf(" [%s]", cmd->args[i]);
		printf("\n");
	}
	else
		printf("argv: (none)\n");

	// redirections of the same command are printed here.
	t_redir *r = cmd->redirs;
	if (!r)
		printf("redirs: (none)\n");
	while (r)
	{
		const char *k = (r->kind == R_IN ? "<" :
						r->kind == R_OUT ? ">" :
						r->kind == R_APP ? ">>" : "<<");
		printf("redir: %s %s\n", k, r->file);
		r = r->next;
	}
}
