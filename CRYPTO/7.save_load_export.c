/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_load_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:52:58 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 15:57:37 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void export_asset_to_csv(t_point *asset, const char *filename)
{
	FILE *file;
	int i;

	if (!asset || !filename)
	{
		printf(RED "Erreur : Actif ou nom de fichier invalide.\n" RESET);
		return;
	}
	file = fopen(filename, "w");
	if (!file)
	{
		perror(RED "Erreur lors de l'ouverture du fichier" RESET);
		return;
	}

	fprintf(file, "Date, Prix, Quantité\n");
	i = 0;
	while (i < asset->date_count)
	{
		fprintf(file, "%s, %.2f, %.2f\n",
				asset->dates[i],
				asset->historique_prix[i],
				asset->historique_quantite[i]);
		i++;
	}

	fclose(file);
	printf(GREEN "Actif %s exporté avec succès vers %s.\n" RESET, asset->nom, filename);
}


void	save_portfolio(t_portfolio *portfolio, const char *filename)
{
	t_point	*asset;
	t_sale	*sale;
	FILE	*file;
	int		i;
	int		j;

	file = fopen(filename, "w");
	if (file == NULL)
	{
		perror(RED "Erreur lors de la sauvegarde du portefeuille\n" RESET);
		return ;
	}
	fprintf(file, "Balance: %.2f\n", portfolio->dollar_balance);
	fprintf(file, "Nombres total d'actifs: %d\n", portfolio->asset_count);
	i = 0;
	while (i < portfolio->asset_count)
	{
		asset = portfolio->assets[i];
		fprintf(file, "Actifs: %s\n", asset->nom);
		fprintf(file, "Prix Moyen: %.5f\n", asset->prix_moyen);
		fprintf(file, "Dates: %d\n", asset->date_count);

		j = 0;
		while (j < asset->date_count)
		{
			fprintf(file, "%s %.5f %.5f\n", asset->dates[j], asset->historique_prix[j], asset->historique_quantite[j]);
			j++;
		}
		fprintf(file, " Ventes: %d\n", asset->sale_count);
		j = 0;
		while (j < asset->sale_count)
		{
			sale = &asset->sales[j];
			fprintf(file, "%s %.5f %.5f %.2f %.2f\n", sale->date, sale->quantite_vendue, sale->prix_vente, sale->profit_loss_exit, sale->percent_exit);
			j++;
		}
		i++;
	}
	fprintf(file, "Transactions: %d\n", portfolio->transaction_count);
	i = 0;
	while (i < portfolio->transaction_count)
	{
		fprintf(file, "%s %.2f\n", portfolio->transactions[i].date, portfolio->transactions[i].amount);
		i++;
	}
	fclose(file);
	printf(GREEN "Portefeuille sauvegardé dans %s\n" RESET, filename);
}

t_portfolio *load_portfolio(const char *filename)
{
	FILE *file;
	t_portfolio *portfolio;
	t_point *asset;
	t_sale *sale;
	t_transaction *transaction;
	char name[50], date[12];
	float balance, price, quantity, profit_loss_exit, percent_exit, amount;
	int asset_count, date_count, sale_count, transaction_count;
	int i, j;

	file = fopen(filename, "r");
	if (file == NULL)
	{
		printf(YELLOW "Aucun fichier de sauvegarde trouvé. Création d'un nouveau portefeuille.\n" RESET);
		return create_portfolio(10);
	}

	portfolio = create_portfolio(10);
	if (!portfolio)
	{
		fclose(file);
		return NULL;
	}

	// Chargement de la balance
	if (fscanf(file, "Balance: %f\n", &balance) != 1)
	{
		printf(RED "Erreur: Format du fichier incorrect (Balance).\n" RESET);
		fclose(file);
		free_portfolio(portfolio);
		return NULL;
	}
	portfolio->dollar_balance = balance;

	// Chargement des actifs
	if (fscanf(file, "Nombre total d'actifs: %d\n", &asset_count) != 1 || asset_count < 0)
	{
		printf(RED "Erreur: Format du fichier incorrect (Nombre total d'actifs).\n" RESET);
		fclose(file);
		free_portfolio(portfolio);
		return NULL;
	}
	i = 0;
	while (i < asset_count)
	{
		if (fscanf(file, "Actif: %49s\n", name) != 1)
		{
			printf(RED "Erreur: Lecture du nom de l'actif échouée.\n" RESET);
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}

		if (fscanf(file, "Prix moyen: %f\n", &price) != 1 || price < 0)
		{
			printf(RED "Erreur: Format du fichier incorrect (Prix moyen pour %s).\n" RESET, name);
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}

		asset = create_asset(portfolio, name, "", price, 0.0);
		if (!asset)
		{
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}

		if (fscanf(file, "Dates: %d\n", &date_count) != 1 || date_count < 0)
		{
			printf(RED "Erreur: Format du fichier incorrect (Dates pour %s).\n" RESET, name);
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}

		j = 0;
		while (j < date_count)
		{
			if (fscanf(file, "%11s %f %f\n", date, &price, &quantity) != 3 || price < 0 || quantity < 0)
			{
				printf(RED "Erreur: Format du fichier incorrect (Date pour %s).\n" RESET, name);
				fclose(file);
				free_portfolio(portfolio);
				return NULL;
			}
			if (asset->date_count >= asset->max_dates && !resize_arrays(asset))
			{
				printf(RED "Erreur: Échec du redimensionnement des dates pour %s.\n" RESET, name);
				fclose(file);
				free_portfolio(portfolio);
				return NULL;
			}
			asset->dates[asset->date_count] = ft_strdup(date);
			asset->historique_prix[asset->date_count] = price;
			asset->historique_quantite[asset->date_count] = quantity;
			asset->date_count++;
			j++;
		}
		if (fscanf(file, "Ventes: %d\n", &sale_count) != 1 || sale_count < 0)
		{
			printf(RED "Erreur: Format du fichier incorrect (Ventes pour %s).\n" RESET, name);
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}
		j = 0;
		while (j < sale_count)
		{
			if (fscanf(file, "%11s %f %f %f %f\n", date, &quantity, &price, &profit_loss_exit, &percent_exit) != 5)
			{
				printf(RED "Erreur: Format du fichier incorrect (Vente pour %s).\n" RESET, name);
				fclose(file);
				free_portfolio(portfolio);
				return NULL;
			}
			if (asset->sale_count >= asset->max_sales && !resize_sales(asset))
			{
				printf(RED "Erreur: Échec du redimensionnement des ventes pour %s.\n" RESET, name);
				fclose(file);
				free_portfolio(portfolio);
				return NULL;
			}
			sale = &asset->sales[asset->sale_count];
			sale->date = ft_strdup(date);
			sale->quantite_vendue = quantity;
			sale->prix_vente = price;
			sale->profit_loss_exit = profit_loss_exit;
			sale->percent_exit = percent_exit;
			asset->sale_count++;
			j++;
		}
		i++;
	}

	// Chargement des transactions
	if (fscanf(file, "Transactions: %d\n", &transaction_count) != 1 || transaction_count < 0)
	{
		printf(RED "Erreur: Format du fichier incorrect (Transactions).\n" RESET);
		fclose(file);
		free_portfolio(portfolio);
		return NULL;
	}

	portfolio->transaction_count = 0;
	while (portfolio->transaction_count < transaction_count)
	{
		if (portfolio->transaction_count >= portfolio->max_transactions && !resize_transactions(portfolio))
		{
			printf(RED "Erreur: Échec du redimensionnement des transactions.\n" RESET);
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}

		transaction = &portfolio->transactions[portfolio->transaction_count];
		if (fscanf(file, "%11s %f\n", date, &amount) != 2)
		{
			printf(RED "Erreur: Format du fichier incorrect (Transaction).\n" RESET);
			fclose(file);
			free_portfolio(portfolio);
			return NULL;
		}
		transaction->date = ft_strdup(date);
		transaction->amount = amount;
		portfolio->transaction_count++;
	}

	fclose(file);
	printf(GREEN "Portefeuille chargé avec succès depuis %s\n" RESET, filename);
	return portfolio;
}
