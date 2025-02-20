/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3f-UTILS-assets.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:00:55 by dinguyen          #+#    #+#             */
/*   Updated: 2025/02/20 18:18:37 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A3.portfolio.h"


/*	============================== UTILS ACTION ==============================*/

t_asset	*create_asset(t_portfolio *portfolio, char *nom, char *date, float prix_achat, float quantite)
{
	// Vérification des paramètres
	if (!portfolio || !nom || !date || prix_achat <= 0 || quantite <= 0)
	{
		print_centered("Erreur: Paramètres invalides pour la création d'actif.", RED);
		pause_if_needed(1);
		return NULL;
	}

	// Allocation du tableau d'actifs si non alloué
	if (!portfolio->assets)
	{
		portfolio->assets = malloc(portfolio->max_assets * sizeof(t_asset *));
		if (!portfolio->assets)
		{
			print_centered("Erreur: Allocation de mémoire pour les actifs échouée.", RED);
			pause_if_needed(1);
			return NULL;
		}
		portfolio->asset_count = 0; // Initialiser le nombre d'actifs à 0
	}

	// Recherche d'un actif existant
	t_asset	*existing_asset = find_asset_by_name(portfolio, nom);
	if (existing_asset)
	{
		print_centered("Actif existant. Mise à jour de la position.", GREEN);
		update_position(existing_asset, prix_achat, quantite, date);
		return existing_asset;
	}

	// Vérification de la capacité du portefeuille
	if (portfolio->asset_count >= portfolio->max_assets)
	{
		if (!resize_portfolio(portfolio))
		{
			print_centered("Erreur: Impossible d'ajouter un nouvel actif. Capacité maximale atteinte.", RED);
			pause_if_needed(1);
			return NULL;
		}
	}

	// Allocation mémoire pour le nouvel actif
	t_asset	*new_asset = (t_asset *)malloc(sizeof(t_asset));
	if (!new_asset)
	{
		print_centered("Erreur: Allocation mémoire échouée pour l'actif.", RED);
		pause_if_needed(1);
		return NULL;
	}

	// Initialisation de base de l'actif
	new_asset->nom = ft_strdup(nom);
	if (!new_asset->nom)
	{
		free(new_asset);
		print_centered("Erreur: Allocation mémoire échouée pour le nom.", RED);
		pause_if_needed(1);
		return NULL;
	}

	// Allocation et initialisation des historiques
	new_asset->historique = malloc(sizeof(t_history) * INITIAL_SIZE);
	if (!new_asset->historique)
	{
		free_asset(new_asset); // Nettoyer toutes les allocations partielles
		print_centered("Erreur: Allocation mémoire échouée pour les historiques de l'actif.", RED);
		pause_if_needed(1);
		return NULL;
	}

	// Initialiser la première entrée historique
	new_asset->historique[0].date = ft_strdup(date);
	if (!new_asset->historique[0].date)
	{
		free_asset(new_asset); // Nettoyer si échec
		print_centered("Erreur: Allocation mémoire échouée pour la date.", RED);
		pause_if_needed(1);
		return NULL;
	}

	new_asset->historique[0].prix = prix_achat;
	new_asset->historique[0].quantite = quantite;
	new_asset->historique[0].diff_begin = 0.0; // Pas de différence au début
	new_asset->historique[0].percent_begin = 0.0; // Pas de pourcentage initial

	// Initialisation des autres champs
	new_asset->prix_achat = prix_achat;
	new_asset->prix_moyen = prix_achat;
	new_asset->historique_count = 1;
	new_asset->max_historique = INITIAL_SIZE; // Taille initiale des historiques
	new_asset->sale_count = 0;
	new_asset->max_sales = INITIAL_SIZE;
	new_asset->sales = malloc(sizeof(t_sale) * INITIAL_SIZE);
	if (!new_asset->sales)
	{
		free_asset(new_asset); // Nettoyer en cas d’échec
		print_centered("Erreur: Allocation mémoire échouée pour les ventes.", RED);
		pause_if_needed(1);
		return NULL;
	}

	new_asset->is_sold_out = 0;

	// Ajout au portefeuille
	portfolio->assets[portfolio->asset_count++] = new_asset;

	return new_asset;
}

void	update_position(t_asset *asset, float nouveau_prix, float nouvelle_quantite, char *nouvelle_date)
{
	// Vérification des paramètres
	if (!asset || !nouvelle_date)
	{
		print_centered("Erreur : Paramètres invalides.", RED);
		return;
	}

	if (nouvelle_quantite <= 0)
	{
		print_centered("Erreur : La nouvelle quantité doit être supérieure à 0.", RED);
		return;
	}

	// Redimensionnement de l'historique si nécessaire
	if (asset->historique_count >= asset->max_historique)
	{
		if (!resize_history(asset))
		{
			print_centered("Erreur : Impossible de redimensionner les tableaux de l'historique.", RED);
			return;
		}
	}

	// Calcul des nouveaux coûts et quantités
	float cout_total_ancien = asset->prix_moyen * asset->historique[asset->historique_count - 1].quantite;
	float cout_total_nouveau = nouveau_prix * nouvelle_quantite;
	float quantite_totale = asset->historique[asset->historique_count - 1].quantite + nouvelle_quantite;

	if (quantite_totale > 0)
	{
		asset->prix_moyen = (cout_total_ancien + cout_total_nouveau) / quantite_totale;
	}
	else
	{
		asset->prix_moyen = 0;
	}

	// Ajouter une nouvelle entrée dans l'historique
	t_history *nouvelle_entree = &asset->historique[asset->historique_count];
	nouvelle_entree->date = ft_strdup(nouvelle_date);
	if (!nouvelle_entree->date)
	{
		print_centered("Erreur : Allocation échouée pour la nouvelle date.", RED);
		return;
	}

	nouvelle_entree->prix = nouveau_prix;
	nouvelle_entree->quantite = quantite_totale;
	nouvelle_entree->diff_begin = (nouveau_prix - asset->prix_achat) * quantite_totale;

	if (asset->prix_achat > 0)
	{
		nouvelle_entree->percent_begin =
			(nouvelle_entree->diff_begin / (asset->prix_achat * quantite_totale)) * 100;
	}
	else
	{
		nouvelle_entree->percent_begin = 0;
	}

	// Incrémenter le compteur de l'historique
	asset->historique_count++;
}

void	update_current_price(t_asset *asset, float update_prix, char *date_suivi)
{
	if (!asset || !date_suivi || update_prix <= 0)
	{
		print_centered("Erreur : Paramètres invalides.", RED);
		return;
	}

	// Redimensionnement de l'historique si nécessaire
	if (asset->historique_count >= asset->max_historique)
	{
		if (!resize_history(asset))
		{
			print_centered("Erreur : Impossible de redimensionner les tableaux de l'historique.", RED);
			return;
		}
	}

	float dernier_prix = asset->historique[asset->historique_count - 1].prix;
	float diff_since_last = update_prix - dernier_prix;
	float percent_since_last = (dernier_prix != 0) ? (diff_since_last / dernier_prix) * 100 : 0;

	// Ajouter une nouvelle entrée dans l'historique
	t_history *nouvelle_entree = &asset->historique[asset->historique_count];
	nouvelle_entree->date = ft_strdup(date_suivi);
	if (!nouvelle_entree->date)
	{
		print_centered("Erreur : Allocation échouée pour la date.", RED);
		return;
	}

	nouvelle_entree->prix = update_prix;
	nouvelle_entree->quantite = asset->historique[asset->historique_count - 1].quantite;
	nouvelle_entree->diff_begin = update_prix - asset->prix_achat;

	if (asset->prix_achat != 0)
	{
		nouvelle_entree->percent_begin = (nouvelle_entree->diff_begin / asset->prix_achat) * 100;
	}
	else
	{
		nouvelle_entree->percent_begin = 0;
	}

	asset->historique_count++;

	// Afficher le pourcentage de progression
	char progress_message[100];
	snprintf(progress_message, sizeof(progress_message), "Pourcentage de progression depuis la dernière MAJ: %.2f%%", percent_since_last);
	print_centered(progress_message, RED);
}

void	calculate_profit(t_asset *asset, int start_index, int end_index)
{
	if (!asset || start_index < 0 || end_index < 0 || start_index >= end_index || end_index >= asset->historique_count)
	{
		print_centered("Erreur : Indices ou actif invalides.", RED);
		return;
	}

	float start_price = asset->historique[start_index].prix;
	float end_price = asset->historique[end_index].prix;
	float quantity = asset->historique[end_index].quantite;
	float diff = (end_price - start_price) * quantity;
	float percent = (start_price * quantity != 0) ? (diff / (start_price * quantity)) * 100 : 0;

	// Affichage des résultats
	char profit_message[256];
	snprintf(profit_message, sizeof(profit_message),
			"Profit/Déficit entre [%s] et [%s]:",
			asset->historique[start_index].date, asset->historique[end_index].date);
	print_centered(profit_message, GRAY);

	if (diff >= 0)
	{
		snprintf(profit_message, sizeof(profit_message), "Différence de valeur: +%.2f $, Pourcentage: +%.2f%%", diff, percent);
		print_centered(profit_message, GREEN);
	}
	else
	{
		snprintf(profit_message, sizeof(profit_message), "Différence de valeur: %.2f $, Pourcentage: %.2f%%", diff, percent);
		print_centered(profit_message, RED);
	}
}

void	archive_sale(t_asset *asset, t_portfolio *portfolio, float quantity, float sale_price, char *sale_date)
{
	if (!asset || !portfolio || !sale_date || quantity <= 0 || sale_price <= 0)
	{
		print_centered("Erreur : Paramètres invalides pour la vente.", RED);
		return;
	}

	if (quantity > asset->historique[asset->historique_count - 1].quantite)
	{
		print_centered("Erreur : Quantité de vente supérieure à la quantité disponible.", RED);
		return;
	}

	if (asset->sale_count >= asset->max_sales)
	{
		if (!resize_sales(asset))
		{
			print_centered("Erreur : Impossible d'enregistrer la vente.", RED);
			return;
		}
	}

	t_sale *sale = &asset->sales[asset->sale_count];
	sale->nom = ft_strdup(asset->nom);
	sale->date = ft_strdup(sale_date);
	sale->quantite_vendue = quantity;
	sale->prix_vente = sale_price;
	sale->profit_loss_exit = (sale_price - asset->prix_moyen) * quantity;

	sale->percent_exit = (asset->prix_moyen != 0) ? ((sale_price - asset->prix_moyen) / asset->prix_moyen) * 100 : 0;

	// Supprimer ou commenter cette ligne
	// portfolio->dollar_balance += quantity * sale_price;

	asset->historique[asset->historique_count - 1].quantite -= quantity;

	if (asset->historique[asset->historique_count - 1].quantite <= 0)
	{
		asset->is_sold_out = 1;
		asset->historique[asset->historique_count - 1].quantite = 0;
	}

	asset->sale_count++;
}
