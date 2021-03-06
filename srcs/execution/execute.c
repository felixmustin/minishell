/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmustin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 12:06:40 by fmustin           #+#    #+#             */
/*   Updated: 2022/04/26 12:06:41 by fmustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_builtin(t_all_cmd *all_cmd, int i)
{
	int	ret;

	ret = -1;
	if (all_cmd->cmds[i].pipe_i == 0 && all_cmd->cmds[i].pipe_o == 0)
		ret = handle_redir(all_cmd, i);
	if (ret != -1)
		return ;
	if (all_cmd->cmds[i].type == 1)
		ex_echo(all_cmd, i);
	else if (all_cmd->cmds[i].type == 2)
		ex_cd(all_cmd, i);
	else if (all_cmd->cmds[i].type == 3)
		ex_pwd(all_cmd, i);
	else if (all_cmd->cmds[i].type == 4)
		ex_export(all_cmd, i);
	else if (all_cmd->cmds[i].type == 5)
		ex_unset(all_cmd, i);
	else if (all_cmd->cmds[i].type == 6)
		ex_env(all_cmd, i);
	else if (all_cmd->cmds[i].type == 7)
		ex_exit(all_cmd, i);
	if (all_cmd->cmds[i].pipe_i > 0 || all_cmd->cmds[i].pipe_o > 0)
		exit(all_cmd->status);
}

void	execute_child_cmd(t_all_cmd *all_cmd, int i)
{
	int	ret;

	ret = -1;
	handle_pipe(all_cmd, i);
	ret = handle_redir_fork(&all_cmd->cmds[i]);
	if (ret != -1)
		exit(ret);
	close_pipes(all_cmd);
	if (all_cmd->cmds[i].type > 0)
		execute_builtin(all_cmd, i);
	if (all_cmd->cmds[i].type == 0)
		execve(all_cmd->cmds[i].cmd[0], all_cmd->cmds[i].cmd, g_env);
	exit_with_free(all_cmd, errno);
}

void	execute_cmd(t_all_cmd *all_cmd, int i)
{
	all_cmd->pids[i] = fork();
	if (all_cmd->pids[i] == -1)
		return ;
	if (!all_cmd->pids[i])
		execute_child_cmd(all_cmd, i);
}

void	execute(t_all_cmd *all_cmd)
{
	int	i;

	i = -1;
	cmd_signals(1);
	if (all_cmd->nbrcmd > 1)
		init_pipes(all_cmd);
	else if (all_cmd->nbrcmd == 1 && all_cmd->cmds[0].type > 0)
	{
		execute_builtin(all_cmd, 0);
		exit_cmds(all_cmd, 1);
		return ;
	}
	while (++i < all_cmd->nbrcmd)
		execute_cmd(all_cmd, i);
	exit_cmds(all_cmd, i);
	return ;
}

void	exit_cmds(t_all_cmd *all_cmd, int i)
{
	close_fd(all_cmd);
	close_pipes(all_cmd);
	wait_pid(all_cmd, i);
	free_pipe(all_cmd);
}
