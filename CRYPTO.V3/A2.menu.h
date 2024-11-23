/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A2.menu.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:41:56 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 01:54:20 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A2_MENU_H
# define A2_MENU_H

#include "A1.master.h"

/*	============================== MENU PRINCIPAL ==============================*/

void 	display_welcome_screen(void);
void 	display_header(const char *title);
void	main_menu(t_portfolio_manager *manager);

/*	============================== MENUS PORTEFEUILLE ==============================*/

void	portfolio_menu(t_portfolio_manager *manager);
void	portfolio_vision_menu(t_portfolio_manager *manager);
void	portfolio_action_menu(t_portfolio_manager *manager);

/*	============================== MENUS DES ACTIFS =============================*/

void 	asset_menu(t_portfolio_manager *manager);
void	asset_vision_menu(t_portfolio_manager *manager);
void 	asset_action_menu(t_portfolio_manager *manager);

#endif
