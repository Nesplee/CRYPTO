/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displays.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:30:15 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 16:33:33 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void display_header(void)
{
	printf(BOLD RED);
	printf("==================================================\n");
	printf("          Portfolio Manager v1.0           \n");
	printf("==================================================\n");
	printf(RESET);
	printf(YELLOW "   GERER VOS ACTIFS, SUIVRE VOS PROFITS,\n");
	printf("     ET EXPORTER VOS DONNEES FACILEMENT    \n" RESET);
	printf(BOLD RED "==================================================\n" RESET);

}

void	display_menu(void)
{
	ft_putchar('\n');
	printf(YELLOW);
	print_line('=', 50);
	printf( "                MENU PRINCIPAL\n");
	print_line('=', 50);
	printf(RESET);
	ft_putchar('\n');
	printf(GRAY "1 - " RESET "Ajouter un nouvel actif\n");
	printf(GRAY "2 - " RESET "Mettre à jour un actif existant\n");
	printf(GRAY "3 - " RESET "Vendre un actif\n");
	printf(GRAY "4 - " RESET "Afficher le portefeuille et le résumé des ventes\n");
	printf(GRAY "5 - " RESET "Calculer un profit/déficit\n");
	printf(GRAY "6 - " RESET "Générer un histogramme ASCII\n");
	printf(GRAY "7 - " RESET "Exporter les données d'un actif en CSV\n");
	printf(GRAY "8 - " RESET "Supprimer un actif\n");
	printf(GRAY "9 - " RESET "Charger un portefeuille\n");
	printf(RED "0 - " RESET "Sauvegarder et quitter\n");
	printf(GRAY "\nEntrez votre choix: " RESET);
}

void display_sales_history(t_point *asset)
{
	int i;

	if (asset == NULL || asset->sale_count == 0)
	{
		printf(RED "Aucune vente enregistrée pour l'actif : %s.\n" RESET, asset->nom);
		return;
	}
	printf(BOLD YELLOW "======= HISTORIQUE DES VENTES =======\n" RESET);
	printf(BOLD "%-10s %-12s %-12s %-12s %-12s\n" RESET,
			"Vente", "Date", "Quantité", "Prix de Vente", "PNL");
	printf(GRAY "-------------------------------------------\n" RESET);

	i = 0;
	while (i < asset->sale_count)
	{
		printf("%-10d" YELLOW " [%-12s] " RESET "%-12.5f %-12.5f ",
				i + 1,
				asset->sales[i].date,
				asset->sales[i].quantite_vendue,
				asset->sales[i].prix_vente);

		if (asset->sales[i].profit_loss_exit >= 0)
			printf(GREEN "+%.5f\n" RESET, asset->sales[i].profit_loss_exit);
		else
			printf(RED "%.5f\n" RESET, asset->sales[i].profit_loss_exit);
		i++;
	}
	printf(GRAY "-------------------------------------------\n" RESET);
}


void display_portfolio(t_portfolio *portfolio)
{
	int i;
	t_point *asset;
	float dernier_prix, difference;

	if (portfolio == NULL || portfolio->asset_count == 0)
	{
		printf(RED "Erreur : Aucun actif dans le portefeuille.\n" RESET);
		return;
	}
	printf(BOLD GRAY "======= PORTFOLIO ACTUEL =======\n" RESET);
	printf(BOLD "%-15s %-12s %-12s %-12s %-12s\n" RESET,
			"Nom", "Prix Moyen", "Quantité", "Dernier Prix", "Diff");
	printf(GRAY "==============================================================\n" RESET);

	i = 0;
	while (i < portfolio->asset_count)
	{
		asset = portfolio->assets[i];
		if (asset != NULL && !asset->is_sold_out)
		{
			dernier_prix = asset->historique_prix[asset->date_count - 1];
			difference = dernier_prix - asset->prix_moyen;
			printf("%-15s %-12.5f %-12.5f %-12.5f",
					asset->nom,
					asset->prix_moyen,
					asset->historique_quantite[asset->date_count - 1],
					dernier_prix);
			if (difference >= 0)
				printf(GREEN " +%.5f\n" RESET, difference);
			else
				printf(RED " %.5f\n" RESET, difference);
		}
		i++;
	}
	printf(GRAY "-------------------------------------------\n" RESET);
	printf(BOLD "Balance en dollars : " GREEN "%.2f $\n" RESET, portfolio->dollar_balance);
	printf(GRAY "===========================================\n" RESET);
}


void display_global_summary(t_portfolio *portfolio)
{
	float total_benefits = 0.0, total_losses = 0.0, total_percentage = 0.0;
	int percentage_count = 0, i = 0, j;

	if (!portfolio || portfolio->asset_count == 0)
	{
		printf(RED "Erreur : Aucun actif dans le portefeuille.\n" RESET);
		return;
	}

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
	printf(BOLD GRAY "======= RÉCAPITULATIF GLOBAL =======\n" RESET);
	printf("Total des bénéfices : " GREEN "%.2f $\n" RESET, total_benefits);
	printf("Total des pertes : " RED "%.2f $\n" RESET, total_losses);
	if (percentage_count > 0)
		printf("Pourcentage moyen : " CYAN "%.2f %%\n" RESET, total_percentage / percentage_count);
	else
		printf("Pourcentage moyen : " GRAY "Aucune vente enregistrée.\n" RESET);
	printf(GRAY "===========================================\n" RESET);
	printf(GRAY "======= HISTORIQUE DES VENTES =======\n" RESET);
	i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i]->sale_count > 0)
		{
			printf(GRAY "\nActif : " RESET BOLD YELLOW "%s\n" RESET, portfolio->assets[i]->nom);
			printf(GRAY "-------------------------------------------\n" RESET);
			display_sales_history(portfolio->assets[i]);
		}
		i++;
	}
	printf(GRAY "===========================================\n" RESET);
}

void display_transactions(t_portfolio *portfolio)
{
	int i;

	if (!portfolio || portfolio->transaction_count == 0)
	{
		printf(GRAY "Aucune transaction enregistrée.\n" RESET);
		return ;
	}
	printf(BOLD CYAN "======= HISTORIQUE DES TRANSACTIONS =======\n" RESET);
	printf("%-15s %-15s\n", "Date", "Montant");
	printf(GRAY "===========================================\n" RESET);
	i = 0;
	while (i < portfolio->transaction_count)
	{
		printf(YELLOW "[%-15s] " RESET "%-15.2f\n",
			portfolio->transactions[i].date,
			portfolio->transactions[i].amount);
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
	printf(GRAY "=========================================================\n" RESET);
	printf(GRAY "=============" RESET RED " RÉCAPITULATIF DU PORTEFEUILLE " RESET GRAY "=============\n" RESET);
	printf(GRAY "=========================================================\n" RESET);
	printf("Solde actuel en dollars : " GREEN "%.2f $\n\n" RESET, portfolio->dollar_balance);

	// Section : Liste des actifs
	printf(BOLD GRAY "==================== LISTE DES ACTIFS ====================\n" RESET);
	if (portfolio->asset_count == 0)
		printf(YELLOW "Aucun actif actuellement dans le portefeuille.\n" RESET);
	else
		display_portfolio(portfolio);

	// Section : Ventes
	printf(BOLD GRAY "\n================= HISTORIQUE DES VENTES ==================\n" RESET);
	if (portfolio->asset_count == 0)
		printf(YELLOW "Aucune vente enregistrée.\n" RESET);
	else
	{
		int i = 0;
		while (i < portfolio->asset_count)
		{
			if (portfolio->assets[i]->sale_count > 0)
				display_sales_history(portfolio->assets[i]);
			i++;
		}
	}

	// Section : Transactions
	printf(BOLD GRAY "\n=============== HISTORIQUE DES TRANSACTIONS ===============\n" RESET);
	if (portfolio->transaction_count == 0)
		printf(YELLOW "Aucune transaction enregistrée.\n" RESET);
	else
		display_transactions(portfolio);

	// Fin
	printf(BOLD GRAY "\n==========================================================\n" RESET);
	printf(BOLD GRAY "================== FIN DU RÉCAPITULATIF ==================\n" RESET);
	printf(GRAY "==========================================================\n" RESET);
}
