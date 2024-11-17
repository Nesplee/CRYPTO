# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    launcher.sh                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/16 22:10:03 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/17 01:18:45 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

PROGRAM="./portfolio_manager"
TERMINAL="gnome-terminal"

if [ ! -f "$PROGRAM" ]; then
    echo -e "\033[31mErreur : L'exécutable $PROGRAM est introuvable. Compilez d'abord avec 'make'.\033[0m"
    exit 1
fi

if command -v $TERMINAL >/dev/null 2>&1; then
    echo -e "\033[32mLancement de l'application dans une nouvelle fenêtre GNOME Terminal...\033[0m"
    $TERMINAL -- bash -c "$PROGRAM; exec bash"
else
    echo -e "\033[33mGNOME Terminal introuvable. Lancement dans ce terminal.\033[0m"
    $PROGRAM
fi
