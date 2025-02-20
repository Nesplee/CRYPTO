/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3b.HANDLE-portfolio.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:31:13 by dinguyen          #+#    #+#             */
/*   Updated: 2025/02/20 18:18:47 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A4.handle.h"

void handle_add_dollars(t_portfolio *portfolio, float amount, char *date)
{
		// Vérification des paramètres
		if (!portfolio || !date)
		{
			print_centered("Erreur : Paramètres invalides.", RED);
			return;
		}

		// Initialiser les transactions si elles n'ont jamais été allouées
		if (!portfolio->transactions)
		{
			portfolio->max_transactions = 10; // Taille initiale
			portfolio->transactions = malloc(portfolio->max_transactions * sizeof(t_transaction));
			if (!portfolio->transactions)
			{
				print_centered("Erreur : Impossible d'allouer le tableau des transactions.", RED);
				return;
			}
			portfolio->transaction_count = 0;
			portfolio->dollar_balance = 0; // Réinitialisation du solde
		}

		// Redimensionner le tableau si nécessaire
		if (portfolio->transaction_count >= portfolio->max_transactions)
		{
			if (!resize_transactions(portfolio))
			{
				print_centered("Erreur : Impossible de redimensionner le tableau des transactions.", RED);
				return;
			}
		}

		// Ajouter la transaction
		t_transaction *transaction = &portfolio->transactions[portfolio->transaction_count];
		transaction->date = strdup(date);
		if (!transaction->date)
		{
			print_centered("Erreur : Allocation mémoire échouée pour la date.", RED);
			return;
		}
		transaction->amount = amount;

		// Afficher un message de confirmation
		char message[100];
		snprintf(message, sizeof(message), "Dollars ajoutés : %.2f. Nouveau solde : %.2f.", amount, portfolio->dollar_balance + amount);
		print_centered(message, GREEN);

		// Confirmer l'ajout
		if (!ask_confirmation("Confirmer l'ajout des dollars ? (O/N)"))
		{
			// Annuler si non confirmé
			free(transaction->date);
			print_centered("Ajout annulé par l'utilisateur.", RED);
			return;
		}

		// Mettre à jour le solde uniquement après confirmation
		portfolio->dollar_balance += amount;
		portfolio->transaction_count++;
		print_centered("Ajout confirmé.", GREEN);
}

void handle_withdraw_dollars(t_portfolio *portfolio, float amount, char *date)
{
	// Vérification des paramètres de la fonction
	if (!portfolio || !date || amount <= 0)
	{
		print_centered("Erreur: Paramètres invalides.", RED);
		return;
	}

	// Vérification si le portefeuille est vide (solde à 0)
	if (portfolio->dollar_balance == 0)
	{
		print_centered("Erreur: Portefeuille vide. Retrait impossible.", RED);
		return;
	}

	// Vérifier si le solde est suffisant pour le retrait
	if (portfolio->dollar_balance < amount)
	{
		char message[100];
		snprintf(message, sizeof(message), "Erreur: Solde insuffisant. Disponible: %.2f.", portfolio->dollar_balance);
		print_centered(message, RED);
		return;
	}

	// Ajouter la transaction de retrait
	t_transaction *transaction = &portfolio->transactions[portfolio->transaction_count];
	transaction->date = ft_strdup(date);
	if (!transaction->date)
	{
		print_centered("Erreur: Allocation mémoire échouée pour la date.", RED);
		return;
	}
	transaction->amount = -amount; // Retrait d'argent (négatif)
	portfolio->dollar_balance -= amount; // Mise à jour du solde
	portfolio->transaction_count++;

	// Affichage du message de confirmation de retrait
	char message[100];
	snprintf(message, sizeof(message), "Dollars retirés: %.2f. Nouveau solde: %.2f.", amount, portfolio->dollar_balance);
	print_centered(message, GREEN);

	// Demander confirmation à l'utilisateur
	if (!ask_confirmation("Confirmer le retrait des dollars ? (O/N)"))
	{
		// Annuler le retrait si l'utilisateur refuse
		portfolio->dollar_balance += amount;
		portfolio->transaction_count--;
		free(transaction->date); // Libérer la mémoire allouée pour la date
		print_centered("Retrait annulé.", RED);
		pause_if_needed(1); // Attendre que l'utilisateur voie le message avant de continuer
		return;
	}

	// Confirmation finale
	print_centered("Retrait confirmé.", GREEN);
}

void display_portfolio_short_summary(t_portfolio *portfolio)
{
	int		i;
	char	row[256];
	char	balance[64];

	if (!portfolio)
	{
		print_centered("Erreur : Portefeuille non défini.", RED);
		return;
	}

	sort_assets_by_percentage(portfolio);

	print_centered("======= RÉSUMÉ COURT DU PORTEFEUILLE =======", GRAY);

	// En-tête de tableau centré
	char header[256];
	snprintf(header, sizeof(header), GRAY "%-19s%-12s%-7s" RESET, "Nom de l'actif", "Quantité", "Pourcentage");
	print_centered(header, NULL);

	print_centered(GRAY "-------------------------------------------" RESET, NULL);

	i = 0;
	while (i < portfolio->asset_count)
	{
		t_asset *asset = portfolio->assets[i];
		float total_portfolio_value = calculate_portfolio_value(portfolio);
		float percentage = 0.0f;

		if (total_portfolio_value > 0.0f)
			percentage = (calculate_asset_value(asset) / total_portfolio_value) * 100.0f;

		// Construire la ligne avec le signe `%` collé au pourcentage
		snprintf(row, sizeof(row), BLUE "%-15s" RESET YELLOW "%-15.5f" RESET MAGENTA "%.2f%%" RESET,
				 asset->nom,
				 asset->historique[asset->historique_count - 1].quantite,
				 percentage);

		print_centered(row, NULL);
		i++;
	}

	snprintf(balance, sizeof(balance), "Solde en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	print_centered(balance, NULL);

	print_centered("===========================================", GRAY);
}


void display_portfolio_long_summary(t_portfolio *portfolio)
{
	if (!portfolio)
	{
		print_centered("Erreur : Portefeuille non défini.", RED);
		return;
	}

	print_centered("======= RÉSUMÉ LONG DU PORTEFEUILLE =======", GRAY);

	// Afficher tous les mouvements triés par date
	combine_and_display_movements(portfolio);

	// Fin du résumé
	print_centered("===============================================", GRAY);
}

void display_current_portfolio_state(t_portfolio *portfolio)
{
	if (!portfolio)
	{
		print_centered("Erreur : Portefeuille non défini.", RED);
		return;
	}

	// Largeur fixe des colonnes
	const int col_width = 15;

	// En-tête du tableau
	print_centered("======= ÉTAT ACTUEL DU PORTEFEUILLE =======", GRAY);

	char header[256];
	snprintf(header, sizeof(header), "%-*s %-*s %-*s %-*s %-*s",
			 col_width, "Nom", col_width, "Quantité", col_width, "Prix Moyen",
			 col_width, "Valeur", col_width, "PNL");
	print_centered(header, GRAY);

	print_centered("-----------------------------------------------------------------------      ", GRAY);

	// Variables pour calculer les totaux
	float total_invested = 0.0f;
	float total_value = 0.0f;
	float total_pnl = 0.0f;

	// Parcourir les actifs et afficher leurs informations
	for (int i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		if (!asset)
			continue;

		// Calculer les valeurs
		float last_price = asset->historique[asset->historique_count - 1].prix;
		float quantity = asset->historique[asset->historique_count - 1].quantite;
		float valeur = last_price * quantity;
		float pnl = (last_price - asset->prix_moyen) * quantity;

		// Ajouter aux totaux
		total_invested += asset->prix_moyen * quantity;
		total_value += valeur;
		total_pnl += pnl;

		// Formater les données avec couleurs
		char row[512];
		snprintf(row, sizeof(row),
				 BLUE "%-*s" RESET " " YELLOW "%-*.*f" RESET " " LIGHT_BLUE "%-*.*f" RESET " " MAGENTA "%-*.*f" RESET " %s%-*.2f" RESET,
				 col_width, asset->nom,                         // Nom (BLEU)
				 col_width, 5, quantity,                        // Quantité (JAUNE)
				 col_width, 5, asset->prix_moyen,               // Prix moyen (BLEU CLAIR)
				 col_width, 2, valeur,                          // Valeur (MAGENTA)
				 pnl >= 0 ? GREEN : RED,                        // Couleur pour le PNL
				 col_width, pnl);                               // PNL

		// Afficher la ligne centrée
		print_centered(row, NULL);
	}

	// Ligne de séparation
	print_centered("-----------------------------------------------------------------------      ", GRAY);

	// Ajouter le récapitulatif du portefeuille
	char summary[512];
	snprintf(summary, sizeof(summary),
			 LIGHT_BLUE "%-*s" RESET " " GREEN "$%-*.2f" RESET " " MAGENTA "$%-*.2f" RESET " %s$%-*.2f" RESET,
			 col_width, portfolio->name,               // Nom du portefeuille
			 col_width, total_invested,                // Montant investi (VERT)
			 col_width, total_value,                   // Valeur actuelle (MAGENTA)
			 total_pnl >= 0 ? GREEN : RED,             // Couleur pour le PNL courant
			 col_width, total_pnl);                    // PNL courant

	print_centered(summary, NULL);

	// Ajouter l'historique des transactions
	print_centered("======= HISTORIQUE DES TRANSACTIONS =======", GRAY);
	display_transactions(portfolio);

	// Ajouter l'historique des ventes
	print_centered("========== HISTORIQUE DES VENTES ==========", GRAY);
	display_all_sales(portfolio);

	print_centered("===========================================", GRAY);
}

void	display_all_portfolios_short(t_portfolio_manager *manager)
{
	int		i;

	if (!manager || manager->portfolio_count == 0)
	{
		print_centered("Erreur : Aucun portefeuille à afficher.", RED);
		return;
	}

	print_centered("======= RÉSUMÉ COURT DE TOUS LES PORTEFEUILLES =======", GRAY);

	i = 0;
	while (i < manager->portfolio_count)
	{
	char	header[64];
	snprintf(header, sizeof(header), "Portefeuille %d :", i + 1);
	print_centered(header, BLUE);
	print_centered("-------------------------------------------------------------------------------", GRAY);
	display_portfolio_short_summary(manager->portfolios[i]);
	i++;
	}

	print_centered("=====================================================", GRAY);
}

void	display_all_portfolios_long(t_portfolio_manager *manager)
{
	int		i;

	if (!manager || manager->portfolio_count == 0)
	{
		print_centered("Erreur : Aucun portefeuille à afficher.", RED);
		return;
	}

	print_centered("======= RÉSUMÉ LONG DE TOUS LES PORTEFEUILLES =======", GRAY);

	i = 0;
	while (i < manager->portfolio_count)
	{
	char	header[64];
	snprintf(header, sizeof(header), "Portefeuille %d :", i + 1);
	print_centered(header, BLUE);
	print_centered("-------------------------------------------------------------------------------", GRAY);
	display_portfolio_long_summary(manager->portfolios[i]);
	i++;
	}

	print_centered("=====================================================", GRAY);
}
