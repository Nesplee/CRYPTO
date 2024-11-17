# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/16 22:36:02 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/17 16:49:55 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRC = main.c 5.asset_management.c 1.utils.c 4.menu.c 3.displays.c 2.resize_free.c \
	  7.save_load_export.c 6.portfolio_management.c
OBJ = $(SRC:.c=.o)
NAME = portfolio_manager
SCRIPT = launcher.sh
TEST_SRC = tests.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_NAME = test_program

# Règle principale
all: $(NAME)

# Compilation du programme principal
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

# Exécution via le script
run: $(NAME)
	./$(SCRIPT)

# Compilation et exécution des tests
test: $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_NAME) $(OBJ) $(TEST_OBJ)

# Génération des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TEST_OBJ)

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: clean fclean all run test
