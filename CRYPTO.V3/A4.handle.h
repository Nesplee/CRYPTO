/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A4.handle.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 01:10:32 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 02:47:21 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A4_HANDLE_H
# define A4_HANDLE_H

#include "A1.master.h"

/*	============================== ET2 HANDLE PORTFOLIO ==============================*/

void 	handle_create_portfolio(t_portfolio_manager *manager);
void 	handle_delete_portfolio(t_portfolio_manager *manager);

/*	============================== ET2 HANDLE ASSETS ==============================*/

void 	handle_delete_asset(t_portfolio *portfolio);

/*	============================== ET3 HANDLE PORTFOLIO ==============================*/

void 	handle_add_dollars(t_portfolio *portfolio, float amount, char *date);
void 	handle_withdraw_dollars(t_portfolio *portfolio, float amount, char *date);
void	display_portfolio_short_summary(t_portfolio *portfolio);
void	display_portfolio_long_summary(t_portfolio *portfolio);
void	display_all_portfolios_short(t_portfolio_manager *manager);
void	display_all_portfolios_long(t_portfolio_manager *manager);
/*	============================== ET3 HANDLE ASSETS ==============================*/

void 	handle_display_last_purchases(t_portfolio_manager *manager);
void 	handle_display_last_sales(t_portfolio_manager *manager);
void 	handle_display_asset_transactions(t_portfolio_manager *manager);
int 	handle_add_asset(t_portfolio *portfolio);
void 	handle_update_asset(t_portfolio *portfolio);
void 	handle_sell_asset(t_portfolio *portfolio);
void 	handle_calculate_profit(t_portfolio *portfolio);
void 	handle_simulate_asset_sale(t_portfolio *portfolio);


#endif
