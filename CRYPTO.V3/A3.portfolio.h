/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A3.portfolio.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:34:39 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 03:36:14 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	A3_PORTFOLIO_H
# define A3_PORTFOLIO_H

#include "A1.master.h"

typedef struct	s_sale
{
	char	*nom;					//	Nom de l'actif vendu
	char	*date;					//	Date de la vente
	float	quantite_vendue;		//	Quantité vendue
	float	prix_vente;				//	Prix de vente par unité
	float	percent_exit;			//	Pourcentage de gain/perte
	float	profit_loss_exit;		//	Bénéfice ou perte sur la vente
}	t_sale;

typedef struct	s_history
{
	char	*date;					//	Date associée à l'entrée historique
	float	prix;					//	Prix pour cette date
	float	quantite;				//	Quantité associée à cette date
	float	diff_begin;				//	Différence avec le début
	float	percent_begin;			//	Pourcentage par rapport au début
}	t_history;

typedef struct	s_asset
{
	char	*nom;					//	Nom de l'actif
	float	prix_achat;				//	Prix d'achat initial
	float	prix_moyen;				//	Prix moyen après achats multiples
	t_history *historique;			//	Tableau dynamique des historiques
	int		historique_count;		//	Nombre d'entrées dans l'historique
	int		max_historique;			//	Capacité maximale du tableau historique
	t_sale	*sales;					//	Tableau dynamique des ventes
	int		sale_count;				//	Nombre de ventes
	int		max_sales;				//	Capacité maximale du tableau des ventes
	int		is_sold_out;			//	Indique si l'actif est entièrement vendu
}	t_asset;

typedef struct	s_transaction
{
	char	*date;					//	Date de la transaction
	float	amount;					//	Montant de la transaction (positif ou négatif)
}	t_transaction;

typedef struct	s_portfolio
{
	char			*name;				//	Nom du portefeuille
	t_asset			**assets;			//	Tableau dynamique des actifs
	int				asset_count;		//	Nombre d'actifs
	int				max_assets;			//	Capacité maximale des actifs
	float			dollar_balance;		//	Solde en dollars du portefeuille
	t_transaction	*transactions;		//	Tableau dynamique des transactions
	int				transaction_count;	//	Nombre de transactions
	int				max_transactions;	//	Capacité maximale des transactions
	char			last_updated[11];	//	Dernière mise à jour du portefeuille (YYYY-MM-DD)
	float			total_profit_loss;	//	Résumé des gains/pertes totaux
}	t_portfolio;

typedef struct	s_portfolio_manager
{
	int				portfolio_count;	//	Nombre de portefeuilles
	t_portfolio		*portfolios[MAX_PORTFOLIOS];	//	Tableau statique des portefeuilles
}	t_portfolio_manager;


t_portfolio_manager *create_portfolio_manager();
int					create_portfolio(t_portfolio_manager *manager, char *name);
t_asset 			*create_asset(t_portfolio *portfolio, char *nom, char *date, float prix_achat, float quantite);
t_portfolio 		*get_active_portfolio(t_portfolio_manager *manager);



int					 delete_portfolio(t_portfolio_manager *manager, const char *portfolio_name);
int 				delete_asset(t_portfolio *portfolio, const char *asset_name);

t_portfolio 		*find_portfolio_by_name(t_portfolio_manager *manager, const char *name);
t_asset 			*find_asset_by_name(t_portfolio *portfolio, const char *nom);
int 				is_valid_portfolio_name(const char *name);


void 				display_portfolios(t_portfolio_manager *manager);
void 				display_assets(t_portfolio *portfolio);

void				display_portfolio(t_portfolio *portfolio);
void 				display_global_summary(t_portfolio *portfolio);
void				display_transactions(t_portfolio *portfolio);
void 				display_all_sales(t_portfolio *portfolio);

void 				update_position(t_asset *asset, float nouveau_prix, float nouvelle_quantite, char *nouvelle_date);
void				 update_current_price(t_asset *asset, float update_prix, char *date_suivi);
void				 calculate_profit(t_asset *asset, int start_index, int end_index);
void				 archive_sale(t_asset *asset, t_portfolio *portfolio, float quantity, float sale_price, char *sale_date);

#endif
