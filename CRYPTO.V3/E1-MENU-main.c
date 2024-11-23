/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E1-MENU-main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:58:43 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 01:54:05 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A2.menu.h"

void display_welcome_screen(void)
{
	clear_screen();

	// Récupérer la taille du terminal
	struct winsize w;
	int padding = 0;
	int i = 0;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1)
	{
		// Calculer le padding pour centrer verticalement
		int total_lines = 7 + 2; // 7 lignes pour le header + 2 lignes additionnelles
		padding = (w.ws_row - total_lines) / 2;
		if (padding < 0)
			padding = 0; // Éviter des valeurs négatives
	}
	else
	{
		padding = 5; // Valeur par défaut si la taille n'est pas récupérée
	}

	// Ajouter des lignes vides pour le centrage vertical
	while (i < padding)
	{
		printf("\n");
		i++;
	}

	// Afficher le header centré horizontalement
	display_asset_manager_header();

	// Afficher les textes additionnels
	print_centered("Gérez vos actifs et vos portefeuilles efficacement.", GRAY);
	print_centered("Appuyez sur Entrée pour commencer...", GREEN);

	// Attendre que l'utilisateur appuie sur Entrée
	while (getchar() != '\n')
		;

	clear_screen();
}


void display_header(const char *title)
{
	int term_width = get_terminal_width();

	char line[term_width + 1];
	memset(line, '=', term_width);
	line[term_width] = '\0';

	print_centered(line, GRAY);
	print_centered(title, RED);
	print_centered(line, GRAY);
}


void main_menu(t_portfolio_manager *manager)
{
	int exit_program = 0;
	int i;

	while (!exit_program)
	{
		clear_screen();
		display_header("CRYPTO MANAGER v1.by Nesplee");
		display_header("MENU PRINCIPAL");

		// Options du menu
		print_centered("1. Gérer les portefeuilles", BLUE);
		print_centered("2. Gérer les actifs", BLUE);
		print_centered("3. Options système", BLUE);
		print_centered("0. Quitter", BLUE);

		int choice = input_choice(0, 3);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1);
			continue;
		}

		if (choice == 1)
		{
			portfolio_menu(manager);
		}
		else if (choice == 2)
		{
			asset_menu(manager);
		}
		else if (choice == 3)
		{
			system_menu(manager);
		}
		else if (choice == 0)
		{
			print_centered("Merci d'avoir utilisé le gestionnaire de portefeuille.", GREEN);
			exit_program = 1;
		}
	}

	// Sauvegarder chaque portefeuille individuellement avant de quitter
	i = 0;
	while (i < manager->portfolio_count)
	{
		char filename[256];
		snprintf(filename, sizeof(filename), "%s.txt", manager->portfolios[i]->name);
		if (!save_portfolio(manager->portfolios[i], filename))
		{
			print_centered("Erreur: Échec de la sauvegarde d'un portefeuille.", RED);
		}
		i++;
	}
}
