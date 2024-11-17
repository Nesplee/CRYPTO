/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_prog.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:48:19 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 16:37:15 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_PROG_H
# define MY_PROG_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

//		DEFINITIONS DES COULEURS UTILISEES

# define RESET "\033[0m"
# define GRAY "\033[90m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define CYAN "\033[36m"
# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"

//		STRUCTURE POUR VENTE

typedef struct	s_sale
{
	char	*nom;
	char	*date;
	float	quantite_vendue;
	float	prix_vente;
	float	percent_exit;
	float	profit_loss_exit;
}	t_sale;

//		STRUCTURE POUR ACTIF

typedef struct	s_point
{
	char	*nom;
	char	**dates;
	float	prix_achat;
	float	prix_moyen;
	float	*historique_prix;
	float	*historique_quantite;
	float	*historique_diff_begin;
	float	*historique_percent_begin;
	int		date_count;
	int		max_dates;
	t_sale	*sales;
	int		sale_count;
	int		max_sales;
	int		is_sold_out;
}	t_point;

//		STRUCTURE POUR TRANSACTIONS

typedef struct	s_transaction
{
	char	*date;
	float	amount;
}	t_transaction;

//		STRUCTURE POUR PORTEFEUILLE

typedef struct	s_portfolio
{
	t_point			**assets;
	int				asset_count;
	int				max_assets;
	float			dollar_balance;
	t_transaction	*transactions;
	int				transaction_count;
	int				max_transactions;
}	t_portfolio;

//		FONCTIONS UTILS

void		ft_putchar(char c);
int			ft_strlen(char *str);
void		ft_putnbr(int n);
void		print_line(char c, int len);
int			is_snake_case(const char *str);
int 		is_valid_date(const char *date);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strdup(char *str);

//		FONCTIONS FREE & RESIZE

void		free_asset(t_point *asset);
void 		free_sold_assets(t_portfolio *portfolio);
void		free_portfolio(t_portfolio *portfolio);
void 		free_transactions(t_portfolio *portfolio);
int			resize_arrays(t_point *asset);
int			resize_portfolio(t_portfolio *portfolio);
int 		resize_sales(t_point *asset);
int 		resize_transactions(t_portfolio *portfolio);

//		FONCTIONS DE DISPLAY

void 		display_header(void);
void		display_menu(void);
void		display_sales_history(t_point *asset);
void 		display_portfolio(t_portfolio *portfolio);
void 		display_global_summary(t_portfolio *portfolio);
void 		display_portfolio_and_summary(t_portfolio *portfolio);
void 		display_transactions(t_portfolio *portfolio);

//		FONCTIONS HANDLE MENU

void		handle_menu(t_portfolio *portfolio);
void		handle_add_asset(t_portfolio *portfolio);
void		handle_update_asset(t_portfolio *portfolio);
void		handle_sell_asset(t_portfolio *portfolio);
void		handle_export_asset(t_portfolio *portfolio);
void 		handle_display_graph(t_portfolio *portfolio);
void		handle_calculate_profit(t_portfolio *portfolio);
void 		handle_delete_asset(t_portfolio *portfolio);

//		FONCTIONS DE MANAGEMENT D'ACTIFS

t_portfolio	*create_portfolio(int initial_capacity);
t_point		*create_asset(t_portfolio *portfolio, char *nom, char *date, float prix_achat, float quantite);
void		update_position(t_point *actif, float nouveau_prix, float nouvelle_quantite, char *nouvelle_date);
void		update_current_price(t_point *actif, float update_prix, char *date_suivi);
void		calculate_profit(t_point *asset, int start_index, int end_index);
void		archive_sale(t_point *asset, t_portfolio *portfolio, float quantity, float sale_price, char *sale_date);

//		FONCTIONS DE MANAGEMENT DE PORTEFEUILLES
void add_dollars(t_portfolio *portfolio, float amount, char *date);
void withdraw_dollars(t_portfolio *portfolio, float amount, char *date);

//		FONCTIONS DE SAUVEGARDE, CHARGEMENT, EXPORTATION

void 		export_asset_to_csv(t_point *asset, const char *filename);
void		save_portfolio(t_portfolio *portfolio, const char *filename);
t_portfolio	*load_portfolio(const char *filename);

#endif
