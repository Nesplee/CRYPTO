# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    launcher.sh                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 11:52:11 by dinguyen          #+#    #+#              #
#    Updated: 2024/11/21 17:22:15 by dinguyen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

PROGRAM="./portfolio_manager"

# Vérifie que l'exécutable existe et est accessible
if [ ! -x "$PROGRAM" ]; then
    echo -e "\033[31mErreur : L'exécutable $PROGRAM est introuvable ou non exécutable.\033[0m"
    exit 1
fi

# Détection des dimensions maximales du terminal actuel
max_cols=$(tput cols 2>/dev/null || echo 150)
max_rows=$(tput lines 2>/dev/null || echo 40)

# Calcul des dimensions ajustées (taille max - 1 pour éviter les débordements)
adjusted_cols=$((max_cols - 1))
adjusted_rows=$((max_rows - 1))

echo "Dimensions maximales détectées : $max_cols colonnes, $max_rows lignes"
echo "Dimensions ajustées : $adjusted_cols colonnes, $adjusted_rows lignes"

# Commande pour redimensionner le terminal actuel si possible
resize_command=$(printf '\033[8;%d;%dt' "$adjusted_rows" "$adjusted_cols")

# Lancer dans un terminal spécifique avec redimensionnement et fermeture automatique
if command -v gnome-terminal >/dev/null 2>&1; then
    echo "Lancement via gnome-terminal..."
    gnome-terminal --maximize -- bash -c "sleep 0.2; echo -ne \"$resize_command\"; $PROGRAM; exit"
elif command -v xterm >/dev/null 2>&1; then
    echo "Lancement via xterm..."
    xterm -geometry ${adjusted_cols}x${adjusted_rows} -e "sleep 0.2; echo -ne \"$resize_command\"; $PROGRAM"
elif command -v konsole >/dev/null 2>&1; then
    echo "Lancement via konsole..."
    konsole --fullscreen --noclose -e "sleep 0.2; echo -ne \"$resize_command\"; $PROGRAM; exit"
elif command -v xfce4-terminal >/dev/null 2>&1; then
    echo "Lancement via xfce4-terminal..."
    xfce4-terminal --maximize --command="sleep 0.2; echo -ne \"$resize_command\"; $PROGRAM; sleep 0"
elif command -v urxvt >/dev/null 2>&1; then
    echo "Lancement via urxvt..."
    urxvt -geometry ${adjusted_cols}x${adjusted_rows} -e "sleep 0.2; echo -ne \"$resize_command\"; $PROGRAM"
elif command -v mate-terminal >/dev/null 2>&1; then
    echo "Lancement via mate-terminal..."
    mate-terminal --maximize -- bash -c "sleep 0.2; echo -ne \"$resize_command\"; $PROGRAM; exit"
else
    echo -e "\033[33mAucun terminal compatible détecté. Exécution dans le terminal actuel.\033[0m"
    echo -ne "$resize_command"
    sleep 0.2
    $PROGRAM
fi
