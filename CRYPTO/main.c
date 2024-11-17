/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 22:00:11 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 01:19:34 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

int main(void)
{
	t_portfolio *portfolio;

	// Créer un portefeuille avec une capacité initiale de 10 actifs
	portfolio = create_portfolio(10);
	if (!portfolio)
	{
		printf(RED "Erreur :" RESET " Impossible de créer le portefeuille.\n");
		return (1);
	}

	// Charger les données du portefeuille depuis un fichier, si disponible
	portfolio = load_portfolio("portfolio.txt");
	if (!portfolio)
	{
		printf(YELLOW "Aucun fichier de sauvegarde trouvé. Portefeuille vide initialisé.\n" RESET);
		portfolio = create_portfolio(10);
	}

	// Afficher et gérer le menu
	handle_menu(portfolio);

	// Libérer les ressources et sauvegarder le portefeuille
	free_portfolio(portfolio);
	return (0);
}
