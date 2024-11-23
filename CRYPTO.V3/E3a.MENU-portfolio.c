/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3a.MENU-portfolio.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:28:06 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/22 16:39:55 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A2.menu.h"

void portfolio_action_menu(t_portfolio_manager *manager)
{
	while (1)
	{
		clear_screen();
		display_header(BLUE "ACTIONS SUR LES PORTEFEUILLES" RESET);

		if (manager->portfolio_count == 0)
		{
			print_centered("Erreur : Aucun portefeuille disponible. Créez un portefeuille d'abord.", RED);
			pause_if_needed(1);
			return; // Retour au portfolio_menu
		}

		print_centered("Portefeuilles disponibles :", BLUE);
		display_portfolios(manager);
		print_centered("1. Ajouter des dollars", BLUE);
		print_centered("2. Retirer des dollars", BLUE);
		print_centered("3. Gérer les actifs", BLUE);
		print_centered("0. Retour", GRAY);

		int choice = input_choice(0, 3);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1);
			continue;
		}

		if (choice == 1 || choice == 2)
		{
			char portfolio_name[50];
			if (input_name(portfolio_name, sizeof(portfolio_name)) == 0) continue;

			t_portfolio *portfolio = find_portfolio_by_name(manager, portfolio_name);
			if (!portfolio)
			{
				print_centered("Portefeuille introuvable.", RED);
				pause_if_needed(1);
				continue;
			}

			if (choice == 2 && portfolio->dollar_balance == 0)
			{
				print_centered("Erreur : Portefeuille vide. Retrait impossible.", RED);
				pause_if_needed(1);
				continue;
			}

			float amount = input_amount();
			if (amount <= 0) continue;

			char date[20];
			if (input_date(date, sizeof(date)) == 0) continue;

			if (choice == 1)
				handle_add_dollars(portfolio, amount, date);
			else if (choice == 2)
				handle_withdraw_dollars(portfolio, amount, date);
		}
		else if (choice == 3)
		{
			asset_action_menu(manager);
		}
		else if (choice == 0)
		{
			return; // Retour au portfolio_menu
		}

		pause_if_needed(1);
	}
}

void portfolio_vision_menu(t_portfolio_manager *manager)
{
	while (1)
	{
		clear_screen();
		display_header(BLUE "VISION SUR LES PORTEFEUILLES" RESET);

		// Options du sous-menu
		print_centered("1. Afficher le portefeuille actif (court)", BLUE);
		print_centered("2. Afficher le portefeuille actif (long)", BLUE);
		print_centered("3. Afficher tous les portefeuilles (court)", BLUE);
		print_centered("4. Afficher tous les portefeuilles (long)", BLUE);
		print_centered("0. Retour", GRAY);

		int choice = input_choice(0, 4);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1);
			continue;
		}

		if (choice == 1)
		{
			display_portfolio_short_summary(get_active_portfolio(manager));
		}
		else if (choice == 2)
		{
			display_portfolio_long_summary(get_active_portfolio(manager));
		}
		else if (choice == 3)
		{
			display_all_portfolios_short(manager);
		}
		else if (choice == 4)
		{
			display_all_portfolios_long(manager);
		}
		else if (choice == 0)
		{
			return; // Retour au portfolio_menu
		}

		pause_if_needed(1);
	}
}
