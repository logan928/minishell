/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_formatter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwettasi <uwettasi@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:38:27 by uwettasi          #+#    #+#             */
/*   Updated: 2025/09/01 15:38:32 by uwettasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_redir	*redir_new(t_redir_type kind, const char *file)
{
	int		i;
	t_redir	*r;

	i = 0;
	r = ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (NULL);
	r->kind = kind;
	r->file = argv_add(r->file, &i, file);
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

static bool	update_redirs(t_shell *shell, t_token **tok_pointer, t_command *cmd)
{
	t_redir_type	kind;
	t_token			*tok;

	tok = *tok_pointer;
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
		printf("minishell: syntax error near redirection\n");
		shell->parse_err = 1;
		//break;//TODO: fix this bug. Need to clear everything and return to prompt. 
		return (false);
	}
	redir_add(cmd, redir_new(kind, tok->data));
	*tok_pointer = tok;
	return (true);
}

static void	format_next_token(t_shell *shell, t_token **tok_ptr, t_command *cmd)
{
	t_token	*tok;
	int		argc;

	tok = *tok_ptr;
	argc = 0;
	while (tok)
	{
		if (tok->token_kind == WORD)
			cmd->args = argv_add(cmd->args, &argc, tok->data);
		else if (tok->token_kind == LESS || tok->token_kind == GREAT
			|| tok->token_kind == DGREAT || tok->token_kind == DLESS)
		{
			if (!update_redirs(shell, &tok, cmd))
				break ;
		}
		else if (tok->token_kind == PIPE || tok->token_kind == AND_IF
			|| tok->token_kind == OR_IF || tok->token_kind == L_PAREN
			|| tok->token_kind == R_PAREN || tok->token_kind == NL)
			break ;
		tok = tok->next;
	}
	*tok_ptr = tok;
}

t_command	*command_formatter(t_shell *shell, t_token **tokptr)
{
	//t_token		*tok;
	t_command	*cmd;

	//tok = *tokptr;
	cmd = command_new();
	format_next_token(shell, tokptr, cmd);
	if (cmd->args)
	{
		ft_variable_expansion(shell, cmd->args, 0);
		ft_field_splitting(shell, &cmd->args, 1);
		ft_file_exp(shell, &cmd->args, 1, 1);
		ft_quote_removal(shell, cmd->args, 0);
	}
	cmd->command_kind = EXTERNAL;
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
		cmd->command_kind = BUILTIN;
	return (cmd);
}
