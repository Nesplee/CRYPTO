/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EU1-Resize_and_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:11:34 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 20:31:58 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A6.utils.h"

void safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void safe_free_array(void ***array, int count)
{
	int i;

	if (!array || !*array)
		return;
	i = 0;
	while (i < count)
	{
		if ((*array)[i])
		{
			free((*array)[i]);
			(*array)[i] = NULL;
		}
		i++;
	}
	free(*array);
	*array = NULL;
}

void free_asset(t_asset *asset)
{
	int i;

	if (!asset)
	{
		return;
	}

	// Libérer les historiques
	i = 0;
	while (i < asset->historique_count)
	{
		free(asset->historique[i].date);
		i++;
	}
	free(asset->historique);

	// Libérer les ventes
	i = 0;
	while (i < asset->sale_count)
	{
		free(asset->sales[i].nom);
		free(asset->sales[i].date);
		i++;
	}
	free(asset->sales);

	// Libérer le nom de l'actif
	free(asset->nom);

	// Libérer la structure elle-même
	free(asset);
}

void free_sold_assets(t_portfolio *portfolio)
{
	int i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i] != NULL && portfolio->assets[i]->is_sold_out)
		{
			free_asset(portfolio->assets[i]);
			portfolio->assets[i] = NULL;
		}
		i++;
	}
}
void free_portfolio(t_portfolio *portfolio)
{
	int i;

	if (!portfolio)
		return;

	// Libérer chaque actif du portefeuille
	i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i])
		{
			// Utiliser la fonction free_asset pour libérer correctement un actif
			free_asset(portfolio->assets[i]);
			portfolio->assets[i] = NULL;
		}
		i++;
	}

	// Libérer le tableau des actifs
	safe_free((void **)&portfolio->assets);

	// Libérer les transactions
	if (portfolio->transactions)
	{
		i = 0;
		while (i < portfolio->transaction_count)
		{
			safe_free((void **)&portfolio->transactions[i].date);
			i++;
		}
		safe_free((void **)&portfolio->transactions);
	}

	// Libérer le nom du portefeuille
	safe_free((void **)&portfolio->name);

	// Libérer la structure elle-même
	safe_free((void **)&portfolio);
}

void free_transactions(t_portfolio *portfolio)
{
	int i;

	if (!portfolio || !portfolio->transactions)
		return;

	i = 0;
	while (i < portfolio->transaction_count)
	{
		// Libérer la date de chaque transaction si allouée dynamiquement
		safe_free((void **)&portfolio->transactions[i].date);
		i++;
	}

	// Libérer le tableau de transactions
	safe_free((void **)&portfolio->transactions);

	// Réinitialiser le compteur de transactions
	portfolio->transaction_count = 0;
	portfolio->max_transactions = 0;
}

void free_portfolio_manager(t_portfolio_manager *manager)
{
	int i;

	if (!manager)
		return;

	// Libérer chaque portefeuille
	i = 0;
	while (i < manager->portfolio_count)
	{
		if (manager->portfolios[i])
		{
			free_portfolio(manager->portfolios[i]);
			manager->portfolios[i] = NULL; // Éviter un accès futur invalide
		}
		i++;
	}

	// Réinitialiser le gestionnaire
	manager->portfolio_count = 0;

	// Remarque : le tableau `portfolios` est statique, donc aucune libération n'est nécessaire pour ce tableau.
}

int resize_portfolio(t_portfolio *portfolio)
{
    int new_size;
    t_asset **new_assets;

    if (!portfolio)
    {
        printf(RED "Erreur: resize_portfolio appelé avec un portefeuille NULL.\n" RESET);
        return 0;
    }

    printf(GRAY "Tentative de redimensionnement : taille actuelle %d, nouvelle taille %d.\n" RESET,
           portfolio->max_assets, portfolio->max_assets * 2);

    new_size = portfolio->max_assets * 2;
    new_assets = (t_asset **)realloc(portfolio->assets, sizeof(t_asset *) * new_size);

    if (!new_assets)
    {
        printf(RED "Erreur: Échec de l'allocation mémoire lors du redimensionnement du portefeuille.\n" RESET);
        return 0;
    }

    portfolio->assets = new_assets;
    portfolio->max_assets = new_size;

    printf(GREEN "Redimensionnement réussi : nouvelle taille = %d.\n" RESET, new_size);
    return 1;
}


int resize_sales(t_asset *asset)
{
    int new_size = asset->max_sales * 2;
    t_sale *new_sales = realloc(asset->sales, new_size * sizeof(t_sale));
    if (!new_sales)
    {
        printf(RED "Erreur: Impossible de redimensionner les ventes.\n" RESET);
        return 0;
    }

    asset->sales = new_sales;
    asset->max_sales = new_size;
    printf("DEBUG: Redimensionnement du tableau des ventes réussi.\n");

    return 1;
}


int resize_history(t_asset *asset)
{
    t_history *nouvel_historique;
    int nouvelle_taille;

    if (!asset)
    {
        return 0;
    }

    nouvelle_taille = asset->max_historique * 2;
    nouvel_historique = (t_history *)realloc(asset->historique, sizeof(t_history) * nouvelle_taille);
    if (!nouvel_historique)
    {
        printf(RED "Erreur: Échec de redimensionnement de l'historique.\n" RESET);
        return 0;
    }

    asset->historique = nouvel_historique;
    asset->max_historique = nouvelle_taille;

    return 1;
}


int resize_arrays(t_asset *asset)
{
	int nouvelle_taille;
	t_history *nouvel_historique;

	if (!asset)
	{
		printf(RED "Erreur: resize_arrays appelé avec un actif NULL.\n" RESET);
		return 0;
	}

	// Calcul de la nouvelle taille
	nouvelle_taille = asset->max_historique * 2;

	// Redimensionnement de l'historique
	nouvel_historique = (t_history *)realloc(asset->historique, sizeof(t_history) * nouvelle_taille);
	if (!nouvel_historique)
	{
		printf(RED "Erreur: Échec de l'allocation mémoire pour l'historique.\n" RESET);
		return 0;
	}

	// Mise à jour de l'actif
	asset->historique = nouvel_historique;
	asset->max_historique = nouvelle_taille;

	printf(GREEN "Redimensionnement réussi: nouvelle taille = %d.\n" RESET, nouvelle_taille);
	return 1;
}

int resize_transactions(t_portfolio *portfolio)
{
    int new_size;
    t_transaction *new_transactions;

    if (!portfolio)
    {
        return 0;
    }

    new_size = portfolio->max_transactions * 2;
    new_transactions = (t_transaction *)realloc(portfolio->transactions, sizeof(t_transaction) * new_size);
    if (!new_transactions)
    {
        return 0;
    }

    portfolio->transactions = new_transactions;
    portfolio->max_transactions = new_size;

    return 1;
}
