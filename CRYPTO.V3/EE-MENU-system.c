/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EE-MENU-system.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:00:53 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 19:59:54 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A5.system.h"

void system_menu(t_portfolio_manager *manager)
{
	int i;
	char filename[256];

	while (1)
	{
		clear_screen();
		display_header("SYSTEM MENU");

		print_centered("1. Sauvegarder un portefeuille", BLUE);
		print_centered("2. Charger un portefeuille", BLUE);
		print_centered("0. Retour au menu principal", GRAY);

		int choice = input_choice(0, 2);
		if (choice == -1)
		{
			print_centered("Choix invalide. Réessayez.", RED);
			pause_if_needed(1);
			continue;
		}

		if (choice == 1) // Sauvegarder un portefeuille
		{
			i = 0;
			while (i < manager->portfolio_count)
			{
				print_centered(manager->portfolios[i]->name, GREEN);
				i++;
			}

			print_centered("Entrez le nom du portefeuille à sauvegarder :", GRAY);
			input_name(filename, sizeof(filename));

			t_portfolio *portfolio = find_portfolio_by_name(manager, filename);
			if (portfolio)
			{
				snprintf(filename, sizeof(filename), "%s.txt", portfolio->name);
				int saved = save_portfolio(portfolio, filename);
				if (saved == 1)
					print_centered("Portefeuille sauvegardé avec succès !", GREEN);
				else
					print_centered("Erreur: Échec de la sauvegarde.", RED);
			}
			else
			{
				print_centered("Erreur: Portefeuille non trouvé.", RED);
			}
		}

		if (choice == 2) // Charger un portefeuille
		{
			if (manager->portfolio_count >= MAX_PORTFOLIOS)
			{
				print_centered("Erreur: Limite maximale de portefeuilles atteinte.", RED);
				continue;
			}

			print_centered("Entrez le nom du fichier à charger :", GRAY);
			input_name(filename, sizeof(filename));
			if (!strchr(filename, '.'))
			{
				strncat(filename, ".txt", sizeof(filename) - strlen(filename) - 1);
			}

			// Créer un portefeuille temporaire pour le chargement
			t_portfolio *new_portfolio = malloc(sizeof(t_portfolio));
			if (!new_portfolio)
			{
				print_centered("Erreur: Mémoire insuffisante pour charger le portefeuille.", RED);
				continue;
			}
			memset(new_portfolio, 0, sizeof(t_portfolio));

			int load_result = load_portfolio(filename, new_portfolio);
			if (load_result == 1)
			{
				manager->portfolios[manager->portfolio_count] = new_portfolio;
				manager->portfolio_count++;
				print_centered("Portefeuille chargé avec succès !", GREEN);
			}
			else
			{
				print_centered("Erreur: Échec du chargement du portefeuille.", RED);
				free(new_portfolio);
			}
		}

		if (choice == 0)
		{
			return;
		}

		pause_if_needed(1);
	}
}


int save_portfolio(const t_portfolio *portfolio, const char *filename)
{
	FILE *file = fopen(filename, "w");
	if (!file)
	{
		printf(RED "Erreur : Impossible d'ouvrir le fichier %s pour écriture.\n" RESET, filename);
		return 0;
	}

	fprintf(file, "{\n");
	if (!save_content(file, portfolio) ||
		!save_assets_history(file, portfolio) ||
		!save_sales_history(file, portfolio) ||
		!save_transactions_history(file, portfolio))
	{
		fclose(file);
		return 0;
	}
	fprintf(file, "}\n");
	fclose(file);
	return 1;
}

int load_portfolio(const char *filename, t_portfolio *portfolio)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }

    char line[1024], buffer[256];
    int assets_loaded = 0;

    printf("DEBUG: Début du chargement du portefeuille...\n");

    // Charger d'abord les métadonnées globales
    while (fgets(line, sizeof(line), file))
    {
        trim_whitespace(line);

        if (sscanf(line, " \"name\": \"%[^\"]\",", buffer) == 1)
        {
            portfolio->name = strdup(buffer);
            if (!portfolio->name)
            {
                printf("Erreur: Impossible d'allouer de la mémoire pour le nom du portefeuille.\n");
                fclose(file);
                return 0;
            }
            printf("DEBUG: Nom du portefeuille chargé : %s\n", portfolio->name);
        }
        else if (sscanf(line, " \"dollar_balance\": %f,", &portfolio->dollar_balance) == 1)
        {
            printf("DEBUG: Solde en dollars chargé : %.2f\n", portfolio->dollar_balance);
        }
        else if (sscanf(line, " \"total_profit_loss\": %f,", &portfolio->total_profit_loss) == 1)
        {
            printf("DEBUG: Total profit/perte chargé : %.2f\n", portfolio->total_profit_loss);
        }

        if (strstr(line, "\"assets\": ["))
        {
            printf("DEBUG: Détection de la section 'assets'.\n");
            assets_loaded = load_assets_history(file, portfolio);
            if (!assets_loaded)
            {
                printf("Erreur: Échec du chargement des actifs.\n");
                fclose(file);
                return 0;
            }
            break; // Passer à la lecture des sections après les métadonnées
        }
    }

    // Charger les autres sections après "assets"
    while (fgets(line, sizeof(line), file))
    {
        trim_whitespace(line);

        if (strstr(line, "\"sales\": ["))
        {
            printf("DEBUG: Détection de la section 'sales'.\n");

            // Pour chaque actif dans le portefeuille, charger son historique des ventes
            for (int i = 0; i < portfolio->asset_count; i++)
            {
                t_asset *asset = portfolio->assets[i];
                if (!load_sales_history(file, asset)) // Charger l'historique des ventes pour chaque actif
                {
                    printf("Erreur: Échec du chargement des ventes pour l'actif %s.\n", asset->nom);
                    fclose(file);
                    return 0;
                }
            }
        }
        else if (strstr(line, "\"transactions\": ["))
        {
            printf("DEBUG: Détection de la section 'transactions'.\n");
            if (!load_transactions_history(file, portfolio))
            {
                printf("Erreur: Échec du chargement des transactions.\n");
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);

    // Résumé final
    printf("DEBUG: Portefeuille chargé avec succès depuis le fichier %s\n", filename);
    printf("DEBUG: Total d'actifs chargés : %d\n", portfolio->asset_count);
    return 1;
}

