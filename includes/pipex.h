/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:12:35 by ghenriqu          #+#    #+#             */
/*   Updated: 2025/06/24 21:43:43 by ghenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define STDHD_FILENO 2

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h> // waitpid
# include <fcntl.h> // open
# include "../libft/src/libft.h" // libft
# include "../libft/src/get_next_line.h" // get_next_line

// pipex mandatory
void	pipex_error(char *message, int error);
void	pipex_execute(char *argv, char **envp);
char	*pipex_find_path(char *cmd, char **envp, char *which);
char	**split_quote(char *command);
void	free_split(char **split);

// pipex_bonus
int		open_file(char *argv, int i);

#endif
