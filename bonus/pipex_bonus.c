/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:12:10 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/25 18:57:11 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/// @brief a helper function to close fds, saving lines
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
/// @param envp the list of environment variables we have locally in the system
/// @param fd the list of file descriptors of our pipe() call
static void	call_process(char *argv, char **envp)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		pipex_error("pipe", 1);
	pid = fork();
	if (pid == -1)
		pipex_error("fork", 1);
	if (pid == 0)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			pipex_error("dup2", 1);
		close_fds(pipefd[0], pipefd[1]);
		pipex_execute(argv, envp);
	}
	else
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			pipex_error("dup2", 1);
		close_fds(pipefd[0], pipefd[1]);
		waitpid (pid, NULL, 0);
	}
}

/// @brief the execution of the here_doc in the terminal
/// @param argv the arguments of the function call
/// @param pipefd the lists of fds that were open
static void	here_doc_exec(char **argv, int *pipefd, int old_fd)
{
	char	*result;

	close (pipefd[0]);
	while (1)
	{
		write (1, "pipe heredoc> ", 14);
		result = get_next_line(STDIN_FILENO);
		if (ft_strncmp(result, argv[2], ft_strlen(argv[2])) == 0)
		{
			free (result);
			close(pipefd[1]);
			close(old_fd);
			exit (0);
		}
		ft_putstr_fd(result, pipefd[1]);
		free(result);
	}
}

/// @brief the function that will pipe and fork the heredoc process
/// @param argv the arguments we will receive in the function call
static void	here_doc(char **argv, int old_fd)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		pipex_error("pipe", 1);
	pid = fork();
	if (pid == -1)
		pipex_error("fork", 1);
	if (pid == 0)
		here_doc_exec(argv, pipefd, old_fd);
	else
	{
		dup2(pipefd[0], STDIN_FILENO);
		close_fds(pipefd[0], pipefd[1]);
		waitpid(pid, NULL, 0);
	}
}

/// @brief it's the core function of our program, here is the logical structure
/// @param argc the number of arguments we will receive
/// @param argv the arguments we'll receive in the program call
/// @param envp the list of environment variables
int	main(int argc, char **argv, char **envp)
{
	int	file[2];
	int	i;

	if (argc < 5)
		pipex_error("Error bonus", 1);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			pipex_error("Usage: ./pipex here_doc LIMITER cmd cmd1 file", 1);
		i = 3;
		file[1] = open_file(argv[argc - 1], 2);
		here_doc(argv, file[1]);
	}
	else
	{
		i = 2;
		file[0] = open_file(argv[1], 0);
		file[1] = open_file(argv[argc - 1], 1);
		redirect_fd(file[0], STDIN_FILENO);
	}
	redirect_fd(file[1], STDOUT_FILENO);
	while (i < argc - 2)
		call_process(argv[i++], envp);
	pipex_execute(argv[argc - 2], envp);
}
