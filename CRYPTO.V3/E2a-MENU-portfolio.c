/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2a-MENU-portfolio.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:49:52 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/22 15:26:49 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A2.menu.h"

void portfolio_menu(t_portfolio_manager *manager)
{
	while (1) // On ne met pas de variable exit_menu ici, car le retour est explicite
	{
		clear_screen();
		display_header(BLUE "PORTFOLIO MENU" RESET);

		// Vérification s'il y a des portefeuilles actifs
		if (manager->portfolio_count == 0)
		{
			print_centered("Erreur : Aucun portefeuille disponible. Créez un portefeuille d'abord.", RED);
			pause_if_needed(1);

			// Afficher les options du menu pour créer un portefeuille
			print_centered("1. Création d'un portefeuille", BLUE);
			print_centered("0. Retour au menu principal", GRAY);

			int choice = input_choice(0, 1);
			if (choice == 1)
			{
				handle_create_portfolio(manager);
			}
			else if (choice == 0)
			{
				return; // Retour au menu principal
			}
			continue;
		}

		// Afficher les portefeuilles disponibles et options du menu
		print_centered("Portefeuilles disponibles :", BLUE);
		display_portfolios(manager);
		print_centered("1. Création d'un portefeuille", BLUE);
		print_centered("2. Visions sur les portefeuilles", BLUE);
		print_centered("3. Actions sur les portefeuilles", BLUE);
		print_centered("4. Suppression d’un portefeuille", BLUE);
		print_centered("0. Retour au menu principal", GRAY);

		int choice = input_choice(0, 4);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1);
			continue;
		}

		if (choice == 1)
		{
			handle_create_portfolio(manager);
		}
		else if (choice == 2)
		{
			portfolio_vision_menu(manager);
		}
		else if (choice == 3)
		{
			portfolio_action_menu(manager);
		}
		else if (choice == 4)
		{
			handle_delete_portfolio(manager);
		}
		else if (choice == 0)
		{
			return; // Retour au menu principal
		}

		pause_if_needed(1);
	}
}
