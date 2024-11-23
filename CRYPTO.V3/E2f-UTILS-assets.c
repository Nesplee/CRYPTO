/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2f-UTILS-assets.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:56:49 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/21 15:19:04 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A3.portfolio.h"

int delete_asset(t_portfolio *portfolio, const char *asset_name)
{
	int i = 0, j;

	if (!portfolio || portfolio->asset_count == 0)
	{
		return 0; // Pas de message ici, géré au niveau de handle_delete_asset
	}

	// Rechercher l'actif à supprimer
	while (i < portfolio->asset_count)
	{
		if (strcmp(portfolio->assets[i]->nom, asset_name) == 0)
		{
			// Libérer la mémoire associée à l'actif
			free_asset(portfolio->assets[i]);

			// Réorganiser le tableau d'actifs
			j = i;
			while (j < portfolio->asset_count - 1)
			{
				portfolio->assets[j] = portfolio->assets[j + 1];
				j++;
			}
			portfolio->assets[portfolio->asset_count - 1] = NULL;
			portfolio->asset_count--;
			return 1;  // Succès de la suppression
		}
		i++;
	}

	return 0; // Actif introuvable
}

//	Affiche le nom des assets

void display_assets(t_portfolio *portfolio)
{
	int i = 0;

	// Vérifier si aucun actif n'est présent
	if (portfolio->asset_count == 0)
	{
		print_centered("Aucun actif disponible dans ce portefeuille.", RED);
		return;
	}

	// Parcourir les actifs
	while (i < portfolio->asset_count)
	{
		// Accéder à l'actif via le tableau des actifs
		print_centered(portfolio->assets[i]->nom, LIGHT_BLUE);  // Afficher le nom de l'actif
		i++;
	}
}

t_asset *find_asset_by_name(t_portfolio *portfolio, const char *nom)
{
	int i = 0;

	if (!portfolio || !nom)
		return (NULL);

	// Parcourir les actifs du portefeuille pour trouver celui qui correspond au nom
	while (i < portfolio->asset_count)
	{
		if (strcmp(portfolio->assets[i]->nom, nom) == 0)
			return (portfolio->assets[i]);
		i++;
	}

	return (NULL); // Si l'actif n'a pas été trouvé
}
