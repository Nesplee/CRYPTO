/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2d-MENU-assets.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:51:43 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 01:53:32 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A2.menu.h"

void asset_menu(t_portfolio_manager *manager)
{
	while (1)
	{
		clear_screen();
		display_header(LIGHT_BLUE "ASSET MENU" RESET);

		if (manager->portfolio_count == 0)
		{
			print_centered("Erreur : Aucun portefeuille actif.", RED);
			print_centered("Créez ou sélectionnez un portefeuille avant de gérer les actifs.", GRAY);
			pause_if_needed(1); // Pause directement sans variable intermédiaire
			return; // Retourner immédiatement au portfolio_menu si aucun portefeuille n'est disponible
		}

		// Afficher les actifs disponibles
		print_centered("Actifs disponibles dans le portefeuille actif :", LIGHT_BLUE);
		display_assets(manager->portfolios[0]);

		// Options du menu
		print_centered("1. Visions sur les actifs", LIGHT_BLUE);
		print_centered("2. Actions sur les actifs", LIGHT_BLUE);
		print_centered("3. Suppression d’un actif", LIGHT_BLUE);
		print_centered("0. Retour au menu portefeuille", GRAY);

		int choice = input_choice(0, 3);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1); // Pause sans variable
			continue;
		}

		if (choice == 1)
		{
			asset_vision_menu(manager);
		}
		else if (choice == 2)
		{
			asset_action_menu(manager);
		}
		else if (choice == 3)
		{
			handle_delete_asset(manager->portfolios[0]);
		}
		else if (choice == 0)
		{
			return; // Retour au portfolio_menu
		}

		pause_if_needed(1); // Pause après chaque action pour laisser le temps de lire les messages
	}
}
