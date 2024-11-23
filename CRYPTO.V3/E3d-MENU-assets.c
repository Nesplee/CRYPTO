/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3d-MENU-assets.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:59:24 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 01:59:43 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A2.menu.h"

/*	============================== MENU VISION ==============================*/

void asset_vision_menu(t_portfolio_manager *manager)
{
	while (1)
	{
		clear_screen();
		display_header(LIGHT_BLUE "VISIONS SUR LES ACTIFS" RESET);

		print_centered("1. Afficher toutes les transactions d'un actif", LIGHT_BLUE);
		print_centered("2. Afficher les 10 derniers achats (tous portefeuilles)", LIGHT_BLUE);
		print_centered("3. Afficher les 10 dernières ventes (tous portefeuilles)", LIGHT_BLUE);
		print_centered("0. Retour", GRAY);

		int choice = input_choice(0, 3);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1);
			continue;
		}

		if (choice == 1)
		{
			handle_display_asset_transactions(manager);
		}
		else if (choice == 2)
		{
			handle_display_last_purchases(manager);
		}
		else if (choice == 3)
		{
			handle_display_last_sales(manager);
		}
		else if (choice == 0)
		{
			return; // Retour à asset_menu
		}

		pause_if_needed(1);
	}
}


/*	============================== MENU ACTION ==============================*/

void asset_action_menu(t_portfolio_manager *manager)
{
	while (1)
	{
		clear_screen();
		display_header(LIGHT_BLUE "ACTIONS SUR LES ACTIFS" RESET);

		// Vérifier s'il y a au moins un portefeuille
		if (manager->portfolio_count == 0)
		{
			print_centered("Erreur : Aucun portefeuille actif.", RED);
			print_centered("Créez un portefeuille avant de gérer les actifs.", GRAY);
			pause_if_needed(1);
			return; // Retourner immédiatement si aucun portefeuille n'est disponible
		}

		// Demander à l'utilisateur de choisir un portefeuille
		print_centered("Veuillez sélectionner un portefeuille :", BLUE);
		display_portfolios(manager);

		char portfolio_name[50];
		input_name(portfolio_name, sizeof(portfolio_name));
		t_portfolio *portfolio = find_portfolio_by_name(manager, portfolio_name);

		if (!portfolio)
		{
			print_centered("Portefeuille introuvable. Veuillez réessayer.", RED);
			pause_if_needed(1); // Attendre l'entrée de l'utilisateur ici
			continue; // Recommence la boucle sans effacement supplémentaire
		}

		// Afficher le résumé du portefeuille actif
		clear_screen();
		display_header(LIGHT_BLUE "ACTIONS SUR LES ACTIFS" RESET);
		print_centered("Portefeuille actif :", LIGHT_BLUE);
		if (portfolio->asset_count > 0)
		{
			char asset_count_msg[50];
			sprintf(asset_count_msg, "Actifs disponibles : %d", portfolio->asset_count);
			print_centered(asset_count_msg, LIGHT_BLUE);
		}
		else
		{
			print_centered("Aucun actif à gérer. Vous pouvez en ajouter.", RED);
		}

		// Options du menu
		print_centered("1. Acheter un actif", LIGHT_BLUE);
		print_centered("2. Mettre à jour un actif", LIGHT_BLUE);
		print_centered("3. Vendre un actif", LIGHT_BLUE);
		print_centered("4. Calculer entre deux dates", LIGHT_BLUE);
		print_centered("5. Simuler une vente", LIGHT_BLUE);
		print_centered("0. Retour", GRAY);

		// Lecture du choix utilisateur
		int choice = input_choice(0, 5);

		if (choice == -1)
		{
			print_centered("Réessayez.", RED);
		}
		else if (choice == 1)
		{
			if (!handle_add_asset(portfolio))
				print_centered("Erreur lors de l'achat d'actif.", RED);
		}
		else if (choice == 2)
		{
			handle_update_asset(portfolio);
		}
		else if (choice == 3)
		{
			handle_sell_asset(portfolio);
		}
		else if (choice == 4)
		{
			handle_calculate_profit(portfolio);
		}
		else if (choice == 5)
		{
			handle_simulate_asset_sale(portfolio);
		}
		else if (choice == 0)
		{
			return; // Retourner au menu précédent
		}

		// Pause après chaque opération pour permettre à l'utilisateur de voir le message
		pause_if_needed(1);
	}
}
