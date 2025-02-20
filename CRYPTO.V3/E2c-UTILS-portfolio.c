/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2c-UTILS-portfolio.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:31:46 by dinguyen          #+#    #+#             */
/*   Updated: 2025/02/20 18:15:24 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A3.portfolio.h"

int create_portfolio(t_portfolio_manager *manager, char *name)
{
	t_portfolio *new_portfolio;
	int i;

	// Vérification des paramètres
	if (!manager || !name)
	{
		print_centered("Erreur : Paramètres invalides pour la création du portefeuille.", RED);
		return 0;
	}

	// Nettoyer les espaces en début et fin du nom
	trim_whitespace(name);

	// Vérifier si le nom est vide après le nettoyage
	if (name[0] == '\0')
	{
		print_centered("Erreur : Le nom du portefeuille ne peut pas être vide.", RED);
		return 0;
	}

	// Vérifier si un portefeuille avec le même nom existe
	i = 0;
	while (i < manager->portfolio_count)
	{
		if (manager->portfolios[i] && ft_strcmp(manager->portfolios[i]->name, name) == 0)
		{
			print_centered("Erreur : Un portefeuille avec ce nom existe déjà.", RED);
			return 0;
		}
		i++;
	}

	// Créer un nouveau portefeuille
	new_portfolio = (t_portfolio *)malloc(sizeof(t_portfolio));
	if (!new_portfolio)
	{
		print_centered("Erreur : Allocation mémoire échouée pour le portefeuille.", RED);
		return 0;
	}

	// Initialiser les champs du portefeuille
	new_portfolio->name = ft_strdup(name);
	if (!new_portfolio->name)
	{
		free(new_portfolio);
		print_centered("Erreur : Allocation mémoire échouée pour le nom du portefeuille.", RED);
		return 0;
	}

	new_portfolio->assets = malloc(sizeof(t_asset *) * 10);
	if (!new_portfolio->assets)
	{
		free(new_portfolio->name);
		free(new_portfolio);
		print_centered("Erreur : Allocation mémoire échouée pour les actifs.", RED);
		return 0;
	}
	new_portfolio->asset_count = 0;
	new_portfolio->max_assets = 10;
	new_portfolio->dollar_balance = 0.0;

	// Allocation initiale pour les transactions
	new_portfolio->transactions = malloc(sizeof(t_transaction) * 10); // allocation de mémoire
	if (!new_portfolio->transactions)
	{
		free(new_portfolio->assets);
		free(new_portfolio->name);
		free(new_portfolio);
		print_centered("Erreur : Allocation mémoire échouée pour les transactions.", RED);
		return 0;
	}
	new_portfolio->transaction_count = 0;
	new_portfolio->max_transactions = 10; // initialisation correcte

	// Initialiser les autres champs
	snprintf(new_portfolio->last_updated, sizeof(new_portfolio->last_updated), "N/A");
	new_portfolio->total_profit_loss = 0.0;

	// Ajouter le portefeuille au gestionnaire
	manager->portfolios[manager->portfolio_count] = new_portfolio;
	manager->portfolio_count++;

	print_centered("Portefeuille créé avec succès !", GREEN);
	return 1;
}



int delete_portfolio(t_portfolio_manager *manager, const char *portfolio_name)
{
	int i = 0;
	int j;

	// Vérification des paramètres
	if (!manager || manager->portfolio_count == 0)
	{
		return 0;	//	Aucun portefeuille à supprimer
	}

	// Rechercher le portefeuille à supprimer
	while (i < manager->portfolio_count)
	{
		if (strcmp(manager->portfolios[i]->name, portfolio_name) == 0)
		{
			// Libérer la mémoire associée au portefeuille
			free_portfolio(manager->portfolios[i]);

			// Réorganiser le tableau des portefeuilles
			j = i;
			while (j < manager->portfolio_count - 1)
			{
				manager->portfolios[j] = manager->portfolios[j + 1];
				j++;
			}
			manager->portfolios[manager->portfolio_count - 1] = NULL;
			manager->portfolio_count--;

			return 1;	//	Succès
		}
		i++;
	}

	return 0;	//	Portefeuille introuvable
}


//	Affiche le nom de tous les portefeuilles

void display_portfolios(t_portfolio_manager *manager)
{
	int i = 0;

	if (manager->portfolio_count == 0)
	{
		print_centered("Aucun portefeuille disponible.", RED);
		return;
	}

	// Afficher chaque portefeuille, centré
	while (i < manager->portfolio_count)
	{
		if (manager->portfolios[i] != NULL)
		{
			print_centered(manager->portfolios[i]->name, BLUE);
		}
		i++;
	}
}

t_portfolio_manager *create_portfolio_manager()
{
	t_portfolio_manager *manager;
	int i = 0;

	// Allocation de mémoire pour le gestionnaire de portefeuille
	manager = (t_portfolio_manager *)malloc(sizeof(t_portfolio_manager));
	if (!manager)
	{
		printf(RED "Erreur : Allocation mémoire échouée pour le gestionnaire de portefeuille.\n" RESET);
		return NULL;
	}

	// Initialisation des valeurs
	manager->portfolio_count = 0;

	// Initialisation des pointeurs de portefeuille à NULL
	while (i < MAX_PORTFOLIOS)
	{
		manager->portfolios[i] = NULL;
		i++;
	}

	return manager;
}


t_portfolio *get_active_portfolio(t_portfolio_manager *manager)
{
	if (!manager || manager->portfolio_count == 0)
	{
		printf(RED "CHARGEMENT:" RESET " Aucun portefeuille actif trouvé. Manager ou portfolio_count invalide.\n");
		return NULL;
	}

	// Vérifie que le premier portefeuille est valide
	if (!manager->portfolios[0])
	{
		printf(RED "CHARGEMENT:" RESET " Le portefeuille actif (index 0) est NULL.\n");
		return NULL;
	}

	// Vérifie que le portefeuille contient des actifs
	if (manager->portfolios[0]->asset_count == 0)
	{
		printf(RED "CHARGEMENT:" RESET " Le portefeuille actif ne contient aucun actif.\n");
	}

	printf(RED "CHARGEMENT:" RESET " Portefeuille actif trouvé. Nom=%s, Solde=%.2f\n",
		manager->portfolios[0]->name ? manager->portfolios[0]->name : "(nom non défini)",
		manager->portfolios[0]->dollar_balance);

	return manager->portfolios[0];
}
