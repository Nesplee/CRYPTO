/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2.resize_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 05:32:21 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/18 20:23:51 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

 static void	safe_free(char **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static void safe_free_array(void **array, int count)
{
	int	i;

	if (array == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

void	free_asset(t_point *asset)
{
	int	i;

	if (asset == NULL)
		return ;
	safe_free (&asset->nom);
	safe_free_array((void **)asset->dates, asset->date_count);
	free(asset->historique_quantite);
	free(asset->historique_prix);
	free(asset->historique_diff_begin);
	free(asset->historique_percent_begin);
	i = 0;
	while (i < asset->sale_count)
	{
		safe_free(&asset->sales[i].nom);
		safe_free(&asset->sales[i].date);
		i++;
	}
	free(asset->sales);
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

void	free_portfolio(t_portfolio *portfolio)
{
	int	i;

	if (portfolio == NULL)
		return ;
	i = 0;
	while (i < portfolio->asset_count)
	{
		free_asset(portfolio->assets[i]);
		i++;
	}
	free(portfolio->assets);
	free(portfolio);
}

void free_transactions(t_portfolio *portfolio)
{
	int i;

	if (!portfolio || !portfolio->transactions)
		return ;
	i = 0;
	while (i < portfolio->transaction_count)
	{
		free(portfolio->transactions[i].date);
		i++;
	}
	free(portfolio->transactions);
}

int	resize_arrays(t_point *asset)
{
	int		new_size;
	char	**new_dates;
	float	*new_prix;
	float	*new_quantite;
	float	*new_diff;
	float	*new_percent;

	if (asset == NULL)
	{
		printf(RED "Erreur: resize_arays appelé avec un actif NULL.\n" RESET);
		return (0);
	}
	new_size = asset->max_dates * 2;
	new_dates = (char **)realloc(asset->dates, sizeof(char *) * new_size);
	new_prix = (float *)realloc(asset->historique_prix, sizeof(float) * new_size);
	new_quantite = (float *)realloc(asset->historique_quantite, sizeof(float) * new_size);
	new_diff = (float *)realloc(asset->historique_diff_begin, sizeof(float) * new_size);
	new_percent = (float *)realloc(asset->historique_percent_begin, sizeof(float) * new_size);

	if (!new_dates || !new_prix || !new_quantite || !new_diff || !new_percent)
	{
		printf(RED "Erreur: Echec de l'allocation mémoir dans resize_arrays pour l'actif '%s'.\n" RESET,asset->nom);
		printf(YELLOW "Etapes: \n" RESET);
		if (!new_dates) printf(RED "- dates: Allocation échouée\n" RESET);
		if (!new_prix) printf(RED "- prix: Allocation échouée\n" RESET);
		if (!new_quantite) printf(RED "- quantités: Allocation échouée\n" RESET);
		if (!new_diff) printf(RED "- différences: Allocation échouée\n" RESET);
		if (!new_percent) printf(RED "- pourcentages: Allocation échouée\n" RESET);
		if (new_dates) free(new_dates);
		if (new_prix) free(new_prix);
		if (new_quantite) free(new_quantite);
		if (new_diff) free(new_diff);
		if (new_percent) free(new_percent);
		return (0);
	}
	asset->dates = new_dates;
	asset->historique_prix = new_prix;
	asset->historique_quantite = new_quantite;
	asset->historique_diff_begin = new_diff;
	asset->historique_percent_begin = new_percent;
	asset->max_dates = new_size;
	printf(GREEN "Redimensionnement réussi : nouvelles dimensions = %d.\n" RESET, new_size);
	return (1);
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
	int i = 0;

	if (!manager)
		return;

	while (i < manager->portfolio_count)
	{
		free_portfolio(manager->portfolios[i]);
		i++;
	}

	free(manager->portfolios);
	free(manager);
}
