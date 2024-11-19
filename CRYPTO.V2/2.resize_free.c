/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.resize_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:32:21 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 02:38:53 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

static void safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}


static void safe_free_array(void ***array, int count)
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

void free_asset(t_point *asset)
{
	if (!asset)
		return;

	safe_free((void **)&asset->nom);  // Cast explicite
	safe_free_array((void ***)&asset->dates, asset->date_count);
	safe_free((void **)&asset->historique_quantite);
	safe_free((void **)&asset->historique_prix);
	safe_free((void **)&asset->historique_diff_begin);
	safe_free((void **)&asset->historique_percent_begin);
	safe_free((void **)&asset->sales);
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

	i = 0;
	while (i < portfolio->asset_count)
	{
		if (portfolio->assets[i])
		{
			free_asset(portfolio->assets[i]);
			portfolio->assets[i] = NULL; // Empêche un double free
		}
		i++;
	}

	free(portfolio->assets);
	portfolio->assets = NULL; // Empêche un double free

	free_transactions(portfolio);
	free(portfolio);
}


void free_transactions(t_portfolio *portfolio)
{
	int i;

	if (!portfolio || !portfolio->transactions)
		return;

	i = 0;
	while (i < portfolio->transaction_count)
	{
		if (portfolio->transactions[i].date)
		{
			free(portfolio->transactions[i].date);
			portfolio->transactions[i].date = NULL; // Empêche un double free
		}
		i++;
	}
	free(portfolio->transactions);
	portfolio->transactions = NULL; // Empêche un double free
}

int resize_arrays(t_point *asset)
{
	int new_size;
	char **new_dates;
	float *new_prix, *new_quantite, *new_diff, *new_percent;

	if (!asset)
	{
		printf(RED "Erreur: resize_arrays appelé avec un actif NULL.\n" RESET);
		return 0;
	}

	new_size = asset->max_dates * 2;
	new_dates = (char **)realloc(asset->dates, sizeof(char *) * new_size);
	new_prix = (float *)realloc(asset->historique_prix, sizeof(float) * new_size);
	new_quantite = (float *)realloc(asset->historique_quantite, sizeof(float) * new_size);
	new_diff = (float *)realloc(asset->historique_diff_begin, sizeof(float) * new_size);
	new_percent = (float *)realloc(asset->historique_percent_begin, sizeof(float) * new_size);

	if (!new_dates || !new_prix || !new_quantite || !new_diff || !new_percent)
	{
		printf(RED "Erreur: Allocation échouée pour l'actif '%s'.\n" RESET, asset->nom);
		free(new_dates);
		free(new_prix);
		free(new_quantite);
		free(new_diff);
		free(new_percent);
		return 0;
	}

	asset->dates = new_dates;
	asset->historique_prix = new_prix;
	asset->historique_quantite = new_quantite;
	asset->historique_diff_begin = new_diff;
	asset->historique_percent_begin = new_percent;
	asset->max_dates = new_size;

	return 1;
}


int resize_portfolio(t_portfolio *portfolio)
{
	int new_size;
	t_point **new_assets;

	if (!portfolio)
	{
		printf(RED "Erreur: resize_portfolio appelé avec un portefeuille NULL.\n" RESET);
		return (0);
	}
	new_size = portfolio->max_assets * 2;
	new_assets = (t_point **)realloc(portfolio->assets, sizeof(t_point) * new_size);
	{
		printf(RED "Erreur: Échec de l'allocation mémoire lors du redimensionnement du portefeuille.\n" RESET);
		return (0);
	}
	portfolio->assets = new_assets;
	portfolio->max_assets = new_size;
	printf(GREEN "Redimensionnement du portefeuille réussi : nouvelles dimensions = %d.\n" RESET, new_size);
	return (1);
}

int resize_sales(t_point *asset)
{
	int new_size;
	t_sale *new_sales;


	if (!asset)
	{
		printf(RED "Erreur: resize_sales appelé avec un actif NULL.\n" RESET);
		return (0);
	}
	new_size = asset->max_sales * 2; // Double la taille.
	new_sales = (t_sale *)realloc(asset->sales, sizeof(t_sale) * new_size);
	if (!new_sales)
	{
		printf(RED "Erreur: Échec de l'allocation mémoire pour les ventes de l'actif '%s'.\n" RESET, asset->nom);
		return (0);
	}
	asset->sales = new_sales;
	asset->max_sales = new_size;
	printf(GREEN "Redimensionnement des ventes réussi : nouvelles dimensions = %d.\n" RESET, new_size);
	return (1);
}

int resize_transactions(t_portfolio *portfolio)
{
	int				new_size;
	t_transaction	*new_transactions;

	if (!portfolio)
		return (0);
	new_size = portfolio->max_transactions * 2;
	new_transactions = (t_transaction *)realloc(portfolio->transactions, sizeof(t_transaction) * new_size);
	if (!new_transactions)
		return (0);
	portfolio->transactions = new_transactions;
	portfolio->max_transactions = new_size;
	return (1);
}

void free_portfolio_manager(t_portfolio_manager *manager)
{
	int i;

	if (!manager)
		return;

	i = 0;
	while (i < manager->portfolio_count)
	{
		free_portfolio(manager->portfolios[i]);
		manager->portfolios[i] = NULL;
		i++;
	}

	free(manager->portfolios);
	manager->portfolios = NULL;
	free(manager);
}
