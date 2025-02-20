/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E4-VALUE.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 18:00:48 by dinguyen          #+#    #+#             */
/*   Updated: 2025/02/20 18:18:40 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A4.handle.h"

float	calculate_asset_value(t_asset *asset)
{
	if (!asset || asset->historique_count == 0)
		return 0.0f; // Si l'actif n'est pas valide ou s'il n'a pas d'historique

	float dernier_prix = asset->historique[asset->historique_count - 1].prix;
	float quantite_totale = asset->historique[asset->historique_count - 1].quantite;

	return dernier_prix * quantite_totale;
}

float	calculate_portfolio_value(t_portfolio *portfolio)
{
	float total_value = 0.0f;
	int i;

	if (!portfolio)
		return 0.0f;

	for (i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		if (asset && asset->historique_count > 0)
		{
			float last_price = asset->historique[asset->historique_count - 1].prix;
			float last_quantity = asset->historique[asset->historique_count - 1].quantite;
			total_value += last_price * last_quantity;
		}
	}

	return total_value;
}


void	display_asset_percentages(t_portfolio *portfolio)
{
	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered("Erreur : Aucun actif dans le portefeuille.", RED);
		return;
	}

	float total_portfolio_value = calculate_portfolio_value(portfolio);
	if (total_portfolio_value == 0.0f)
	{
		print_centered("Erreur : Valeur totale du portefeuille est zéro.", RED);
		return;
	}

	print_centered("======= POURCENTAGE DES ACTIFS =======", GRAY);

	for (int i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		float asset_value = calculate_asset_value(asset);
		float percentage = (asset_value / total_portfolio_value) * 100.0f;

		char row[256];
		snprintf(row, sizeof(row),
				BLUE "%-15s" RESET " : " LIGHT_BLUE "%.2f $" RESET " (" YELLOW "%.2f%%" RESET ")",
				asset->nom, asset_value, percentage);

		print_centered(row, GRAY);
	}

	print_centered("====================================", GRAY);
}


int	compare_assets_by_percentage(const void *a, const void *b)
{
	t_asset *asset_a = *(t_asset **)a;
	t_asset *asset_b = *(t_asset **)b;

	float value_a = calculate_asset_value(asset_a);
	float value_b = calculate_asset_value(asset_b);

	// Tri par ordre décroissant
	if (value_a < value_b)
		return 1;
	if (value_a > value_b)
		return -1;
	return 0;
}

void	sort_assets_by_percentage(t_portfolio *portfolio)
{
	if (!portfolio || portfolio->asset_count <= 1)
		return;

	// Calculer la valeur totale du portefeuille
	float total_value = calculate_portfolio_value(portfolio);

	// Trier les actifs en utilisant leur pourcentage
	qsort(portfolio->assets, portfolio->asset_count, sizeof(t_asset *), compare_assets_by_percentage);

	// Mettre à jour les pourcentages des parts des actifs
	for (int i = 0; i < portfolio->asset_count; i++)
	{
		t_asset *asset = portfolio->assets[i];
		float asset_value = calculate_asset_value(asset);
		asset->historique[asset->historique_count - 1].percent_begin = (asset_value / total_value) * 100.0f;
	}
}

void	update_asset_percentages(t_portfolio *portfolio)
{
	float total_value = 0.0;

	// Calculer la valeur totale des actifs
	for (int i = 0; i < portfolio->asset_count; i++) {
		if (portfolio->assets[i]) {
			total_value += portfolio->assets[i]->prix_moyen *
						portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite;
		}
	}

	// Mettre à jour les pourcentages
	for (int i = 0; i < portfolio->asset_count; i++) {
		if (portfolio->assets[i]) {
			portfolio->assets[i]->sales[portfolio->assets[i]->sale_count - 1].percent_exit =
				(portfolio->assets[i]->prix_moyen *
				portfolio->assets[i]->historique[portfolio->assets[i]->historique_count - 1].quantite / total_value) * 100;
		}
	}
}
