/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3.displays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:30:15 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 03:21:49 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void display_menu_header(const char *title)
{
	int term_width = get_terminal_width();

	char line[term_width + 1];
	memset(line, '=', term_width);
	line[term_width] = '\0';

	print_centered(line); // Ligne supérieure
	print_centered(title); // Titre centré
	print_centered(line); // Ligne inférieure
}


void display_sales_history(t_point *asset)
{
	int i;

	if (asset == NULL || asset->sale_count == 0)
	{
		printf(RED "Aucune vente enregistrée pour l'actif : %s.\n" RESET, asset->nom);
		return;
	}

	print_centered("======= HISTORIQUE DES VENTES =======");
	print_centered("Vente      Date         Quantité    Prix de Vente      PNL");
	print_centered("-------------------------------------------");

	i = 0;
	while (i < asset->sale_count)
	{
		char row[256];
		snprintf(row, sizeof(row), "%-10d %-12s %-12.5f %-12.5f",
				i + 1,
				 asset->sales[i].date,
				 asset->sales[i].quantite_vendue,
				 asset->sales[i].prix_vente);

		if (asset->sales[i].profit_loss_exit >= 0)
			snprintf(row + strlen(row), sizeof(row) - strlen(row),
					 GREEN " +%.5f" RESET, asset->sales[i].profit_loss_exit);
		else
			snprintf(row + strlen(row), sizeof(row) - strlen(row),
					 RED " %.5f" RESET, asset->sales[i].profit_loss_exit);

		print_centered(row);
		i++;
	}
	print_centered("-------------------------------------------");
}

void display_portfolio(t_portfolio *portfolio)
{
	int		i;
	t_point	*asset;
	float	dernier_prix, pnl;
	char	row[256], balance[64];

	if (portfolio == NULL || portfolio->asset_count == 0)
	{
		print_centered(RED "Erreur : Aucun actif dans le portefeuille." RESET);
		return;
	}

	// En-tête centré
	print_centered("======= PORTFOLIO ACTUEL =======");
	print_centered("   Nom           Prix Moyen     Quantité       Dernier Prix        PNL");
	print_centered("===============================================================");

	// Parcours des actifs
	i = 0;
	while (i < portfolio->asset_count)
	{
		asset = portfolio->assets[i];
		if (asset != NULL && !asset->is_sold_out)
		{
			dernier_prix = asset->historique_prix[asset->date_count - 1];
			pnl = (dernier_prix - asset->prix_moyen) * asset->historique_quantite[asset->date_count - 1];

			// Formater la ligne de l'actif
			snprintf(row, sizeof(row), "   %-13s %-15.5f %-15.5f %-18.5f",
					 asset->nom,
					 asset->prix_moyen,
					 asset->historique_quantite[asset->date_count - 1],
					 dernier_prix);

			if (pnl >= 0)
				snprintf(row + strlen(row), sizeof(row) - strlen(row),
						 GREEN "+%.2f" RESET, pnl);
			else
				snprintf(row + strlen(row), sizeof(row) - strlen(row),
						 RED "%.2f" RESET, pnl);

			print_centered(row); // Affiche la ligne centrée
		}
		else
		{
			print_centered(YELLOW "Actif non valide." RESET);
		}
		i++;
	}

	// Pied de tableau
	print_centered("---------------------------------------------------------------");
	snprintf(balance, sizeof(balance), "Balance en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	print_centered(balance);
	print_centered("===============================================================");
}


/*void	display_portfolio(t_portfolio *portfolio)
{
	int		i;
	t_point	*asset;
	float	dernier_prix, pnl;

	if (portfolio == NULL || portfolio->asset_count == 0)
	{
		printf(RED "Erreur : Aucun actif dans le portefeuille.\n" RESET);
		return;
	}

	print_centered("======= PORTFOLIO ACTUEL =======");
	print_centered("Nom            Prix Moyen    Quantité     Dernier Prix    PNL");
	print_centered("==============================================================");

	i = 0;
	while (i < portfolio->asset_count)
	{
		asset = portfolio->assets[i];
		if (asset != NULL && !asset->is_sold_out)
		{
			dernier_prix = asset->historique_prix[asset->date_count - 1];
			pnl = (dernier_prix - asset->prix_moyen) * asset->historique_quantite[asset->date_count - 1];

			char row[256];
			snprintf(row, sizeof(row), "%-15s %-12.5f %-12.5f %-12.5f",
					 asset->nom,
					 asset->prix_moyen,
					 asset->historique_quantite[asset->date_count - 1],
					 dernier_prix);

			if (pnl >= 0)
				snprintf(row + strlen(row), sizeof(row) - strlen(row),
						 GREEN " +%.2f" RESET, pnl);
			else
				snprintf(row + strlen(row), sizeof(row) - strlen(row),
						 RED " %.2f" RESET, pnl);

			print_centered(row);
		}
		else
		{
			printf(YELLOW "Actif non valide à l'index %d.\n" RESET, i);
		}
		i++;
	}

	print_centered("-------------------------------------------");
	char balance[64];
	snprintf(balance, sizeof(balance), "Balance en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	print_centered(balance);
	print_centered("===========================================");
}*/


void display_global_summary(t_portfolio *portfolio)
{
	float total_benefits = 0.0, total_losses = 0.0, total_percentage = 0.0;
	int percentage_count = 0, i = 0, j;

	if (!portfolio || portfolio->asset_count == 0)
	{
		printf(RED "Erreur : Aucun actif dans le portefeuille.\n" RESET);
		return;
	}

	print_centered("======= RÉCAPITULATIF GLOBAL =======");

	while (i < portfolio->asset_count)
	{
		t_point *asset = portfolio->assets[i];
		if (asset && asset->sale_count > 0)
		{
			j = 0;
			while (j < asset->sale_count)
			{
				if (asset->sales[j].profit_loss_exit > 0)
					total_benefits += asset->sales[j].profit_loss_exit;
				else
					total_losses += asset->sales[j].profit_loss_exit;

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
	print_centered(summary);

	if (percentage_count > 0)
	{
		char percentage[64];
		snprintf(percentage, sizeof(percentage), "Pourcentage moyen : " CYAN "%.2f %%" RESET,
					 total_percentage / percentage_count);
		print_centered(percentage);
	}
	else
	{
		print_centered("Pourcentage moyen : " GRAY "Aucune vente enregistrée." RESET);
	}

	print_centered("===========================================");
}

void display_transactions(t_portfolio *portfolio)
{
	int i;

	if (!portfolio || portfolio->transaction_count == 0)
	{
		print_centered("Aucune transaction enregistrée.");
		return;
	}

	print_centered("======= HISTORIQUE DES TRANSACTIONS =======");
	print_centered("Date            Montant");
	print_centered("===========================================");

	i = 0;
	while (i < portfolio->transaction_count)
	{
		char row[64];
		snprintf(row, sizeof(row), "%-15s %-15.2f",
				 portfolio->transactions[i].date,
				 portfolio->transactions[i].amount);
		print_centered(row);
		i++;
	}
}

void display_portfolio_and_summary(t_portfolio *portfolio)
{
	if (!portfolio)
	{
		printf(RED "Erreur : Portefeuille non défini.\n" RESET);
		return;
	}

	// Section : Solde
	print_centered("=========================================================");
	print_centered("============= RÉCAPITULATIF DU PORTEFEUILLE =============");
	print_centered("=========================================================");

	char balance[64];
	snprintf(balance, sizeof(balance), "Solde actuel en dollars : " GREEN "%.2f $" RESET, portfolio->dollar_balance);
	print_centered(balance);
	printf("\n");

	// Section : Liste des actifs
	print_centered("==================== LISTE DES ACTIFS ====================");
	if (portfolio->asset_count == 0)
		print_centered(YELLOW "Aucun actif actuellement dans le portefeuille." RESET);
	else
		display_portfolio(portfolio); // Réutilise votre fonction existante

	// Section : Ventes
	print_centered("\n================= HISTORIQUE DES VENTES ==================");
	if (portfolio->asset_count == 0)
		print_centered(YELLOW "Aucune vente enregistrée." RESET);
	else
	{
		int i = 0;
		while (i < portfolio->asset_count)
		{
			if (portfolio->assets[i]->sale_count > 0)
			{
				char asset_header[64];
				snprintf(asset_header, sizeof(asset_header), "Actif : %s", portfolio->assets[i]->nom);
				print_centered(asset_header);
				print_centered("-------------------------------------------");
				display_sales_history(portfolio->assets[i]);
			}
			i++;
		}
	}

	// Section : Transactions
	print_centered("\n=============== HISTORIQUE DES TRANSACTIONS ===============");
	if (portfolio->transaction_count == 0)
		print_centered(YELLOW "Aucune transaction enregistrée." RESET);
	else
		display_transactions(portfolio); // Réutilise votre fonction existante

	// Fin
	print_centered("\n==========================================================");
	print_centered("================== FIN DU RÉCAPITULATIF ==================");
	print_centered("==========================================================");
}

void display_all_portfolios(t_portfolio_manager *manager)
{
	int i = 0;

	if (!manager || manager->portfolio_count == 0)
	{
		print_centered(RED "Aucun portefeuille à afficher." RESET);
		return;
	}
	print_centered("======= LISTE DES PORTEFEUILLES =======");
	while (i < manager->portfolio_count)
	{
		char header[64];
		snprintf(header, sizeof(header), "Portefeuille %d :", i + 1);
		print_centered(header);

		print_centered("-------------------------------------------");
		display_portfolio(manager->portfolios[i]); // Réutilise votre fonction existante
		i++;
	}
	print_centered("===========================================");
}
