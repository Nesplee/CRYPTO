/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2b-HANDLE-portfolio.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:30:03 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/22 21:22:35 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A4.handle.h"

/*	============================== ET2 HANDLE PORTFOLIO ==============================*/

void handle_create_portfolio(t_portfolio_manager *manager)
{
	char name[50];

	// Saisie du nom du portefeuille
	input_name(name, sizeof(name));

	// Vérification que le nom n'est pas vide
	if (name[0] == '\0' || is_only_spaces(name))
	{
		print_centered("Erreur : Le nom du portefeuille ne peut pas être vide ou composé uniquement d'espaces.", RED);
		return; // Retour immédiat sans pause ici
	}

	// Création du portefeuille
	if (create_portfolio(manager, name))
	{
		// Si la création réussit, afficher un message de succès
		print_centered("Portefeuille créé avec succès !", GREEN);
	}
	else
	{
		// Si la création échoue pour une autre raison
		print_centered("Erreur : Impossible de créer le portefeuille.", RED);
	}
}


void handle_delete_portfolio(t_portfolio_manager *manager)
{
	char portfolio_name[50];

	// Vérification s'il y a des portefeuilles à supprimer
	if (!manager || manager->portfolio_count == 0)
	{
		print_centered(RED "Erreur : Aucun portefeuille à supprimer." RESET, RED);
		return;
	}

	// Demander le nom du portefeuille à supprimer
	input_name(portfolio_name, sizeof(portfolio_name));

	// Vérifier si le portefeuille existe
	t_portfolio *portfolio = find_portfolio_by_name(manager, portfolio_name);
	if (!portfolio)
	{
		// Si le portefeuille n'existe pas, afficher un message d'erreur
		char message[100];
		snprintf(message, sizeof(message), "Erreur: Portefeuille '%s' introuvable.\n", portfolio_name);
		print_centered(message, RED);
		return;
	}

	// Demander confirmation à l'utilisateur
	if (!ask_confirmation("Confirmer la suppression du portefeuille ? (O/N)"))
	{
		print_centered("Suppression annulée.", RED);
		return;  // Annuler la suppression si l'utilisateur refuse
	}

	// Appeler la fonction de suppression
	if (delete_portfolio(manager, portfolio_name))
	{
		print_centered(GREEN "Portefeuille supprimé avec succès." RESET, GREEN);
	}
	else
	{
		print_centered(RED "Erreur : La suppression du portefeuille a échoué." RESET, RED);
	}
	pause_if_needed(1); // Attendre que l'utilisateur voie le message avant de continuer

	// Nettoyer l'écran après la confirmation
	clear_screen();
}
