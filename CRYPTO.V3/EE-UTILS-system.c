/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EE-UTILS-system.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 03:01:52 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 19:55:26 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A5.system.h"

int save_assets_history(FILE *file, const t_portfolio *portfolio)
{
	fprintf(file, "\t\"assets\": [\n");
	for (int i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		fprintf(file, "\t\t{\n");
		fprintf(file, "\t\t\t\"name\": \"%s\",\n", asset->nom ? asset->nom : "null");
		fprintf(file, "\t\t\t\"prix_achat\": %.2f,\n", asset->prix_achat);
		fprintf(file, "\t\t\t\"prix_moyen\": %.2f,\n", asset->prix_moyen);

		// Sauvegarde de l'historique
		fprintf(file, "\t\t\t\"historique\": [\n");
		for (int j = 0; j < asset->historique_count; j++)
		{
			t_history *history = &asset->historique[j];
			fprintf(file, "\t\t\t\t{\n");
			fprintf(file, "\t\t\t\t\t\"date\": \"%s\",\n", history->date ? history->date : "null");
			fprintf(file, "\t\t\t\t\t\"prix\": %.2f,\n", history->prix);
			fprintf(file, "\t\t\t\t\t\"quantite\": %.2f,\n", history->quantite);
			fprintf(file, "\t\t\t\t\t\"diff_begin\": %.2f,\n", history->diff_begin);
			fprintf(file, "\t\t\t\t\t\"percent_begin\": %.2f\n", history->percent_begin);
			if (j < asset->historique_count - 1)
				fprintf(file, "\t\t\t\t},\n");
			else
				fprintf(file, "\t\t\t\t}\n");
		}
		fprintf(file, "\t\t\t],\n");

		// Vérifiez si l'actif est vendu entièrement
		fprintf(file, "\t\t\t\"is_sold_out\": %s\n", asset->is_sold_out ? "true" : "false");

		if (i < portfolio->asset_count - 1)
			fprintf(file, "\t\t},\n");
		else
			fprintf(file, "\t\t}\n");
	}
	fprintf(file, "\t],\n");
	return 1;
}


int save_sales_history(FILE *file, const t_portfolio *portfolio)
{
	int first_sale = 1;

	fprintf(file, "\t\"sales\": [\n");
	for (int i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		for (int j = 0; j < asset->sale_count; j++)
		{
			t_sale *sale = &asset->sales[j];
			if (!first_sale)
				fprintf(file, ",\n");
			else
				first_sale = 0;

			fprintf(file, "\t\t{\n");
			fprintf(file, "\t\t\t\"nom\": \"%s\",\n", sale->nom ? sale->nom : "null");
			fprintf(file, "\t\t\t\"date\": \"%s\",\n", sale->date ? sale->date : "null");
			fprintf(file, "\t\t\t\"quantite_vendue\": %.2f,\n", sale->quantite_vendue);
			fprintf(file, "\t\t\t\"prix_vente\": %.2f,\n", sale->prix_vente);
			fprintf(file, "\t\t\t\"percent_exit\": %.2f,\n", sale->percent_exit);
			fprintf(file, "\t\t\t\"profit_loss_exit\": %.2f\n", sale->profit_loss_exit);
			fprintf(file, "\t\t}");
		}
	}
	fprintf(file, "\n\t],\n");
	return 1;
}

int save_transactions_history(FILE *file, const t_portfolio *portfolio)
{
	fprintf(file, "\t\"transactions\": [\n");
	for (int i = 0; i < portfolio->transaction_count; i++)
	{
		t_transaction *transaction = &portfolio->transactions[i];
		fprintf(file, "\t\t{\n");
		fprintf(file, "\t\t\t\"date\": \"%s\",\n", transaction->date);
		fprintf(file, "\t\t\t\"amount\": %.2f\n", transaction->amount);
		if (i < portfolio->transaction_count - 1)
			fprintf(file, "\t\t},\n");
		else
			fprintf(file, "\t\t}\n");
	}
	fprintf(file, "\t]\n");
	return 1;
}


int save_content(FILE *file, const t_portfolio *portfolio)
{
	fprintf(file, "\t\"name\": \"%s\",\n", portfolio->name);
	fprintf(file, "\t\"dollar_balance\": %.2f,\n", portfolio->dollar_balance);
	fprintf(file, "\t\"last_updated\": \"%s\",\n", portfolio->last_updated);
	fprintf(file, "\t\"total_profit_loss\": %.2f,\n", portfolio->total_profit_loss);
	return 1;
}

int	load_assets_history(FILE *file, t_portfolio *portfolio)
{
	char	line[1024], buffer[256];
	int		asset_index = 0;

	printf("DEBUG: Début du chargement des actifs...\n");

	while (fgets(line, sizeof(line), file))
	{
		trim_whitespace(line);
		if (strstr(line, "]")) // Fin de la section "assets"
		{
			printf("DEBUG: Fin de la section 'assets' détectée.\n");
			break;
		}

		if (strstr(line, "{")) // Début d'un actif
		{
			t_asset *asset = malloc(sizeof(t_asset));
			if (!asset)
			{
				printf("Erreur: Impossible d'allouer un actif.\n");
				return 0;
			}
			memset(asset, 0, sizeof(t_asset));

			// Lire les propriétés de l'actif
			while (fgets(line, sizeof(line), file) && !strstr(line, "}"))
			{
				trim_whitespace(line);
				if (sscanf(line, " \"name\": \"%[^\"]\",", buffer) == 1)
				{
					asset->nom = strdup(buffer);
					printf("DEBUG: Actif chargé : %s\n", asset->nom);
				}
				else if (sscanf(line, " \"prix_achat\": %f,", &asset->prix_achat) == 1)
				{
					printf("DEBUG: Prix d'achat : %.2f\n", asset->prix_achat);
				}
				else if (sscanf(line, " \"prix_moyen\": %f,", &asset->prix_moyen) == 1)
				{
					printf("DEBUG: Prix moyen : %.2f\n", asset->prix_moyen);
				}
				else if (strstr(line, "\"historique\": ["))
				{
					printf("DEBUG: Début de l'historique détecté.\n");

					asset->max_historique = 10;
					asset->historique = malloc(asset->max_historique * sizeof(t_history));
					if (!asset->historique)
					{
						free(asset);
						printf("Erreur: Impossible d'allouer l'historique.\n");
						return 0;
					}
					asset->historique_count = 0;

					while (fgets(line, sizeof(line), file))
					{
						trim_whitespace(line);
						if (strstr(line, "]")) // Fin de l'historique
						{
							printf("DEBUG: Fin de l'historique détectée.\n");
							break;
						}

						if (strstr(line, "{")) // Début d'une entrée
						{
							t_history *history = &asset->historique[asset->historique_count];
							memset(history, 0, sizeof(t_history));

							while (fgets(line, sizeof(line), file) && !strstr(line, "}"))
							{
								trim_whitespace(line);
								if (sscanf(line, " \"date\": \"%[^\"]\",", buffer) == 1)
								{
									history->date = strdup(buffer);
									printf("DEBUG: Date historique : %s\n", history->date);
								}
								else if (sscanf(line, " \"prix\": %f,", &history->prix) == 1)
								{
									printf("DEBUG: Prix historique : %.2f\n", history->prix);
								}
								else if (sscanf(line, " \"quantite\": %f,", &history->quantite) == 1)
								{
									printf("DEBUG: Quantité historique : %.2f\n", history->quantite);
								}
							}
							asset->historique_count++;
						}
					}
				}
			}

			// Ajouter l'actif au portefeuille
			if (asset_index >= portfolio->max_assets)
			{
				portfolio->max_assets *= 2;
				t_asset **new_assets = realloc(portfolio->assets, portfolio->max_assets * sizeof(t_asset *));
				if (!new_assets)
				{
					printf("Erreur: Redimensionnement du tableau d'actifs échoué.\n");
					free(asset);
					return 0;
				}
				portfolio->assets = new_assets;
			}

			portfolio->assets[asset_index++] = asset;
			printf("DEBUG: Actif ajouté avec succès : %s\n", asset->nom);
		}
	}

	portfolio->asset_count = asset_index;
	printf("DEBUG: Nombre total d'actifs chargés : %d\n", portfolio->asset_count);

	return 1;
}

int load_sales_history(FILE *file, t_asset *asset)
{
    if (!file || !asset)
    {
        printf(RED "Erreur: fichier ou actif invalide.\n" RESET);
        return 0;
    }

    char date[11];
    float quantite_vendue, prix_vente, percent_exit;
    while (fscanf(file, "%10s %f %f %f", date, &quantite_vendue, &prix_vente, &percent_exit) == 4)
    {
        // Vérification de la capacité de l'historique des ventes
        if (asset->sale_count >= asset->max_sales)
        {
            if (!resize_sales(asset))
            {
                printf(RED "Erreur: Impossible de redimensionner les ventes pour l'actif %s.\n", asset->nom);
                return 0;
            }
        }

        // Ajouter la vente à l'historique
        asset->sales[asset->sale_count].nom = strdup(date);  // Stockage de la date de la vente
        if (!asset->sales[asset->sale_count].nom)
        {
            printf(RED "Erreur: Allocation mémoire échouée pour le nom de la vente.\n" RESET);
            return 0;
        }

        asset->sales[asset->sale_count].quantite_vendue = quantite_vendue;
        asset->sales[asset->sale_count].prix_vente = prix_vente;
        asset->sales[asset->sale_count].percent_exit = percent_exit;
        asset->sales[asset->sale_count].profit_loss_exit = quantite_vendue * prix_vente * (percent_exit / 100.0);

        asset->sale_count++;
    }

    return 1;
}


int load_transactions_history(FILE *file, t_portfolio *portfolio)
{
    char line[1024], buffer[256];
    int transaction_index = 0;

    printf("DEBUG: Début du chargement des transactions...\n");

    // Vérifier si portfolio->transactions est NULL et l'allouer si nécessaire
    if (portfolio->transactions == NULL)
    {
        portfolio->max_transactions = 10;  // Taille initiale du tableau
        portfolio->transactions = malloc(portfolio->max_transactions * sizeof(t_transaction));
        if (!portfolio->transactions)
        {
            printf("Erreur: Impossible d'allouer de la mémoire pour les transactions.\n");
            return 0;
        }
        printf("DEBUG: Allocation initiale réussie pour les transactions.\n");
    }

    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file))
    {
        trim_whitespace(line);

        // Si on arrive à la fin de la section transactions
        if (strstr(line, "]"))
        {
            printf("DEBUG: Fin de la section 'transactions' détectée.\n");
            break;
        }

        if (strstr(line, "{")) // Début d'une transaction
        {
            t_transaction transaction;
            memset(&transaction, 0, sizeof(t_transaction));  // S'assurer que la transaction est bien initialisée

            // Lire les propriétés de la transaction
            while (fgets(line, sizeof(line), file) && !strstr(line, "}"))
            {
                trim_whitespace(line);
                if (sscanf(line, " \"date\": \"%[^\"]\",", buffer) == 1)
                {
                    transaction.date = strdup(buffer);  // S'assurer qu'on a bien alloué la mémoire pour la date
                    printf("DEBUG: Date de la transaction : %s\n", transaction.date);
                }
                else if (sscanf(line, " \"amount\": %f,", &transaction.amount) == 1)
                {
                    printf("DEBUG: Montant de la transaction : %.2f\n", transaction.amount);
                }
            }

            // Vérifier que nous avons assez de place pour ajouter une transaction
            if (portfolio->transaction_count >= portfolio->max_transactions)
            {
                portfolio->max_transactions *= 2; // Doubler la capacité
                t_transaction *new_transactions = realloc(portfolio->transactions, portfolio->max_transactions * sizeof(t_transaction));
                if (!new_transactions)
                {
                    printf("Erreur: Redimensionnement du tableau des transactions échoué.\n");
                    return 0;
                }
                portfolio->transactions = new_transactions;
                printf("DEBUG: Redimensionnement du tableau des transactions réussi, nouvelle taille : %d\n", portfolio->max_transactions);
            }

            // Ajouter la transaction au tableau
            portfolio->transactions[portfolio->transaction_count++] = transaction;
            transaction_index++;
        }
    }

    // Vérification du nombre total de transactions chargées
    printf("DEBUG: Nombre total de transactions chargées : %d\n", transaction_index);
    return 1;
}



int load_content(FILE *file, t_portfolio *portfolio)
{
	char line[1024], buffer[256];

	while (fgets(line, sizeof(line), file))
	{
		if (strstr(line, "\"assets\": [")) // Si on atteint les actifs, arrêter la lecture
			break;

		if (sscanf(line, " \"dollar_balance\": %f,", &portfolio->dollar_balance) == 1)
			continue;
		else if (sscanf(line, " \"name\": \"%[^\"]\",", buffer) == 1)
			portfolio->name = strdup(buffer);
		else if (sscanf(line, " \"total_profit_loss\": %f,", &portfolio->total_profit_loss) == 1)
			continue;
		else if (sscanf(line, " \"last_updated\": \"%[^\"]\",", buffer) == 1)
			strncpy(portfolio->last_updated, buffer, sizeof(portfolio->last_updated) - 1);
	}

	return 1;
}
