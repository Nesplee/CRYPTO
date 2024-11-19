/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:00:11 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/18 02:18:30 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

int main(void)
{
    t_portfolio_manager *manager;
    int i = 0;

    // Charger ou créer un gestionnaire de portefeuille
    manager = load_all_portfolios("portfolios.txt");
    if (manager == NULL)
    {
        printf(RED "Erreur: Impossible de créer ou charger le gestionnaire de portefeuille.\n" RESET);
        return (1);
    }

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

    return (0);
}
