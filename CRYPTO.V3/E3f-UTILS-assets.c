/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E3f-UTILS-assets.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 02:00:55 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 19:50:20 by dinguyen         ###   ########.fr       */
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

void update_position(t_asset *asset, float nouveau_prix, float nouvelle_quantite, char *nouvelle_date)
{
	float cout_total_ancien;
	float cout_total_nouveau;
	float quantite_totale;

	// Vérification des paramètres
	if (!asset || !nouvelle_date)
	{
		printf(RED "Erreur:" RESET " Paramètres invalides.\n");
		return;
	}

	if (nouvelle_quantite <= 0)
	{
		printf(RED "Erreur:" RESET " La nouvelle quantité doit être supérieure à 0.\n");
		return;
	}

	// Redimensionnement de l'historique si nécessaire
	if (asset->historique_count >= asset->max_historique)
	{
		if (!resize_history(asset))
		{
			printf(RED "Erreur:" RESET " Impossible de redimensionner les tableaux de l'historique.\n");
			return;
		}
	}

	// Calcul des nouveaux coûts et quantités
	cout_total_ancien = asset->prix_moyen * asset->historique[asset->historique_count - 1].quantite;
	cout_total_nouveau = nouveau_prix * nouvelle_quantite;
	quantite_totale = asset->historique[asset->historique_count - 1].quantite + nouvelle_quantite;

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
		printf(RED "Erreur:" RESET " Allocation échouée pour la nouvelle date.\n");
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


void update_current_price(t_asset *asset, float update_prix, char *date_suivi)
{
	float dernier_prix;
	float diff_since_last;
	float percent_since_last;

	// Vérification des paramètres
	if (!asset || !date_suivi || update_prix <= 0)
	{
		printf(RED "Erreur:" RESET " Paramètres invalides.\n");
		return;
	}

	// Redimensionnement de l'historique si nécessaire
	if (asset->historique_count >= asset->max_historique)
	{
		if (!resize_history(asset))
		{
			printf(RED "Erreur:" RESET " Impossible de redimensionner les tableaux de l'historique.\n");
			return;
		}
	}

	// Récupération du dernier prix
	dernier_prix = asset->historique[asset->historique_count - 1].prix;
	diff_since_last = update_prix - dernier_prix;
	if (dernier_prix != 0)
	{
		percent_since_last = (diff_since_last / dernier_prix) * 100;
	}
	else
	{
		percent_since_last = 0;
	}

	// Ajouter une nouvelle entrée dans l'historique
	t_history *nouvelle_entree = &asset->historique[asset->historique_count];
	nouvelle_entree->date = ft_strdup(date_suivi);
	if (!nouvelle_entree->date)
	{
		printf(RED "Erreur:" RESET " Allocation échouée pour la date.\n");
		return;
	}

	nouvelle_entree->prix = update_prix;
	nouvelle_entree->quantite = asset->historique[asset->historique_count - 1].quantite;
	nouvelle_entree->diff_begin = update_prix - asset->prix_achat;

	if (asset->prix_achat != 0)
	{
		nouvelle_entree->percent_begin =
			(nouvelle_entree->diff_begin / asset->prix_achat) * 100;
	}
	else
	{
		nouvelle_entree->percent_begin = 0;
	}

	// Incrémenter le compteur de l'historique
	asset->historique_count++;

	printf("Pourcentage de progression depuis la dernière MAJ: %.2f%%\n", percent_since_last);
}

void calculate_profit(t_asset *asset, int start_index, int end_index)
{
	float start_price;
	float end_price;
	float quantity;
	float diff;
	float percent;

	// Vérification des paramètres
	if (!asset || start_index < 0 || end_index < 0 || start_index >= end_index || end_index >= asset->historique_count)
	{
		printf(RED "Erreur:" RESET " Indices ou actif invalides.\n");
		return;
	}

	// Récupération des valeurs historiques
	start_price = asset->historique[start_index].prix;
	end_price = asset->historique[end_index].prix;
	quantity = asset->historique[end_index].quantite;
	diff = (end_price - start_price) * quantity;

	// Calcul du pourcentage de différence
	if (start_price * quantity != 0)
	{
		percent = (diff / (start_price * quantity)) * 100;
	}
	else
	{
		percent = 0;
	}

	// Affichage des résultats
	printf("Profit/Déficit entre " YELLOW "[%s]" RESET " et " YELLOW "[%s]:" RESET "\n",
		   asset->historique[start_index].date, asset->historique[end_index].date);

	if (diff >= 0)
	{
		printf(GREEN "Différence de valeur: +%.2f $\nPourcentage: +%.2f%%\n" RESET, diff, percent);
	}
	else
	{
		printf(RED "Différence de valeur: %.2f $\nPourcentage: %.2f%%\n" RESET, diff, percent);
	}
}
void archive_sale(t_asset *asset, t_portfolio *portfolio, float quantity, float sale_price, char *sale_date)
{
	t_sale *sale;

	// Vérification des paramètres
	if (!asset || !portfolio || !sale_date || quantity <= 0 || sale_price <= 0)
	{
		printf(RED "Erreur:" RESET " Paramètres invalides pour la vente.\n");
		return;
	}

	if (quantity > asset->historique[asset->historique_count - 1].quantite)
	{
		printf(RED "Erreur:" RESET " Quantité de vente supérieure à la quantité disponible.\n");
		return;
	}

	// Redimensionnement du tableau des ventes si nécessaire
	if (asset->sale_count >= asset->max_sales)
	{
		if (!resize_sales(asset))
		{
			printf(RED "Erreur:" RESET " Impossible d'enregistrer la vente.\n");
			return;
		}
	}

	// Initialiser la vente
	sale = &asset->sales[asset->sale_count];
	sale->nom = ft_strdup(asset->nom);
	sale->date = ft_strdup(sale_date);
	sale->quantite_vendue = quantity;
	sale->prix_vente = sale_price;
	sale->profit_loss_exit = (sale_price - asset->prix_moyen) * quantity;

	if (asset->prix_moyen != 0)
	{
		sale->percent_exit = ((sale_price - asset->prix_moyen) / asset->prix_moyen) * 100;
	}
	else
	{
		sale->percent_exit = 0;
	}

	// Mettre à jour le solde du portefeuille
	portfolio->dollar_balance += quantity * sale_price;

	// Mettre à jour l'historique des quantités
	asset->historique[asset->historique_count - 1].quantite -= quantity;

	if (asset->historique[asset->historique_count - 1].quantite <= 0)
	{
		asset->is_sold_out = 1;
		asset->historique[asset->historique_count - 1].quantite = 0;
	}

	// Incrémenter le compteur de ventes
	asset->sale_count++;
}
