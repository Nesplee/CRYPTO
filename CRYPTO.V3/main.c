/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:40:52 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 15:26:31 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "A1.master.h"


/*
int main(int argc, char *argv[])
{
	t_portfolio_manager *manager = NULL;
	int i = 0;

	// Allocation de mémoire pour le gestionnaire de portefeuilles
	manager = (t_portfolio_manager *)malloc(sizeof(t_portfolio_manager));
	if (manager == NULL)
	{
		printf(RED "Erreur: Allocation de mémoire pour le gestionnaire de portefeuille échouée.\n" RESET);
		return 1;
	}

	// Initialisation des valeurs du gestionnaire de portefeuilles
	manager->portfolio_count = 0;
	manager->max_portfolios = 20;
	manager->portfolios = (t_portfolio **)malloc(sizeof(t_portfolio *) * manager->max_portfolios);
	if (manager->portfolios == NULL)
	{
		printf(RED "Erreur: Allocation de mémoire pour les portefeuilles échouée.\n" RESET);
		free(manager);
		return 1;
	}

	// Charger les portefeuilles si un fichier est fourni en argument
	if (argc >= 2)
	{
		printf(GRAY "Début du chargement des portefeuilles...\n" RESET);
		load_all_portfolios(manager, argv[1]);
		printf(GRAY "Chargement des portefeuilles terminé.\n" RESET);
		if (manager->portfolio_count > 0)
		{
			printf(GREEN "Portefeuilles chargés avec succès depuis le fichier : %s\n" RESET, argv[1]);
		}
		else
		{
			printf(YELLOW "Aucun portefeuille trouvé dans le fichier fourni.\n" RESET);
		}
	}
	else
	{
		printf(YELLOW "Aucun fichier de portefeuille fourni. Vous pouvez créer un nouveau portefeuille via le menu.\n" RESET);
	}

	display_welcome_screen();
	// Affichage du menu principal
	main_menu(manager);

	// Libération des portefeuilles
	while (i < manager->portfolio_count)
	{
		free_portfolio(manager->portfolios[i]);
		manager->portfolios[i] = NULL;
		i++;
	}

	// Libération du gestionnaire de portefeuille
	free(manager->portfolios);
	manager->portfolios = NULL;
	free(manager);
	manager = NULL;

	return 0;
}
*/

int main(void)
{
	t_portfolio_manager *manager;
	int i;

	// Allocation de mémoire pour le gestionnaire
	manager = (t_portfolio_manager *)malloc(sizeof(t_portfolio_manager));
	if (!manager)
	{
		printf(RED "Erreur : Allocation de mémoire pour le gestionnaire de portefeuille échouée.\n" RESET);
		return EXIT_FAILURE;
	}

	// Initialisation du gestionnaire
	manager->portfolio_count = 0;

	// Initialisation explicite des pointeurs de portefeuille à NULL
	i = 0;
	while (i < MAX_PORTFOLIOS)
	{
		manager->portfolios[i] = NULL;
		i++;
	}

	// Affichage de l'écran de bienvenue
	display_welcome_screen();

	// Affichage du menu principal
	main_menu(manager);

	// Libération des ressources
	free_portfolio_manager(manager);

	// Retourne un code de succès
	return EXIT_SUCCESS;
}
