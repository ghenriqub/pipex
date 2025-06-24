/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:12:15 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/22 20:56:30 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/// @brief the function that allows us to correctly clean the split, if called
/// @param split the splitted string that'll be cleaned
void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free (split[i]);
		i++;
	}
	free(split);
}

/// @brief here we count the arguments with " or '
/// @param com is the line of command
/// @return how many commands we have even if it has " or ' inside
static int	count_args(const char *com)
{
	int		count;
	char	quote;

	count = 0;
	while (*com)
	{
		while (*com == ' ')
			com++;
		if (!*com)
			break ;
		count++;
		if (*com == '\'' || *com == '"')
		{
			quote = *com++;
			while (*com && *com != quote)
				com++;
			if (!*com)
				return (-1);
			com++;
		}
		else
			while (*com && *com != '"' && *com != '\'' && *com != ' ')
				com++;
	}
	return (count);
}

/// @brief here we find the argument, like the strdup to find the whole str
/// @param str the argument that we will allocate
/// @param i the index in the original str, thats why we use *i
/// @return the argument of the function {'awk'}, {'cat'}, {''{print}''}
static char	*extract_arg(const char *str, int *i)
{
	char	quote;
	int		start;
	char	*arg;

	if (str[*i] == '\'' || str[*i] == '"')
	{
		quote = str[(*i)++];
		start = *i;
		while (str[*i] && str[*i] != quote)
			(*i)++;
		if (!str[*i])
			return (NULL);
		arg = ft_substr(str, start, *i - start);
		(*i)++;
	}
	else
	{
		start = *i;
		while (str[*i] && str[*i] != ' ' && str[*i] != '\'' && str[*i] != '"')
			(*i)++;
		arg = ft_substr(str, start, *i - start);
	}
	if (!arg)
		return (NULL);
	return (arg);
}

/// @brief here we are goind to split the command in a correct way, \" or \'
/// @param command is the line of command that will be splited
/// @return the command ex: {'cat', NULL} or {'awk', '{print}', 'out', NULL}
char	**split_quote(char *command)
{
	char	**result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = ft_calloc(count_args(command) + 1, sizeof(char *));
	if (!result)
		return (NULL);
	while (command[j] && (i < count_args(command)))
	{
		while (command[j] == ' ')
			j++;
		if (!command[j])
			break ;
		result[i] = extract_arg(command, &j);
		if (!result[i])
		{
			free_split(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

/// @brief searches for the executable's full path in the PATH environment
/// @param command is the command we are looking for
/// @param envp is the list of local variables we ar matching
/// @return the path (ok) or null (error)
char	*pipex_find_path(char *command, char **envp, char *which)
{
	char	**split;
	char	*path;
	char	*pre_path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], which, ft_strlen(which)) != 0)
		i++;
	split = ft_split(envp[i] + ft_strlen(which), ':');
	i = 0;
	while (split[i])
	{
		pre_path = ft_strjoin(split[i], "/");
		path = ft_strjoin(pre_path, command);
		free(pre_path);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_split(split);
			return (path);
		}
		free(path);
		i++;
	}
	free_split(split);
	return (0);
}
