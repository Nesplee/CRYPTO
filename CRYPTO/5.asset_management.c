/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5.asset_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:14:49 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 16:51:02 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

/*
		FONCTIONS D'ACHAT/UPDATES D'ASSETS
			- CONFIGURER LES ACHATS AVEC $ OU APPORT DE CASH.
*/

t_point	*create_asset(t_portfolio *portfolio, char *nom, char *date, float prix_achat, float quantite)
{
	t_point	*new_actif;

	if (!portfolio || !nom || !date)
	{
		printf(RED "Erreur:" RESET "Paramètres invalides.\n");
		return (NULL);
	}
	if (prix_achat < 0)
	{
		printf(RED "Erreur:" RESET "Le prix d'achat doit être supérieur à 0.\n");
		return (NULL);
	}
	if (quantite <= 0)
	{
		printf(RED "Erreur:" RESET "La quantité doit être supérieure à 0.\n");
		return (NULL);
	}
	if (portfolio->asset_count >= portfolio->max_assets)
	{
		if (!resize_portfolio(portfolio))
		{
			printf(RED "Erreur:" RESET "Impossible d'ajouter un nouvel actif.\n");
			return NULL;
		}
	}
	new_actif = (t_point *)malloc(sizeof(t_point));
	if (new_actif == NULL)
		return (NULL);
	new_actif->nom = ft_strdup(nom);
	if (new_actif->nom == NULL)
	{
		free(new_actif);
		return (NULL);
	}
	new_actif->prix_achat = prix_achat;
	new_actif->prix_moyen = prix_achat;
	new_actif->date_count = 0;
	new_actif->sale_count = 0;
	new_actif->max_dates = 20;
	new_actif->max_sales = 20;
	new_actif->dates = (char **)malloc(sizeof(char *) * new_actif->max_dates);
	new_actif->historique_quantite = (float *)malloc(sizeof(float) * new_actif->max_dates);
	new_actif->historique_prix = (float *)malloc(sizeof(float) * new_actif->max_dates);
	new_actif->historique_diff_begin = (float *)malloc(sizeof(float) * new_actif->max_dates);
	new_actif->historique_percent_begin = (float *)malloc(sizeof(float) * new_actif->max_dates);
	new_actif->sales = (t_sale *)malloc(sizeof(t_sale) * new_actif->max_sales);
	if (!new_actif->dates || !new_actif->historique_prix || !new_actif->historique_quantite ||
		!new_actif->historique_diff_begin || !new_actif->historique_percent_begin || !new_actif->sales)
	{
		free_asset(new_actif);
		return NULL;
	}
	new_actif->dates[0] = ft_strdup(date);
	if (!new_actif->dates[0])
	{
		free_asset(new_actif);
		return NULL;
	}
	new_actif->historique_prix[0] = prix_achat;
	new_actif->historique_quantite[0] = quantite;
	new_actif->historique_diff_begin[0] = 0;
	new_actif->historique_percent_begin[0] = 0;
	new_actif->date_count = 1;
	portfolio->assets[portfolio->asset_count++] = new_actif;
	return (new_actif);
}

void	update_position(t_point *actif, float nouveau_prix, float nouvelle_quantite, char *nouvelle_date)
{
	if (!actif || !nouvelle_date)
	{
		printf(RED "Erreur:" RESET "Actif ou date invalide.\n");
		return ;
	}
	if (nouvelle_quantite < 0)
	{
		printf(RED "Erreur:" RESET "La nouvelle quantité doit être supérieure à 0.\n");
		return ;
	}
	if (actif->date_count >= actif->max_dates)
	{
		if (!resize_arrays(actif))
		{
			printf(RED "Erreur:" RESET "Impossible de mettre à jour l'actif.\n");
			return ;
		}
	}

	float	cout_total_ancien;
	float	cout_total_nouveau;
	float	quantite_totale;

	cout_total_ancien = actif->prix_moyen * actif->historique_quantite[actif->date_count - 1];
	cout_total_nouveau = nouveau_prix * nouvelle_quantite;
	quantite_totale = actif->historique_quantite[actif->date_count - 1] + nouvelle_quantite;
	actif->prix_moyen = (cout_total_ancien + cout_total_nouveau) / quantite_totale;
	actif->dates[actif->date_count] = ft_strdup(nouvelle_date);
	if (actif->dates[actif->date_count] == NULL)
	{
		printf(RED "Erreur" RESET "d'allocation pour la nouvelle date.\n");
		return ;
	}
	actif->historique_prix[actif->date_count] = nouveau_prix;
	actif->historique_quantite[actif->date_count] = quantite_totale;
	actif->historique_diff_begin[actif->date_count] = (nouveau_prix - actif->prix_achat) * quantite_totale;
	actif->historique_percent_begin[actif->date_count] = (actif->historique_diff_begin[actif->date_count] / (actif->prix_achat * quantite_totale)) * 100;
	actif->date_count++;
}

void	update_current_price(t_point *actif, float update_prix, char *date_suivi)
{
	float	dernier_prix;
	float	diff_since_last;
	float	percent_since_last;
	int		i;
	if (actif == NULL)
		return ;
	if (actif->date_count >= actif->max_dates)
	{
		if (!resize_arrays(actif))
		{
			printf(RED "Erreur:" RESET "Impossible de redimmensionner l'actif.\n");
			return ;
		}
	}
	actif->historique_prix[actif->date_count] = update_prix;
	actif->historique_diff_begin[actif->date_count] = (update_prix - actif->prix_achat);
	if (actif->prix_achat != 0)
		actif->historique_percent_begin[actif->date_count] = (actif->historique_diff_begin[actif->date_count] / actif->prix_achat) * 100;
	else
		actif->historique_percent_begin[actif->date_count] = 0;
	if (actif->date_count > 0)
	{
		dernier_prix = actif->historique_prix[actif->date_count - 1];
		diff_since_last = update_prix - dernier_prix;
		percent_since_last = 0;
		if (dernier_prix != 0)
			percent_since_last = (diff_since_last / dernier_prix) * 100;
		printf("Différence de valeur depuis la dernière MAJ: %.5f\n", diff_since_last);
		printf("Pourcentage de progression depuis la dernière MAJ: %.5f%%\n", percent_since_last);
	}
	if (actif->dates[actif->date_count] != NULL)
	{
		free(actif->dates[actif->date_count]);
		actif->dates[actif->date_count] = NULL;
	}
	actif->dates[actif->date_count] = malloc(ft_strlen(date_suivi) + 1);
	if (actif->dates[actif->date_count] == NULL)
		return ;
	i = 0;
	while (date_suivi[i])
	{
		actif->dates[actif->date_count][i] = date_suivi[i];
		i++;
	}
	actif->dates[actif->date_count][i] = '\0';
	actif->date_count++;
}

/*
			FONCTIONS DE CALCULS - A COMPLETER -
*/

void	calculate_profit(t_point *asset, int start_index, int end_index)
{
	float	start_price;
	float	end_price;
	float	quantity;
	float	diff;
	float	percent;

	if (asset == NULL)
	{
		printf(RED "Erreur:" RESET "Actif non défini.\n");
		return ;
	}
	if (start_index < 0 || start_index >= asset->date_count)
	{
		printf(RED "Erreur:" RESET "start_index doit être >= 0.\n");
		return ;
	}
	if (end_index >= asset->date_count)
	{
		printf(RED "Erreur :" RESET "end_index dépasse le nombre total de dates (%d).\n", asset->date_count);
		return ;
	}
	if (start_index >= end_index)
	{
		printf(RED "Erreur:" RESET "start_index (%d) doit être strictement inférieur à end_index (%d).\n", start_index, end_index);
		return ;
	}

	start_price = asset->historique_prix[start_index];
	end_price = asset->historique_prix[end_index];
	quantity = asset->historique_quantite[end_index];
	diff = (end_price -start_price) * quantity;
	percent = 0;
	if (start_price * quantity != 0)
		percent = (diff/ (start_price * quantity)) * 100;
	printf("Profit ou Deficit entre les dates %d et %d :\n", start_index, end_index);
	printf("Entre" YELLOW "[%s]" RESET "et" YELLOW "[%s]" RESET ":\n", asset->dates[start_index], asset->dates[end_index]);
	if (diff >= 0)
		printf(GREEN "Difference de valeur: +%.5f $\nPourcentage de progression: +%.2f%%\n" RESET, diff, percent);
	else
		printf(RED "Différence de valeur: %.5f $\nPourcentage de progression: %.2f%%\n" RESET, diff, percent);
}

void	archive_sale(t_point *asset, t_portfolio *portfolio, float quantity, float sale_price, char *sale_date)
{
	t_sale	*sale;
	float	sale_value;

	if (asset == NULL || portfolio == NULL || sale_date == NULL)
	{
		printf(RED "Erreur:" RESET "Données invalides\n");
		return ;
	}
	if (quantity < 0)
	{
		printf(RED "Erreur:" RESET "La quantité vendue doit être supérieure à 0.\n");
		return ;
	}
	if (sale_price < 0)
	{
		printf(RED "Erreur:" RESET "Le prix de vente doit être supérieur à 0.\n");
		return ;
	}
	if (quantity > asset->historique_quantite[asset->date_count - 1])
	{
		printf(RED "Erreur:" RESET "Quantité de vente supérieure à la quantité disponible.\n");
		return ;
	}
	if (asset->sale_count >= asset->max_sales)
	{
		if (!resize_sales(asset))
		{
			printf(RED "Erreur:" RESET "Impossible d'ajouter une nouvelle vente.\n");
			return ;
		}
	}
	if (quantity > asset->historique_quantite[asset->date_count - 1])
	{
		printf("Quantité de vente supérieure à la quantité détenue.\n");
		return ;
	}
	sale = &asset->sales[asset->sale_count];
	sale->nom = ft_strdup(asset->nom);
	if (sale->nom == NULL)
	{
		printf(RED "Erreur d'allocation mémoire lors de l'enregistrement de la vente.\n" RESET);
		return ;
	}
	sale->date = ft_strdup(sale_date);
	if (sale->date == NULL)
	{
		printf(RED "Erreur d'alloction mémoire pour la date de vente.\n" RESET);
		free(sale->nom);
		return ;
	}
	sale->quantite_vendue = quantity;
	sale->prix_vente = sale_price;
	sale->profit_loss_exit = (sale_price - asset->prix_moyen) * quantity;
	if(asset->prix_moyen != 0)
		sale->percent_exit = ((sale_price - asset->prix_moyen) / asset->prix_moyen) * 100;
	else
		sale->percent_exit = 0;
	sale_value = quantity * sale_price;
	portfolio->dollar_balance = portfolio->dollar_balance + sale_value;
	asset->historique_quantite[asset->date_count - 1] = asset->historique_quantite[asset->date_count - 1] - quantity;
	if (asset->historique_quantite[asset->date_count - 1] <= 0)
	{
		asset->is_sold_out = 1;
		printf("Actif %s vendu intégralement, mais conservé dans le portefeuille.\n", asset->nom);
	}
	asset->sale_count++;
	printf("Vente archivée pour %s :\n", asset->nom);
	printf("Quantité vendue : %.5f\nPrix de vente : %.5f\n", quantity, sale_price);
	printf("Bénéfice/Déficit : %.5f\nPourcentage par rapport au prix d'achat moyen : %.5f%%\n", sale->profit_loss_exit, sale->percent_exit);
}
