#include "../../includes/minishell.h"

struct s_cmd get_pipe(t_cmd cmd, t_token *token)
{
    if (token->type == pipeline)
        cmd.pipe_o = 1; 
    if (token->type == and)
        cmd.pipe_o = 2;
    if (token->type == or)
        cmd.pipe_o = 3;
    return (cmd);
}

void create_cmd(t_cmd *full_cmd, char *str)
{
	char	**new;

	new = ft_split(str, -1);
    full_cmd->type = get_type(new);
    full_cmd->cmd = new;
}

t_lists *move_lst(t_lists *lst, int i)
{
    int count;

    if (i > 0)
    {
        count = 0;
        while (count < i)
        {
            if (lst->token->type == pipeline)
                count++;
            lst = lst->next;
        }
    }
    return (lst);
}

struct s_cmd parse_cmds(t_lists *lst, int i)
{
    t_cmd full_cmd;

    init_struct(&full_cmd);
    while(lst)
    {
        lst = move_lst(lst, i);
        i = 0;
        if (lst->token->type == literal)
            create_cmd(&full_cmd, lst->token->content);
        else if (lst->token->type == simple_redir_right)
            full_cmd.fd_o = get_redir_r(lst->token);
        else if (lst->token->type == simple_redir_left)
            full_cmd.fd_i = get_redir_l(lst->token);
        else if (lst->token->type == double_redir_right)
            full_cmd.fd_o = get_redir_dr(lst->token);
        else if (lst->token->type == double_redir_left)
            full_cmd.fd_i = get_redir_dl(lst->token);
        else if (lst->token->type == pipeline || lst->token->type == and || lst->token->type == or)
            return(get_pipe(full_cmd, lst->token));
        lst = lst->next;
    }
    return (full_cmd);
}

int set_cmd(t_all_cmd *all_cmd, t_lists *lst)
{
    int i;

    all_cmd->nbrcmd = count_cmd(lst);
    all_cmd->cmds = malloc(sizeof(t_cmd) * all_cmd->nbrcmd);
    if (!all_cmd->cmds)
        return (0);
    i = 0;
    while (i < all_cmd->nbrcmd)
    {
        all_cmd->cmds[i] = parse_cmds(lst, i);
        if (i > 0)
            all_cmd->cmds[i].pipe_i = all_cmd->cmds[i - 1].pipe_o;
        i++;
    }
    if (i == 1 && all_cmd->cmds[0].type > 0)
        return (1);
    else
    {
        all_cmd->pids = malloc(sizeof(pid_t) * i);
        return (1);
    }
}