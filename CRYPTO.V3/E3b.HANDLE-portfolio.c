/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3b.HANDLE-portfolio.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:31:13 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 05:12:53 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A4.handle.h"

void handle_add_dollars(t_portfolio *portfolio, float amount, char *date)
{
	// Vérification des paramètres de la fonction
	if (!portfolio)
	{
		print_centered("Erreur : Paramètres invalides.", RED);
		return;
	}

	// Vérifier si le portefeuille a atteint la limite de transactions
	if (portfolio->transaction_count >= portfolio->max_transactions)
	{
		if (!resize_transactions(portfolio))
		{
			print_centered("Erreur: Impossible de redimensionner les transactions.", RED);
			return;
		}
	}

	// Ajouter la transaction
	t_transaction *transaction = &portfolio->transactions[portfolio->transaction_count];
	transaction->date = ft_strdup(date);
	if (!transaction->date)
	{
		print_centered("Erreur: Allocation mémoire échouée pour la date.", RED);
		return;
	}
	transaction->amount = amount;

	// Mise à jour du solde
	portfolio->dollar_balance += amount;
	portfolio->transaction_count++;

	// Afficher le message de confirmation
	char message[100];
	snprintf(message, sizeof(message), "Dollars ajoutés: %.2f. Nouveau solde: %.2f.", amount, portfolio->dollar_balance);
	print_centered(message, GREEN);

	// Demander confirmation après le traitement
	if (!ask_confirmation("Confirmer l'ajout des dollars ? (O/N)"))
	{
		// Annuler si non confirmé (hypothèse : la transaction est réversible)
		portfolio->dollar_balance -= amount;
		portfolio->transaction_count--;
		print_centered("Ajout annulé par l'utilisateur.", RED);
	}
	else
	{
		print_centered("Ajout confirmé.", GREEN);
	}

	// **Supprimer pause_if_needed ici pour éviter redondance**
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

	// Vérification si le portefeuille est valide
	if (!portfolio)
	{
		print_centered("Erreur : Portefeuille non défini.", RED);
		return;
	}

	// Titre centré
	print_centered("======= RÉSUMÉ COURT DU PORTEFEUILLE =======", GRAY);

	// En-tête du tableau centré
	char header[256];
	snprintf(header, sizeof(header), GRAY "%-15s%-15s" RESET, "Nom de l'actif", "Quantité");
	print_centered(header, NULL);

	print_centered(GRAY "-------------------------------------------" RESET, NULL);

	// Parcourir les actifs
	i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i])
		{
			// Vérifiez que l'actif a un historique valide
			if (portfolio->assets[i]->historique_count > 0)
			{
				snprintf(row, sizeof(row), BLUE "%-10s" RESET YELLOW "%-15.5f" RESET,
						 portfolio->assets[i]->nom,
						 portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite);
			}
			else
			{
				snprintf(row, sizeof(row), BLUE "%-15s" RESET RED "Non défini" RESET,
						 portfolio->assets[i]->nom);
			}
			print_centered(row, NULL);
		}
		i++;
	}

	// Afficher le solde en dollars centré
	snprintf(balance, sizeof(balance), "Solde en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	print_centered(balance, NULL);

	// Ligne de séparation finale centrée
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
		print_centered("-------------------------------------------\n", GRAY);
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
		print_centered("-------------------------------------------", GRAY);
		display_portfolio_long_summary(manager->portfolios[i]);
		i++;
	}

	print_centered("=====================================================", GRAY);
}
