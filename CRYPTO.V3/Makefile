# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/21 03:26:32 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/24 18:11:51 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRC = E1-MENU-main.c E2a-MENU-portfolio.c E2b-HANDLE-portfolio.c E2c-UTILS-portfolio.c \
	E2d-MENU-assets.c E2e-HANDLE-assets.c E2f-UTILS-assets.c E3a.MENU-portfolio.c \
	E3b.HANDLE-portfolio.c E3c-UTILS-portfolio.c E3d-MENU-assets.c E3e-HANDLE-assets.c \
	E3f-UTILS-assets.c EE-MENU-system.c EE-UTILS-system.c EU1-Resize_and_free.c \
	EU2-UTILS-global.c EU3-Inputs.c E4-VALUE.c main.c
OBJ = $(SRC:.c=.o)
NAME = portfolio_manager
HEADER = A1.master.h A2.menu.h A3.portfolio.h A4.handle.h A5.system.h A6.utils.h

# Règle principale
all: $(NAME)

# Compilation du programme principal
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Compilation des fichiers objets
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets
clean:
	rm -f $(OBJ)

# Règle pour nettoyer tous les fichiers générés (objets et exécutables)
fclean: clean
	rm -f $(NAME)

# Règle pour nettoyer et recompiler tout
re: fclean all

# Exécution du programme
run: $(NAME)
	@echo "Lancement du programme dans une nouvelle fenêtre de terminal..."
	@./launcher.sh

.PHONY: all clean fclean re run
