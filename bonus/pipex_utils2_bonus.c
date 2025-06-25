/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:54:27 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/25 16:13:21 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/// @brief 
/// @param file 
/// @param in_out 
void	redirect_fd(int file, int in_out)
{
	dup2 (file, in_out);
	close(file);
}

/// @brief a helper function to close fds, saving lines
/// @param fd1 the pipefd[0]
/// @param fd2 the pipefd[1]
void	close_fds(int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
}

/// @brief 
/// @param argv 
/// @param i 
/// @return 
int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_RDONLY, 0644);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 2)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (file == -1)
		pipex_error("file", 1);
	return (file);
}
