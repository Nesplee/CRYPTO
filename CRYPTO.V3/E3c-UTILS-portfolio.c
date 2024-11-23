/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3c-UTILS-portfolio.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:33:20 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 15:32:58 by dinguyen         ###   ########.fr       */
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
	printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "Date", "Nom", "Prix Moyen", "Quantité", "Dernier Prix", "PNL");
	printf("--------------------------------------------------------------------------------------------\n");

	i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i])
		{
			// Récupérer le dernier prix, la dernière quantité et la dernière date
			last_price = portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].prix;
			pnl = (last_price - portfolio->assets[i]->prix_moyen) * portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite;

			snprintf(row, sizeof(row), YELLOW "[%s]" RESET " %-15s%-15.5f%-15.5f%-15.5f",
					 portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].date,
					 portfolio->assets[i]->nom,
					 portfolio->assets[i]->prix_moyen,
					 portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite,
					 last_price);

			// Ajout du PNL avec couleur (vert pour bénéfices, rouge pour pertes)
			if (pnl >= 0)
				snprintf(row + strlen(row), sizeof(row) - strlen(row), GREEN " +%.2f" RESET, pnl);
			else
				snprintf(row + strlen(row), sizeof(row) - strlen(row), RED " %.2f" RESET, pnl);

			printf("%s\n", row);
		}
		i++;
	}

	// Afficher le solde en dollars
	snprintf(balance, sizeof(balance), "Solde en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	printf("%s\n", balance);
	print_centered("===========================================", GRAY);
}


//	Affiche les dernieres ventes d'un actif

void display_global_summary(t_portfolio *portfolio)
{
	float total_benefits = 0.0;
	float total_losses = 0.0;
	float total_percentage = 0.0;
	int percentage_count = 0;
	int i = 0;
	int j;

	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif dans le portefeuille.", RED);
		return;
	}

	print_centered("======= RÉCAPITULATIF GLOBAL =======", GRAY);

	while (i < portfolio->asset_count)
	{
		t_asset *asset = portfolio->assets[i];
		if (asset && asset->sale_count > 0)
		{
			j = 0;
			while (j < asset->sale_count)
			{
				if (asset->sales[j].profit_loss_exit > 0)
				{
					total_benefits += asset->sales[j].profit_loss_exit;
				}
				else
				{
					total_losses += asset->sales[j].profit_loss_exit;
				}

				total_percentage += asset->sales[j].percent_exit;
				percentage_count++;
				j++;
			}
		}
		i++;
	}

	char summary[256];
	snprintf(summary, sizeof(summary),
		"Total des bénéfices : " GREEN "%.2f $" RESET "\n"
		"Total des pertes : " RED "%.2f $" RESET,
		total_benefits, total_losses);
	print_centered(summary, GRAY);

	if (percentage_count > 0)
	{
		char percentage[64];
		snprintf(percentage, sizeof(percentage),
			"Pourcentage moyen : " BLUE "%.2f %%" RESET,
			total_percentage / percentage_count);
		print_centered(percentage, GRAY);
	}
	else
	{
		print_centered("Pourcentage moyen : Aucune vente enregistrée.", GRAY);
	}

	print_centered("===========================================", GRAY);
}


//	Affiche les aller retours en $ d'un portefeuille.

void	display_transactions(t_portfolio *portfolio)
{
	int		i = 0;
	char	row[64];

	if (!portfolio || portfolio->transaction_count == 0)
	{
		print_centered("Aucune transaction enregistrée.", GRAY);
		return;
	}

	print_centered("======= HISTORIQUE DES TRANSACTIONS =======", GRAY);
	printf("%-15s%-15s\n", "Date", "Montant");
	printf("-------------------------------------------\n");

	while (i < portfolio->transaction_count)
	{
		snprintf(row, sizeof(row), "%-15s%-15.2f",
			portfolio->transactions[i].date,
			portfolio->transactions[i].amount);
		printf("%s\n", row);
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
