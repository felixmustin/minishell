/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmustin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 14:43:55 by fmustin           #+#    #+#             */
/*   Updated: 2022/04/26 14:43:56 by fmustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_redir_l(t_token *token)
{
	int	fd;

	fd = open(token->content, O_RDONLY);
	if (fd == -1)
		printf("minishell: %s: No such file or directory\n", token->content);
	return (fd);
}

int	close_dl_input(char **line, int fd, int i)
{
	close(fd);
	free(*line);
	if (i == 0)
		return (-1);
	fd = open("heredoc", O_RDWR);
	return (fd);
}

int	get_redir_dl(t_token *token)
{
	char	*line;
	int		size;
	int		fd;

	input_signals();
	set_sig_code(0);
	fd = open("heredoc", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (!fd)
		return (-1);
	size = ft_strlen(token->content);
	ft_putstr("heredoc>");
	line = get_next_line(0);
	while (line)
	{
		if (get_sig_code())
			return (close_dl_input(&line, fd, 0));
		if (!strncmp(line, token->content, size))
			break ;
		ft_putstr("heredoc>");
		ft_putstr_fd(line, fd);
		free(line);
		line = get_next_line(0);
	}
	return (close_dl_input(&line, fd, 1));
}

int	get_redir_r(t_token *token)
{
	int	fd;

	fd = open(token->content, O_CREAT | O_RDWR | O_TRUNC, 0666);
	return (fd);
}

int	get_redir_dr(t_token *token)
{
	int	fd;

	fd = open(token->content, O_CREAT | O_RDWR | O_APPEND, 0666);
	if (!fd)
		return (0);
	return (fd);
}
