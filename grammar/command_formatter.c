#include "../minishell.h"

static char	*str_append(char *s1, const char *s2)
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

static t_lexem	*lexem_new(void)
{
	t_lexem	*lx = calloc(1, sizeof(t_lexem));
	if (!lx)
		return (NULL);
	lx->lexem_kind = CMAND; // default, refine later
	lx->args = NULL;
	lx->op = NULL;
	lx->file = NULL;
	return (lx);
}

static t_redir	*redir_new(t_redir_kind kind, const char *file)
{
	t_redir	*r = calloc(1, sizeof(t_redir));
	if (!r)
		return (NULL);
	r->kind = kind;
	r->file = strdup(file);
	r->next = NULL;
	return (r);
}

static void	redir_add(t_lexem *cmd, t_redir *redir)
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


t_lexem	*command_formatter(t_token **tokptr)
{
	t_token	*tok = *tokptr;
	t_lexem	*cmd = lexem_new();
	char	*current_word = NULL;
	int		argc = 0;

	while (tok)
	{
		if (tok->token_kind == WORD)
		{
			// glue consecutive WORDs - discuss with Mikhail whetehr to pass as an string or separate words (i.e. without space)
			if (current_word)
				current_word = str_append(current_word, " "); //can'use ft_strjoin_free or ft_strjoin
			current_word = str_append(current_word, tok->data);
			if (!tok->next || tok->next->token_kind != WORD)
			{
				cmd->args = argv_add(cmd->args, &argc, current_word);
				free(current_word);
				current_word = NULL;
			}

			//todo: add cmd to the cmd list & make a new cmd (lexem_new())
		}
		else if (tok->token_kind == LESS || tok->token_kind == GREAT
			|| tok->token_kind == DGREAT || tok->token_kind == DLESS)
		{
			t_redir_kind kind;
			if (tok->token_kind == LESS)
				kind = R_IN;
			else if (tok->token_kind == GREAT)
				kind = R_OUT;
			else if (tok->token_kind == DGREAT)
				kind = R_APP;
			else
				kind = R_HDOC;

			// expect next WORD as filename/limiter
			tok = tok->next;
			if (!tok || tok->token_kind != WORD)
			{
				printf("minishell: syntax error near redirection\n");
				break;
			}
			redir_add(cmd, redir_new(kind, tok->data));
			
			//todo: add cmd to the cmd list & make a new cmd (lexem_new())

		}
		else if (tok->token_kind == PIPE || tok->token_kind == AND_IF
			|| tok->token_kind == OR_IF || tok->token_kind == L_PAREN
			|| tok->token_kind == R_PAREN || tok->token_kind == NL)
		{
			// stop here and let parser handle higher-level ops

						//todo: add cmd to the cmd list & make a new cmd (lexem_new())

			break; //continue
		}
		tok = tok->next;
	}
	*tokptr = tok; // tell caller where we stopped
	return (cmd);
}

void	print_lexem(t_lexem *cmd)
{
	if (!cmd)
		return;
	printf("=== Command ===\n");

	// args
	if (cmd->args)
	{
		printf("argv:");
		for (int i = 0; cmd->args[i]; i++)
			printf(" [%s]", cmd->args[i]);
		printf("\n");
	}
	else
		printf("argv: (none)\n");

	// redirections
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
