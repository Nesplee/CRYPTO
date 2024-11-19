/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7.save_load_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:52:58 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 00:14:34 by dinguyen         ###   ########.fr       */
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

void save_portfolio(t_portfolio *portfolio, FILE *file)
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
}

t_portfolio *load_portfolio(FILE *file)
{
	char buffer[256], name[50], date[12];
	float balance, price, quantity;
	int asset_count, date_count, sale_count, transaction_count;
	t_portfolio *portfolio;

	// Lecture de la balance
	if (fgets(buffer, sizeof(buffer), file))
	{
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0';
		}
		if (strncmp(buffer, "Balance:", 8) == 0 && sscanf(buffer + 8, "%f", &balance) == 1)
		{
			portfolio = create_portfolio(10);
			if (!portfolio) return NULL;
			portfolio->dollar_balance = balance;
		}
		else
		{
			printf("Erreur : Impossible de lire la balance.\n");
			return NULL;
		}
	}

	// Lecture du nombre total d'actifs
	if (fscanf(file, "Nombre total d'actifs: %d\n", &asset_count) != 1 || asset_count < 0)
	{
		printf("Erreur : Impossible de lire le nombre total d'actifs.\n");
		free_portfolio(portfolio);
		return NULL;
	}
	portfolio->asset_count = 0;

	int i = 0;
	while (i < asset_count)
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

		// Création de l'actif sans la quantité initiale
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

		int j = 0;
		while (j < date_count)
		{
			// Lecture des dates, prix et quantités
			if (fscanf(file, "%11s %f %f\n", date, &price, &quantity) != 3 || quantity <= 0)
			{
				printf("Erreur : Date ou données incorrectes pour l'actif '%s'.\n", name);
				printf("Date : %s, Prix : %.2f, Quantité : %.2f\n", date, price, quantity);
				free_portfolio(portfolio);
				return NULL;
			}

			if (asset->date_count >= asset->max_dates && !resize_arrays(asset))
			{
				printf("Erreur : Impossible de redimensionner les dates pour l'actif '%s'.\n", name);
				free_portfolio(portfolio);
				return NULL;
			}
			asset->dates[asset->date_count] = ft_strdup(date);
			asset->historique_prix[asset->date_count] = price;
			asset->historique_quantite[asset->date_count] = quantity;
			asset->date_count++;
			j++;
		}

		// Lecture des ventes
		if (fscanf(file, "Ventes: %d\n", &sale_count) != 1 || sale_count < 0)
		{
			printf("Erreur : Impossible de lire le nombre de ventes pour l'actif '%s'.\n", name);
			free_portfolio(portfolio);
			return NULL;
		}
		asset->sale_count = sale_count;

		// Lecture des transactions
		if (fscanf(file, "Transactions: %d\n", &transaction_count) != 1 || transaction_count < 0)
		{
			printf("Erreur : Impossible de lire le nombre de transactions.\n");
			free_portfolio(portfolio);
			return NULL;
		}
		portfolio->transaction_count = transaction_count;
		i++;
	}

	return portfolio;
}


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
		return create_portfolio_manager(5); // On retourne un gestionnaire vide
	}

	// Lecture de la ligne "Portefeuilles:"
	if (fscanf(file, "Portefeuilles: %d\n", &portfolio_count) != 1 || portfolio_count < 0)
	{
		fclose(file);
		return NULL;
	}

	// Crée un gestionnaire pour stocker les portefeuilles
	manager = create_portfolio_manager(portfolio_count);
	if (!manager)
	{
		fclose(file);
		return NULL;
	}

	// Appeler load_portfolio avec le fichier déjà ouvert
	manager->portfolios[0] = load_portfolio(file);
	if (!manager->portfolios[0])
	{
		free(manager);
		fclose(file);
		return NULL;
	}

	manager->portfolio_count = 1;
	fclose(file);
	return manager;
}
