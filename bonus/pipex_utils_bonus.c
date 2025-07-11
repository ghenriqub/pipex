/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:54:27 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/26 15:08:39 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/// @brief here we call the perror function with the personalized message
/// @param message the string with the text passed to perror
void	pipex_error(char *message, int error)
{
	char	*msg;

	if (ft_strncmp(message, "Error bonus", 11) == 0)
	{
		msg = ft_strdup("Usage: ./pipex here_doc LIMITER cmd cmd1 file\n \
						Usage: ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n");
		write (2, &msg, ft_strlen(msg));
	}
	else
		perror(message);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	if (error)
		exit(error);
	else
		exit(EXIT_FAILURE);
}

/// @brief here we clean our splits and paths with error in return
/// @param split the split that we've allocated
/// @param path the path to our command
static void	command_error(char **split, char *path)
{
	if (split)
		free_split(split);
	if (path)
		free(path);
	pipex_error("command not found", 127);
}

/// @brief executes a function using execve handling errors
/// @param argv is the command we got
/// @param envp is the list of local variables
void	pipex_execute(char *argv, char **envp)
{
	char	**command;
	char	*path;

	command = split_quote(argv);
	if (!command || !command[0])
		command_error(command, NULL);
	path = pipex_find_path(command[0], envp, "PATH=");
	if (command[0][0] == '/')
	{
		free(path);
		path = ft_strdup(command[0]);
	}
	if (!path)
	{
		path = pipex_find_path(command[0], envp, "PWD=");
		if (!path || access(path, F_OK | X_OK) == -1)
			command_error(command, path);
	}
	if (execve(path, command, envp) == -1)
		command_error(command, path);
}
