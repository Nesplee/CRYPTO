# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    launcher.sh                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/16 22:10:03 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/17 21:17:57 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

PROGRAM="./portfolio_manager"

if [ ! -f "$PROGRAM" ]; then
	echo -e "\033[31mErreur : L'exécutable $PROGRAM est introuvable. Compilez d'abord avec 'make'.\033[0m"
	exit 1
fi

if command -v gnome-terminal >/dev/null 2>&1; then
	gnome-terminal -- bash -c "$PROGRAM; exit"
elif command -v xterm >/dev/null 2>&1; then
	xterm -e "$PROGRAM; exit"
elif command -v konsole >/dev/null 2>&1; then
	konsole --noclose -e "$PROGRAM; exit"
else
	echo -e "\033[33mAucun terminal compatible détecté. Exécution dans ce terminal.\033[0m"
	$PROGRAM
	exit
fi

