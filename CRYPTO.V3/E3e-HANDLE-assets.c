/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3e-HANDLE-assets.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:01:33 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 03:20:51 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A4.handle.h"

/*	============================== HANDLE VISIONS ==============================*/

void handle_display_last_purchases(t_portfolio_manager *manager)
{
	int total_count = 0;
	int i = 0;
	int j;
	int k;

	print_centered("===== LES 10 DERNIERS ACHATS =====", GRAY);

	while (i < manager->portfolio_count && total_count < 10)
	{
		t_portfolio *portfolio = manager->portfolios[i];
		j = 0;
		while (j < portfolio->asset_count && total_count < 10)
		{
			t_asset *asset = portfolio->assets[j];
			k = asset->historique_count - 1;
			while (k >= 0 && total_count < 10)
			{
				printf("%-15s%-15.2f%-15.2f\n",
					   asset->historique[k].date,
					   asset->historique[k].quantite,
					   asset->historique[k].prix);
				k--;
				total_count++;
			}
			j++;
		}
		i++;
	}

	if (total_count == 0)
	{
		print_centered("Aucun achat trouvé.", RED);
	}

	print_centered("===========================================", GRAY);
}

void handle_display_last_sales(t_portfolio_manager *manager)
{
	int total_count = 0;
	int i = 0;
	int j;
	int k;

	print_centered("===== LES 10 DERNIÈRES VENTES =====", GRAY);

	while (i < manager->portfolio_count && total_count < 10)
	{
		t_portfolio *portfolio = manager->portfolios[i];
		j = 0;
		while (j < portfolio->asset_count && total_count < 10)
		{
			t_asset *asset = portfolio->assets[j];
			k = asset->sale_count - 1;
			while (k >= 0 && total_count < 10)
			{
				t_sale sale = asset->sales[k];
				printf("%-15s%-15.2f%-15.2f\n", sale.date,
					   sale.quantite_vendue, sale.prix_vente);
				k--;
				total_count++;
			}
			j++;
		}
		i++;
	}

	if (total_count == 0)
	{
		print_centered("Aucune vente trouvée.", RED);
	}

	print_centered("===========================================", GRAY);
}

void handle_display_asset_transactions(t_portfolio_manager *manager)
{
	char asset_name[256];
	int i = 0;
	int j;
	int k;
	int found = 0;

	// Demander à l'utilisateur d'entrer le nom de l'actif
	print_centered("Entrez le nom de l'actif dont vous souhaitez afficher les transactions :", LIGHT_BLUE);
	if (!input_name(asset_name, sizeof(asset_name)))
	{
		print_centered("Erreur : Nom d'actif invalide ou vide.", RED);
		return;  // Retourner si l'entrée est invalide
	}

	// Recherche de l'actif dans tous les portefeuilles
	while (i < manager->portfolio_count && !found)
	{
		t_portfolio *portfolio = manager->portfolios[i];
		j = 0;
		while (j < portfolio->asset_count && !found)
		{
			t_asset *asset = portfolio->assets[j];
			if (strcmp(asset->nom, asset_name) == 0)  // Si l'actif correspond
			{
				// Afficher l'historique des ventes de l'actif
				print_centered("======= HISTORIQUE DES VENTES =======", GRAY);
				printf("%-15s%-15s%-15s%-15s\n", "Date", "Quantité", "Prix de vente", "PNL");

				k = 0;
				while (k < asset->sale_count)
				{
					t_sale sale = asset->sales[k];
					printf("%-15s%-15.2f%-15.2f%-15.2f\n", sale.date,
						   sale.quantite_vendue, sale.prix_vente, sale.profit_loss_exit);
					k++;
				}

				print_centered("===========================================", GRAY);
				found = 1;  // L'actif a été trouvé, on arrête la recherche
			}
			j++;
		}
		i++;
	}

	// Si l'actif n'a pas été trouvé, afficher un message d'erreur
	if (!found)
	{
		print_centered("Aucun actif trouvé avec ce nom.", RED);
	}
}



/*	============================== HANDLE ACTIONS ==============================*/



int handle_add_asset(t_portfolio *portfolio)
{
	char nom[15], date[12];
	float prix, quantite, total_cost;

	if (!portfolio)
	{
		print_centered("Erreur: Portefeuille non défini.", RED);
		pause_if_needed(1);
		return 0;
	}

	// Saisie du nom
	input_name(nom, sizeof(nom));
	if (!is_snake_case(nom))
	{
		print_centered("Erreur: Nom invalide. Doit être en snake_case.", RED);
		pause_if_needed(1);
		return 0;
	}
	print_centered("Nom validé.", GREEN);

	// Saisie de la date
	input_date(date, sizeof(date));
	print_centered("Date validée.", GREEN);

	// Saisie du prix
	print_centered("PRIX D'ACHAT", LIGHT_BLUE);
	prix = input_amount();
	if (prix <= 0)
	{
		print_centered("Erreur: Prix invalide.", RED);
		pause_if_needed(1);
		return 0;
	}
	print_centered("Prix validé.", GREEN);

	// Saisie de la quantité
	print_centered("QUANTITE ACHETEE", LIGHT_BLUE);
	quantite = input_amount();
	if (quantite <= 0)
	{
		print_centered("Erreur: Quantité invalide.", RED);
		pause_if_needed(1);
		return 0;
	}
	print_centered("Quantité validée.", GREEN);

	total_cost = prix * quantite;

	// Vérification pour déduire du solde
	print_centered("Demande de confirmation pour déduction du solde...", GRAY);
	if (ask_confirmation(YELLOW "Voulez-vous utiliser des dollars locaux pour cet achat? (O/N) : " RESET))
	{
		if (portfolio->dollar_balance >= total_cost)
		{
			portfolio->dollar_balance -= total_cost;
			char balance_message[100];
			snprintf(balance_message, sizeof(balance_message),
				 "Achat déduit. Nouveau solde : %.2f $", portfolio->dollar_balance);
			print_centered(balance_message, GREEN);
			pause_if_needed(1);  // Pause pour voir le message
		}
		else
		{
			char error_message[150];
			snprintf(error_message, sizeof(error_message),
				 "Erreur: Solde insuffisant (%.2f $ nécessaires, %.2f $ disponibles).",
				 total_cost, portfolio->dollar_balance);
			print_centered(error_message, RED);
			pause_if_needed(1);
			return 0;
		}
	}

	// Demander confirmation avant de créer l'actif
	print_centered("Demande de confirmation pour ajout d'actif...", GRAY);
	if (!ask_confirmation(GRAY "Confirmer l'ajout de l'actif ? (O/N) : " RESET))
	{
		print_centered("Ajout d'actif annulé.", RED);
		pause_if_needed(1);
		return 0;
	}

	// Création de l'actif
	if (!create_asset(portfolio, nom, date, prix, quantite))
	{
		print_centered("Erreur: Impossible d'ajouter l'actif.", RED);
		pause_if_needed(1);
		return 0;
	}
	print_centered("Actif ajouté avec succès!", GREEN);

	return 1;
}

void handle_update_asset(t_portfolio *portfolio)
{
	char nom[15], date[12];
	float nouveau_prix, nouvelle_quantite, montant_dollars;

	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif disponible pour la mise à jour.", RED);
		return;
	}

	// Saisie du nom de l'actif
	input_name(nom, sizeof(nom));
	if (!is_snake_case(nom))
	{
		print_centered("Erreur: Nom invalide. Doit être en snake_case.", RED);
		return;
	}
	// Recherche de l'actif
	t_asset *asset = find_asset_by_name(portfolio, nom);
	if (!asset)
	{
		print_centered("Erreur: Actif non trouvé.", RED);
		return;
	}

	// Demander confirmation avant de réaliser un achat
	if (ask_confirmation(GRAY "Souhaitez-vous réaliser un achat ? (O/N) : " RESET))
	{
		// Saisie du nouveau prix
		print_centered("NOUVEAU PRIX", LIGHT_BLUE);
		nouveau_prix = input_amount();

		// Saisie de la nouvelle quantité
		print_centered("QUANTITE ACHETEE", LIGHT_BLUE);
		nouvelle_quantite = input_amount();

		// Saisie de la date
		input_date(date, sizeof(date));

		// Demander confirmation pour utiliser les dollars locaux
		if (ask_confirmation(GRAY "Souhaitez-vous utiliser des dollars locaux ? (O/N) : " RESET))
		{
			montant_dollars = nouveau_prix * nouvelle_quantite;
			if (portfolio->dollar_balance < montant_dollars)
			{
				char error_message[150];
				snprintf(error_message, sizeof(error_message),
					 "Erreur: Solde insuffisant en dollars locaux (%.2f $ nécessaires, %.2f $ disponibles).",
					 montant_dollars, portfolio->dollar_balance);
				print_centered(error_message, RED);
				return;
			}
			portfolio->dollar_balance -= montant_dollars;
			char balance_message[100];
			snprintf(balance_message, sizeof(balance_message),
				 "Montant de %.2f $ utilisé. Nouveau solde : %.2f $", montant_dollars, portfolio->dollar_balance);
			print_centered(balance_message, GREEN);
		}

		// Demander confirmation avant la mise à jour
		if (!ask_confirmation(GRAY "Confirmer la mise à jour de l'actif avec cet achat ? (O/N) : " RESET))
		{
			print_centered("Mise à jour annulée.", RED);
			return;
		}

		// Mise à jour de l'actif avec un nouvel achat
		update_position(asset, nouveau_prix, nouvelle_quantite, date);
		char success_message[100];
		snprintf(success_message, sizeof(success_message),
			 "Actif '%s' mis à jour avec un nouvel achat.", nom);
		print_centered(success_message, GREEN);
	}
	else
	{
		// Saisie du nouveau prix
		print_centered("NOUVEAU PRIX", LIGHT_BLUE);
		nouveau_prix = input_amount();

		// Saisie de la date
		input_date(date, sizeof(date));

		// Demander confirmation avant la mise à jour du prix
		if (!ask_confirmation(GRAY "Confirmer la mise à jour de l'actif avec ce nouveau prix ? (O/N) : " RESET))
		{
			print_centered("Mise à jour annulée.", RED);
			return;
		}

		// Mise à jour du prix actuel
		update_current_price(asset, nouveau_prix, date);
		char update_message[100];
		snprintf(update_message, sizeof(update_message),
			 "Actif '%s' mis à jour avec un nouveau prix.", nom);
		print_centered(update_message, GREEN);
	}
}

void handle_sell_asset(t_portfolio *portfolio)
{
	char nom[15], date[12];
	float quantite, prix;
	t_asset *asset;

	// Vérification de la validité du portefeuille
	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif disponible pour la vente.", RED);
		return;
	}

	// Saisie du nom de l'actif
	input_name(nom, sizeof(nom));
	if (!is_snake_case(nom))
	{
		print_centered("Erreur: Nom invalide. Doit être en snake_case.", RED);
		return;
	}

	// Recherche de l'actif
	asset = find_asset_by_name(portfolio, nom);
	if (!asset)
	{
		print_centered("Erreur: Actif non trouvé.", RED);
		return;
	}

	// Saisie de la quantité à vendre
	print_centered("QUANTITE A VENDRE", LIGHT_BLUE);
	quantite = input_amount();

	if (quantite > asset->historique[asset->historique_count - 1].quantite)
	{
		char error_message[100];
		snprintf(error_message, sizeof(error_message),
				 "Erreur: Quantité demandée (%.2f) dépasse la quantité disponible (%.2f).",
				 quantite, asset->historique[asset->historique_count - 1].quantite);
		print_centered(error_message, RED);
		return;
	}

	// Saisie du prix de vente
	print_centered("PRIX DE VENTE", LIGHT_BLUE);
	prix = input_amount();

	// Saisie de la date de vente
	input_date(date, sizeof(date));

	// Demander confirmation avant de vendre
	char confirm_message[200];
	snprintf(confirm_message, sizeof(confirm_message),
			 GRAY "Confirmer la vente de %.2f unités de '%s' au prix de %.2f le %s ? (O/N) : " RESET,
			 quantite, nom, prix, date);
	if (!ask_confirmation(confirm_message))
	{
		print_centered("Vente annulée.", RED);
		return;  // Annuler la vente si l'utilisateur refuse
	}

	// Archivage de la vente
	archive_sale(asset, portfolio, quantite, prix, date);

	// Libération des actifs vendus
	free_sold_assets(portfolio);

	// Affichage du message de succès
	char success_message[150];
	snprintf(success_message, sizeof(success_message),
			 "Actif '%s' vendu avec succès. Quantité : %.2f, Prix : %.2f.", nom, quantite, prix);
	print_centered(success_message, GREEN);
}

void handle_calculate_profit(t_portfolio *portfolio)
{
	char nom[15];
	int start_index, end_index;

	// Vérification du portefeuille et de ses actifs
	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur: Aucun actif dans le portefeuille.", RED);
		return;
	}

	// Saisie du nom de l'actif
	input_name(nom, sizeof(nom));

	// Recherche de l'actif
	t_asset *asset = find_asset_by_name(portfolio, nom);
	if (!asset)
	{
		print_centered("Erreur: Actif non trouvé.", RED);
		return;
	}

	// Vérification de l'historique de l'actif
	if (asset->historique_count == 0)
	{
		print_centered("Erreur: Pas d'historique pour cet actif.", RED);
		return;
	}

	// Affichage des dates disponibles
	print_centered("Dates disponibles pour l'actif :", GRAY);
	for (int j = 0; j < asset->historique_count; j++)
	{
		printf(YELLOW "[%d]: %s\n" RESET, j, asset->historique[j].date);
	}

	// Lecture des indices de début et de fin
	print_centered("Sélectionnez l'indice de début :", GRAY);
	start_index = input_choice(0, asset->historique_count - 1);
	print_centered("Sélectionnez l'indice de fin :", GRAY);
	end_index = input_choice(0, asset->historique_count - 1);

	if (start_index > end_index)
	{
		print_centered("Erreur: L'indice de début doit être inférieur ou égal à l'indice de fin.", RED);
		return;
	}

	// Demander confirmation avant de calculer le profit
	if (!ask_confirmation(GRAY "Confirmer le calcul du profit entre ces dates ? (O/N) : " RESET))
	{
		print_centered("Calcul annulé.", RED);
		return;  // Annuler le calcul si l'utilisateur refuse
	}

	// Calcul du profit
	calculate_profit(asset, start_index, end_index);

	pause_if_needed(1);
}

void handle_simulate_asset_sale(t_portfolio *portfolio)
{
	int found = 0;
	int i = 0;
	float quantity_to_sell, current_price, potential_profit;
	char asset_name[50];

	// Vérification si le portefeuille contient des actifs
	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif à simuler.", RED);
		pause_if_needed(1);
		return;
	}

	// Afficher les actifs disponibles
	print_centered("Actifs disponibles pour la simulation :", LIGHT_BLUE);
	display_assets(portfolio);

	// Demander le nom de l’actif
	input_name(asset_name, sizeof(asset_name));

	// Rechercher l’actif
	while (i < portfolio->asset_count && !found)
	{
		if (strcmp(portfolio->assets[i]->nom, asset_name) == 0)
		{
			found = 1;
		}
		else
		{
			i++;
		}
	}

	if (!found)
	{
		print_centered("Erreur : Actif introuvable.", RED);
		pause_if_needed(1);
		return;
	}

	t_asset *asset = portfolio->assets[i];

	// Vérification de l'historique de l'actif
	if (asset->historique_count == 0)
	{
		print_centered("Erreur : Pas de données historiques pour cet actif.", RED);
		pause_if_needed(1);
		return;
	}

	// Demander la quantité à vendre
	print_centered("Quantité à vendre :", LIGHT_BLUE);
	quantity_to_sell = input_amount();
	if (quantity_to_sell <= 0)
	{
		print_centered("Erreur : Quantité invalide.", RED);
		pause_if_needed(1);
		return;
	}

	// Vérifier si la quantité demandée est disponible
	if (quantity_to_sell > asset->historique[asset->historique_count - 1].quantite)
	{
		print_centered("Erreur : Quantité à vendre supérieure à la quantité disponible.", RED);
		pause_if_needed(1);
		return;
	}

	// Demander le prix actuel de l'actif
	print_centered("Prix actuel de l'actif :", LIGHT_BLUE);
	current_price = input_amount();
	if (current_price <= 0)
	{
		print_centered("Erreur : Prix invalide.", RED);
		pause_if_needed(1);
		return;
	}

	// Calculer le profit potentiel
	potential_profit = (current_price - asset->prix_moyen) * quantity_to_sell;

	// Demander confirmation avant de procéder à la simulation de la vente
	if (!ask_confirmation(GRAY "Confirmer la simulation de vente ? (O/N) : " RESET))
	{
		print_centered("Simulation annulée.", RED);
		return;  // Annuler la simulation si l'utilisateur refuse
	}

	// Afficher le résultat de la simulation
	print_centered("Simulation de vente terminée :", GREEN);
	char result_message[200];
	snprintf(result_message, sizeof(result_message),
		"Quantité : %.2f, Prix de vente : %.2f, Profit potentiel : %.2f",
		quantity_to_sell, current_price, potential_profit);
	print_centered(result_message, GREEN);

	pause_if_needed(1);
}
