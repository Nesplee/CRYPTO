/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3c-UTILS-portfolio.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:33:20 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 05:15:07 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A3.portfolio.h"


//	Affiches les actifs, quantités, dernier prix, PNL + solde en $
void display_portfolio(t_portfolio *portfolio)
{
	int		i;
	char	row[256], balance[64];
	float	last_price, pnl;

	// Vérifier si le portefeuille est valide et contient des actifs
	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif dans le portefeuille.", RED);
		return;
	}

	print_centered("======= LISTE DES ACTIFS =======", GRAY);

	// En-tête de tableau centré
	snprintf(row, sizeof(row), "%-20s%-15s%-15s%-15s%-15s%-10s",
			 "Date", "Nom", "Prix Moyen", "Quantité", "Dernier Prix", "PNL");
	print_centered(row, GRAY);

	print_centered("-------------------------------------------------------------------------------------------", GRAY);

	i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i])
		{
			// Récupérer le dernier prix, la dernière quantité et la dernière date
			last_price = portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].prix;
			pnl = (last_price - portfolio->assets[i]->prix_moyen) *
				  portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite;

			// Chaîne temporaire pour la date avec crochets et couleurs
			char formatted_date[64];
			snprintf(formatted_date, sizeof(formatted_date),
					 YELLOW "[%-10s]" RESET,
					 portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].date);

			// Construire la ligne de l'actif avec les couleurs appropriées
			// Construire la ligne de l'actif avec les couleurs appropriées
		snprintf(row, sizeof(row),
		 YELLOW "%-25s" RESET BLUE "%-15s" RESET LIGHT_BLUE "%-15.5f" RESET YELLOW "%-15.5f" RESET LIGHT_BLUE "%-15.5f" RESET,
         formatted_date,  // Chaîne avec crochets et alignement
         portfolio->assets[i]->nom,
         portfolio->assets[i]->prix_moyen,
         portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite,
         last_price);

			// Ajouter le PNL avec couleur (vert pour bénéfices, rouge pour pertes)
			if (pnl >= 0)
				snprintf(row + strlen(row), sizeof(row) - strlen(row), GREEN " +%.2f" RESET, pnl);
			else
				snprintf(row + strlen(row), sizeof(row) - strlen(row), RED " %.2f" RESET, pnl);

			print_centered(row, GRAY);
		}
		i++;
	}

	// Afficher le solde en dollars centré
	snprintf(balance, sizeof(balance), "Solde en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	print_centered(balance, GRAY);
	print_centered("===========================================", GRAY);
}


//	Affiche les aller retours en $ d'un portefeuille.

void display_transactions(t_portfolio *portfolio)
{
	int i = 0;

	if (!portfolio || portfolio->transaction_count == 0)
	{
		print_centered("Aucune transaction enregistrée.", GRAY);
		return;
	}

	print_centered("======= HISTORIQUE DES TRANSACTIONS =======", GRAY);

	// En-tête du tableau centré
	char header[256];
	snprintf(header, sizeof(header), "%-15s%-15s", "Date", "Montant");
	print_centered(header, GRAY);

	print_centered("-------------------------------------------", GRAY);

	// Boucle pour afficher les transactions
	while (i < portfolio->transaction_count)
	{
		char row[256];

		// Ajouter la date en jaune
		snprintf(row, sizeof(row), YELLOW "%-15s" RESET, portfolio->transactions[i].date);

		// Ajouter le montant avec couleur (vert pour positif, rouge pour négatif)
		if (portfolio->transactions[i].amount >= 0)
		{
			snprintf(row + strlen(row), sizeof(row) - strlen(row), GREEN "%-15.2f" RESET, portfolio->transactions[i].amount);
		}
		else
		{
			snprintf(row + strlen(row), sizeof(row) - strlen(row), RED "%-15.2f" RESET, portfolio->transactions[i].amount);
		}

		// Afficher la ligne centrée
		print_centered(row, GRAY);
		i++;
	}

	print_centered("===========================================", GRAY);
}

//	Trouve un portefeuille par son nom

t_portfolio *find_portfolio_by_name(t_portfolio_manager *manager, const char *name)
{
	int i;

	if (!manager || !name)
	{
		printf(RED "Erreur: Paramètres invalides dans find_portfolio_by_name.\n" RESET);
		return (NULL);
	}

	// Parcourir la liste des portefeuilles pour trouver celui avec le nom spécifié
	i = 0;
	while (i < manager->portfolio_count)
	{
		if (manager->portfolios[i] && strcmp(manager->portfolios[i]->name, name) == 0)
		{
			return (manager->portfolios[i]);
		}
		i++;
	}

	return (NULL);
}

int is_valid_portfolio_name(const char *name)
{
	// Vérifier que le nom n'est pas vide
	if (name[0] == '\0')
		return 0;

	// Vérifier si le nom contient uniquement des lettres et éventuellement des chiffres
	for (int i = 0; name[i] != '\0'; i++)
	{
		if (!isalnum(name[i])) // Accepter uniquement des lettres et des chiffres
			return 0;
	}

	return 1; // Nom valide
}

void display_all_sales(t_portfolio *portfolio)
{
	int i, j;
	char row[256];

	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif ou portefeuille non défini.", RED);
		return;
	}

	print_centered("======= HISTORIQUE DE TOUTES LES VENTES =======", GRAY);
	print_centered("[Date]         Nom            Prix de vente     Quantité       PNL", GRAY);
	print_centered("--------------------------------------------------------------------", GRAY);

	// Parcourir chaque actif du portefeuille
	for (i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		if (asset && asset->sale_count > 0)
		{
			// Parcourir chaque vente de l'actif
			for (j = 0; j < asset->sale_count; j++)
			{
				t_sale sale = asset->sales[j];

				// Construire une ligne de vente avec les couleurs
				snprintf(row, sizeof(row),
					YELLOW "[%s]" RESET " " BLUE "%-15s" RESET LIGHT_BLUE "%-15.2f" RESET YELLOW "%-15.2f" RESET,
					sale.date,
					asset->nom,
					sale.prix_vente,
					sale.quantite_vendue);

				// Ajouter le PNL avec couleur (vert pour bénéfices, rouge pour pertes)
				if (sale.profit_loss_exit >= 0)
					snprintf(row + strlen(row), sizeof(row) - strlen(row), GREEN " +%.2f" RESET, sale.profit_loss_exit);
				else
					snprintf(row + strlen(row), sizeof(row) - strlen(row), RED " %.2f" RESET, sale.profit_loss_exit);

				// Afficher la ligne centrée
				print_centered(row, NULL);
			}
		}
	}

	print_centered("===============================================", GRAY);
}

void combine_and_display_movements(t_portfolio *portfolio)
{
	if (!portfolio)
	{
		print_centered("Erreur : Portefeuille non défini.", RED);
		return;
	}

	int total_count = 0;
	t_movement *movements = combine_and_sort_date(portfolio, &total_count);

	if (!movements || total_count == 0)
	{
		print_centered("Aucun mouvement trouvé.", GRAY);
		return;
	}

	// En-tête du tableau
	print_centered("======= TOUS LES MOUVEMENTS =======", GRAY);

	char header[256];
	snprintf(header, sizeof(header), "%-15s %-15s %-15s %15s %10s %10s",
			 "Date", "Type", "Nom", "Montant", "Prix", "PNL");
	print_centered(header, GRAY);

	print_centered("---------------------------------------------------------------------------------", GRAY);

	// Parcourir et afficher les mouvements
	for (int i = 0; i < total_count; i++)
	{
		char row[256];
		char date_with_brackets[24];
		char pnl_with_color[32]; // Temporaire pour PNL avec couleur

		// Formatage de la date avec crochets
		snprintf(date_with_brackets, sizeof(date_with_brackets),
				 YELLOW "[%-10s]" RESET, movements[i].date);

		// Formatage du PNL avec couleur
		if (movements[i].pnl >= 0)
		{
			snprintf(pnl_with_color, sizeof(pnl_with_color), GREEN "%10.2f" RESET, movements[i].pnl);
		}
		else
		{
			snprintf(pnl_with_color, sizeof(pnl_with_color), RED "%10.2f" RESET, movements[i].pnl);
		}

		// Construire la ligne formatée avec alignement précis
		snprintf(row, sizeof(row), "%-15s %-15s %-15s %15.2f %10.2f %10s",
				 date_with_brackets,
				 movements[i].type,
				 movements[i].nom ? movements[i].nom : "-",
				 movements[i].amount,
				 movements[i].price,
				 pnl_with_color);

		// Afficher la ligne centrée
		print_centered(row, NULL);
	}

	// Libérer la mémoire des mouvements
	free_movements(movements, total_count);

	print_centered("===============================================", GRAY);
}
