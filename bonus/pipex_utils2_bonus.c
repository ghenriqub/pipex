/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:54:27 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/25 18:32:58 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/// @brief the function that executes the dup2 function and closes the file
/// @param file the file that will be opened and closed
/// @param in_out the behavior of the dup2 (open, close)
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

/// @brief the function that will open the respective file with permissions
/// @param argv the arguments passed in the function call
/// @param i the module that will be opened
/// @return returns the opened file
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
