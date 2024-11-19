/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7.save_load_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:52:58 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 03:53:12 by dinguyen         ###   ########.fr       */
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

/* void save_portfolio(t_portfolio *portfolio, FILE *file)
{
	int i = 0;
	int j;

	fprintf(file, "Balance: %.2f\n", portfolio->dollar_balance);
	fprintf(file, "Nombre total d'actifs: %d\n", portfolio->asset_count);

	while (i < portfolio->asset_count)
	{
		t_point *asset = portfolio->assets[i];
		fprintf(file, "Actif: %s\n", asset->nom);
		fprintf(file, "Prix moyen: %.5f\n", asset->prix_moyen);
		fprintf(file, "Dates: %d\n", asset->date_count);

		j = 0;
		while (j < asset->date_count)
		{
			fprintf(file, "%s %.5f %.5f\n",
					asset->dates[j],
					asset->historique_prix[j],
					asset->historique_quantite[j]);
			j++;
		}
		fprintf(file, "Ventes: %d\n", asset->sale_count);
		j = 0;
		while (j < asset->sale_count)
		{
			t_sale *sale = &asset->sales[j];
			fprintf(file, "%s %.5f %.5f %.2f %.2f\n",
					sale->date,
					sale->quantite_vendue,
					sale->prix_vente,
					sale->profit_loss_exit,
					sale->percent_exit);
			j++;
		}
		i++;
	}
	fprintf(file, "Transactions: %d\n", portfolio->transaction_count);
	i = 0;
	while (i < portfolio->transaction_count)
	{
		t_transaction *transaction = &portfolio->transactions[i];
		fprintf(file, "%s %.2f\n", transaction->date, transaction->amount);
		i++;
	}
}*/

void save_portfolio(t_portfolio *portfolio, FILE *file)
{
	fprintf(file, "Balance:\t%.2f\n", portfolio->dollar_balance);
	fprintf(file, "Nombre total d'actifs:\t%d\n\n", portfolio->asset_count);

	for (int i = 0; i < portfolio->asset_count; i++)
	{
		t_point *asset = portfolio->assets[i];
		fprintf(file, "Actif:\t%s\n", asset->nom);
		fprintf(file, "\tPrix moyen:\t%.5f\n", asset->prix_moyen);
		fprintf(file, "\tDates associées:\t%d\n", asset->date_count);

		for (int j = 0; j < asset->date_count; j++)
		{
			fprintf(file, "\t\t- %s : Prix:\t%.5f, Quantité:\t%.5f\n",
					asset->dates[j],
					asset->historique_prix[j],
					asset->historique_quantite[j]);
		}

		fprintf(file, "\tVentes associées:\t%d\n", asset->sale_count);
		for (int k = 0; k < asset->sale_count; k++)
		{
			t_sale *sale = &asset->sales[k];
			fprintf(file, "\t\t- Date:\t%s, Quantité vendue:\t%.5f, Prix de vente:\t%.5f, Gain:\t%.2f, Pourcentage:\t%.2f\n",
					sale->date,
					sale->quantite_vendue,
					sale->prix_vente,
					sale->profit_loss_exit,
					sale->percent_exit);
		}
	}

	fprintf(file, "Transactions:\t%d\n", portfolio->transaction_count);
	for (int i = 0; i < portfolio->transaction_count; i++)
	{
		fprintf(file, "\t- %s : Montant:\t%.2f\n",
				portfolio->transactions[i].date,
				portfolio->transactions[i].amount);
	}
}

t_portfolio *load_portfolio(FILE *file)
{
	char buffer[256], name[50], date[12];
	float balance, price, quantity, sale_price, sale_profit, sale_percent;
	int asset_count, date_count, sale_count, transaction_count;
	t_portfolio *portfolio;

	// Lecture de la balance
	if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "Balance:\t%f", &balance) != 1)
	{
		printf(RED "Erreur : Impossible de lire la balance.\n" RESET);
		return NULL;
	}

	portfolio = create_portfolio(10);
	if (!portfolio)
	{
		printf(RED "Erreur : Impossible de créer le portefeuille.\n" RESET);
		return NULL;
	}
	portfolio->dollar_balance = balance;

	// Lecture du nombre total d'actifs
	if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "Nombre total d'actifs:\t%d", &asset_count) != 1 || asset_count < 0)
	{
		printf(RED "Erreur : Impossible de lire le nombre total d'actifs.\n" RESET);
		free_portfolio(portfolio);
		return NULL;
	}

	for (int i = 0; i < asset_count; i++)
	{
		// Lecture du nom de l'actif
		if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "Actif:\t%49s", name) != 1)
		{
			printf(RED "Erreur : Impossible de lire le nom de l'actif.\n" RESET);
			free_portfolio(portfolio);
			return NULL;
		}

		// Lecture du prix moyen
		if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "\tPrix moyen:\t%f", &price) != 1 || price < 0)
		{
			printf(RED "Erreur : Impossible de lire le prix moyen pour l'actif '%s'.\n" RESET, name);
			free_portfolio(portfolio);
			return NULL;
		}

		t_point *asset = create_asset(portfolio, name, "", price, 0.0);
		if (!asset)
		{
			printf(RED "Erreur : Impossible de créer l'actif '%s'.\n" RESET, name);
			free_portfolio(portfolio);
			return NULL;
		}

		// Lecture des dates associées
		if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "\tDates associées:\t%d", &date_count) != 1 || date_count < 0)
		{
			printf(RED "Erreur : Impossible de lire les dates pour '%s'.\n" RESET, name);
			free_portfolio(portfolio);
			return NULL;
		}

		for (int j = 0; j < date_count; j++)
		{
			if (!fgets(buffer, sizeof(buffer), file) ||
				sscanf(buffer, "\t\t- %11s : Prix:\t%f, Quantité:\t%f", date, &price, &quantity) != 3)
			{
				printf(RED "Erreur : Données incorrectes pour '%s'.\n" RESET, name);
				free_portfolio(portfolio);
				return NULL;
			}

			asset->dates[asset->date_count] = ft_strdup(date);
			asset->historique_prix[asset->date_count] = price;
			asset->historique_quantite[asset->date_count] = quantity;
			asset->date_count++;
		}

		// Lecture des ventes associées
		if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "\tVentes associées:\t%d", &sale_count) != 1 || sale_count < 0)
		{
			printf(RED "Erreur : Impossible de lire les ventes pour '%s'.\n" RESET, name);
			free_portfolio(portfolio);
			return NULL;
		}

		for (int k = 0; k < sale_count; k++)
		{
			if (!fgets(buffer, sizeof(buffer), file) ||
				sscanf(buffer, "\t\t- Date:\t%11s, Quantité vendue:\t%f, Prix de vente:\t%f, Gain:\t%f, Pourcentage:\t%f",
					   date, &quantity, &sale_price, &sale_profit, &sale_percent) != 5)
			{
				printf(RED "Erreur : Données incorrectes pour une vente de '%s'.\n" RESET, name);
				free_portfolio(portfolio);
				return NULL;
			}

			if (asset->sale_count >= asset->max_sales && !resize_sales(asset))
			{
				printf(RED "Erreur : Impossible de redimensionner les ventes pour '%s'.\n" RESET, name);
				free_portfolio(portfolio);
				return NULL;
			}

			t_sale *sale = &asset->sales[asset->sale_count++];
			sale->date = ft_strdup(date);
			sale->quantite_vendue = quantity;
			sale->prix_vente = sale_price;
			sale->profit_loss_exit = sale_profit;
			sale->percent_exit = sale_percent;
		}
	}

	// Lecture des transactions
	if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "Transactions:\t%d", &transaction_count) != 1 || transaction_count < 0)
	{
		printf(RED "Erreur : Impossible de lire les transactions.\n" RESET);
		free_portfolio(portfolio);
		return NULL;
	}

	for (int i = 0; i < transaction_count; i++)
	{
		if (!fgets(buffer, sizeof(buffer), file) ||
			sscanf(buffer, "\t- %11s : Montant:\t%f", date, &price) != 2)
		{
			printf(RED "Erreur : Données incorrectes pour une transaction.\n" RESET);
			free_portfolio(portfolio);
			return NULL;
		}

		portfolio->transactions[portfolio->transaction_count].date = ft_strdup(date);
		portfolio->transactions[portfolio->transaction_count].amount = price;
		portfolio->transaction_count++;
	}

	return portfolio;
}

/* t_portfolio *load_portfolio(FILE *file)
{
	char buffer[256], name[50], date[12];
	float balance, price, quantity;
	int asset_count, date_count, sale_count, transaction_count;
	t_portfolio *portfolio;

	// Lecture de la balance
	if (!fgets(buffer, sizeof(buffer), file) || sscanf(buffer, "Balance: %f", &balance) != 1)
	{
		printf("Erreur : Impossible de lire la balance.\n");
		return NULL;
	}

	// Création du portefeuille
	portfolio = create_portfolio(10);
	if (!portfolio)
	{
		printf("Erreur : Impossible de créer le portefeuille.\n");
		return NULL;
	}
	portfolio->dollar_balance = balance;

	// Lecture du nombre total d'actifs
	if (fscanf(file, "Nombre total d'actifs: %d\n", &asset_count) != 1 || asset_count < 0)
	{
		printf("Erreur : Impossible de lire le nombre total d'actifs.\n");
		free_portfolio(portfolio);
		return NULL;
	}

	// Lecture des actifs
	for (int i = 0; i < asset_count; i++)
	{
		// Lecture du nom de l'actif
		if (fscanf(file, "Actif: %49s\n", name) != 1)
		{
			printf("Erreur : Impossible de lire le nom de l'actif.\n");
			free_portfolio(portfolio);
			return NULL;
		}

		// Lecture du prix moyen
		if (fscanf(file, "Prix moyen: %f\n", &price) != 1 || price < 0)
		{
			printf("Erreur : Impossible de lire le prix moyen pour l'actif '%s'.\n", name);
			free_portfolio(portfolio);
			return NULL;
		}

		// Création de l'actif
		t_point *asset = create_asset(portfolio, name, "", price, 0.0);
		if (!asset)
		{
			printf("Erreur : Impossible de créer l'actif '%s'.\n", name);
			free_portfolio(portfolio);
			return NULL;
		}

		// Lecture des dates associées
		if (fscanf(file, "Dates: %d\n", &date_count) != 1 || date_count < 0)
		{
			printf("Erreur : Impossible de lire le nombre de dates pour l'actif '%s'.\n", name);
			free_portfolio(portfolio);
			return NULL;
		}

		for (int j = 0; j < date_count; j++)
		{
			// Lecture des dates, prix et quantités
			if (fscanf(file, "%11s %f %f\n", date, &price, &quantity) != 3 || quantity <= 0)
			{
				printf("Erreur : Données incorrectes pour l'actif '%s'.\n", name);
				free_portfolio(portfolio);
				return NULL;
			}

			if (asset->date_count >= asset->max_dates && !resize_arrays(asset))
			{
				printf("Erreur : Impossible de redimensionner les données pour l'actif '%s'.\n", name);
				free_portfolio(portfolio);
				return NULL;
			}

			asset->dates[asset->date_count] = ft_strdup(date);
			asset->historique_prix[asset->date_count] = price;
			asset->historique_quantite[asset->date_count] = quantity;
			asset->date_count++;
		}

		// Lecture des ventes
		if (fscanf(file, "Ventes: %d\n", &sale_count) != 1 || sale_count < 0)
		{
			printf("Erreur : Impossible de lire le nombre de ventes pour l'actif '%s'.\n", name);
			free_portfolio(portfolio);
			return NULL;
		}

		asset->sale_count = sale_count;
	}

	// Lecture des transactions
	if (fscanf(file, "Transactions: %d\n", &transaction_count) != 1 || transaction_count < 0)
	{
		printf("Erreur : Impossible de lire le nombre de transactions.\n");
		free_portfolio(portfolio);
		return NULL;
	}

	portfolio->transaction_count = 0;

	for (int i = 0; i < transaction_count; i++)
	{
		char transaction_date[12];
		float transaction_amount;

		if (portfolio->transaction_count >= portfolio->max_transactions && !resize_transactions(portfolio))
		{
			printf("Erreur : Impossible de redimensionner les transactions.\n");
			free_portfolio(portfolio);
			return NULL;
		}

		if (fscanf(file, "%11s %f\n", transaction_date, &transaction_amount) != 2)
		{
			printf("Erreur : Données incorrectes pour une transaction.\n");
			free_portfolio(portfolio);
			return NULL;
		}

		portfolio->transactions[i].date = ft_strdup(transaction_date);
		portfolio->transactions[i].amount = transaction_amount;
		portfolio->transaction_count++;
	}

	return portfolio;
} */


void save_all_portfolios(t_portfolio_manager *manager, const char *filename)
{
	FILE *file;
	int  i = 0;

	if (!manager || !filename)
		return ;
	file = fopen(filename, "w");
	if (!file)
	{
		perror(RED "Erreur lors de la sauvegarde.\n" RESET);
		return ;
	}
	fprintf(file, "Portefeuilles: %d\n", manager->portfolio_count);
	while (i < manager->portfolio_count)
	{
		save_portfolio(manager->portfolios[i], file); // Sauvegarde chaque portefeuille
		i++;
	}
	fclose(file);
	printf(GREEN "Tous les portefeuilles ont été sauvegardés dans '%s'.\n" RESET, filename);
}

t_portfolio_manager *load_all_portfolios(const char *filename)
{
	FILE *file;
	t_portfolio_manager *manager;
	int portfolio_count;

	file = fopen(filename, "r");
	if (!file)
	{
		printf("Fichier introuvable. Création d'un gestionnaire vide.\n");
		return create_portfolio_manager(5);
	}

	// Lecture du nombre de portefeuilles
	if (fscanf(file, "Portefeuilles: %d\n", &portfolio_count) != 1 || portfolio_count < 0)
	{
		printf("Erreur : Impossible de lire le nombre de portefeuilles.\n");
		fclose(file);
		return NULL;
	}

	manager = create_portfolio_manager(portfolio_count);
	if (!manager)
	{
		printf("Erreur : Impossible de créer le gestionnaire.\n");
		fclose(file);
		return NULL;
	}

	for (int i = 0; i < portfolio_count; i++)
	{
		manager->portfolios[i] = load_portfolio(file);
		if (!manager->portfolios[i])
		{
			printf("Erreur : Impossible de charger le portefeuille %d.\n", i + 1);
			free_portfolio_manager(manager);
			fclose(file);
			return NULL;
		}
		manager->portfolio_count++;
	}

	fclose(file);
	return manager;
}
