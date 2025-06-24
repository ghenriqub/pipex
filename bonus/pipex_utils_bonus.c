/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:54:27 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/24 18:09:58 by ghenriqu         ###   ########.fr       */
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
	if (error)
		exit(error);
	else
		exit(EXIT_FAILURE);
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

/// @brief here we clean our splits and paths with error in return
/// @param split the split that we've allocated
/// @param path the path to our command
static void	command_error(char **split, char *path)
{
	free_split(split);
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
