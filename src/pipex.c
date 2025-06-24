/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:12:10 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/23 20:02:22 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/// @brief the function tha helps us close our fds after use, saving lines
/// @param fd1 the pipefd[0]
/// @param fd2 the pipefd[1]
static void	close_fds(int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
}

/// @brief we call this function when the child process is running in the main
/// @param argv the list or arguments from the program call
/// @param envp the list of local variables we have locally in the system
/// @param fd the list of file descriptors of our pipe() call
static void	call_child(char **argv, char **envp, int *fd)
{
	int	file_in;

	file_in = open(argv[1], O_RDONLY);
	if (file_in == -1)
	{
		close_fds(fd[0], fd[1]);
		pipex_error("open infile", 1);
	}
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	dup2(file_in, STDIN_FILENO);
	close_fds(file_in, fd[0]);
	pipex_execute(argv[2], envp);
}

/// @brief we call this function when the parent process is running in the main
/// @param argv the list or arguments from the program call
/// @param envp the list of local variables we have locally in the system
/// @param fd the list of file descriptors of our pipe() call
static void	call_parent(char **argv, char **envp, int *fd)
{
	int	file_out;

	file_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_out == -1)
	{
		close_fds(fd[0], fd[1]);
		pipex_error("open outfile", 1);
	}
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	dup2(file_out, STDOUT_FILENO);
	close_fds(file_out, fd[1]);
	pipex_execute(argv[3], envp);
}

/// @brief it's the core function of our program, here is the logical structure
/// @param argc the number of arguments we will receive
/// @param argv the arguments we'll receive in the program call
/// @param envp the list of local functions
int	main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (argc != 5)
		pipex_error("Usage: ./pipex infile <cmd1> <cmd2> outfile", 1);
	if (pipe(pipefd) == -1)
		pipex_error("pipe", 1);
	pid1 = fork();
	if (pid1 == -1)
		pipex_error("fork", 1);
	if (pid1 == 0)
		call_child(argv, envp, pipefd);
	pid2 = fork();
	if (pid2 == -1)
		pipex_error("fork", 1);
	if (pid2 == 0)
		call_parent(argv, envp, pipefd);
	close_fds(pipefd[0], pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
}
