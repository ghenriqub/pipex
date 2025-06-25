# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ghenriqu <ghenriqu@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/18 15:11:08 by ghenriqu          #+#    #+#              #
#    Updated: 2025/06/25 16:13:57 by ghenriqu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# hello, my name is:
NAME = pipex

# just the compiler and some basic flags (and RM rule, idk where it fits)
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

# directories
SRC_DIR = src
BONUS_DIR = bonus
INCLUDES_DIR = includes
LIBFT_DIR = libft

# libft
LIBFT = $(LIBFT_DIR)/libft.a

# includes
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

# basic and additional files:
SRC = 	$(SRC_DIR)/pipex.c \
		$(SRC_DIR)/pipex_utils.c \
		$(SRC_DIR)/pipex_parser.c

OBJ = $(SRC:.c=.o)

# basic and additional files for bonus:
SRCB = 	$(BONUS_DIR)/pipex_bonus.c \
		$(BONUS_DIR)/pipex_utils_bonus.c \
		$(BONUS_DIR)/pipex_utils2_bonus.c \
		$(BONUS_DIR)/pipex_parser_bonus.c

OBJB = $(SRCB:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiling $<..."

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -o $(NAME)

bonus: $(LIBFT) $(OBJB)
	@echo "Building bonus pipex..."
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJB) $(LIBFT) -o $(NAME)

clean:
	@echo "Removing object files..."
	@rm -f $(OBJ) $(OBJB)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean:
	@echo "Removing binary file..."
	@rm -f $(NAME)
	@rm -f $(OBJ) $(OBJB)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all
