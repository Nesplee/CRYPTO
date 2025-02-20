/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:40:52 by dinguyen          #+#    #+#             */
/*   Updated: 2025/02/20 16:32:33 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "A1.master.h"

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
