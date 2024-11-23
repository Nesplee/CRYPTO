/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2e-HANDLE-assets.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:49:20 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/22 02:26:37 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A4.handle.h"

void handle_delete_asset(t_portfolio *portfolio)
{
	char asset_name[50];

	if (!portfolio || portfolio->asset_count == 0)
	{
		print_centered(RED "Erreur : Aucun actif à supprimer." RESET, RED);
		return;
	}

	// Saisie du nom de l'actif à supprimer
	print_centered(GRAY "Entrez le nom de l'actif à supprimer :" RESET, GRAY);
	input_name(asset_name, sizeof(asset_name));
	if (asset_name[0] == '\0')
	{
		print_centered(RED "Erreur : Nom d'actif invalide." RESET, RED);
		return;
	}

	// Demander confirmation à l'utilisateur
	if (!ask_confirmation("Confirmer la suppression de l'actif ? (O/N)"))
	{
		print_centered("Suppression annulée.", RED);
		return;  // Annuler la suppression si l'utilisateur refuse
	}

	// Appel de la fonction de suppression
	if (delete_asset(portfolio, asset_name))
	{
		print_centered(GREEN "Actif supprimé avec succès." RESET, GREEN);
	}
	else
	{
		print_centered(RED "Erreur : Actif introuvable." RESET, RED);
	}
	pause_if_needed(1); // Attendre que l'utilisateur voie le message avant de continuer

	// Nettoyer l'écran après la confirmation
	clear_screen();
}

