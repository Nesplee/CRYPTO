/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A5.system.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:38:39 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/25 01:16:42 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A5_SYSTEM_H
# define A5_SYSTEM_H

#include "A1.master.h"

void 				system_menu(t_portfolio_manager *manager);
void 				undo_last_action(t_portfolio_manager *manager);
int					load_portfolio(const char *filename, t_portfolio *portfolio);
int 				save_portfolio(const t_portfolio *portfolio, const char *filename);


int 				save_assets_history(FILE *file, const t_portfolio *portfolio);
int 				save_sales_history(FILE *file, const t_portfolio *portfolio);
int 				save_transactions_history(FILE *file, const t_portfolio *portfolio);
int 				save_content(FILE *file, const t_portfolio *portfolio);

int					 load_assets_history(FILE *file, t_portfolio *portfolio);
int 				load_sales_history(FILE *file, t_asset *asset);
int 				load_transactions_history(FILE *file, t_portfolio *portfolio);
int 				load_content(FILE *file, t_portfolio *portfolio);

#endif
