/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A6.utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:33:21 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 03:19:02 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A6_UTILS_H
# define A6_UTILS_H

#include "A1.master.h"

void				ft_putchar(char c);
int					ft_strlen(const char *str);
void				ft_putnbr(int n);
void				print_line(char c, int len);
int					is_snake_case(const char *str);
int 				is_only_spaces(const char *str);
int 				is_valid_date(const char *date);
int					ft_strcmp(char *s1, char *s2);
char				*ft_strdup(const char *str);
void 				clear_screen(void);
int 				get_terminal_width(void);
size_t 				stripped_strlen(const char *str);
void 				print_centered(const char *text, const char *color);
void 				clear_stdin();
void 				pause_if_needed(int pause_needed);
void 				display_asset_manager_header();
int 				ask_confirmation(const char *message);


void 				safe_free(void **ptr);
void 				safe_free_array(void ***array, int count);
void				free_asset(t_asset *asset);
void 				free_sold_assets(t_portfolio *portfolio);
void				free_portfolio(t_portfolio *portfolio);
void 				free_transactions(t_portfolio *portfolio);
void 				free_portfolio_manager(t_portfolio_manager *manager);
int					resize_arrays(t_asset *asset);
int					resize_portfolio(t_portfolio *portfolio);
int 				resize_sales(t_asset *asset);
int 				resize_transactions(t_portfolio *portfolio);
int 				resize_history(t_asset *asset);

int 				input_name(char *name, size_t size);
float 				input_amount();
int 				input_date(char *date, size_t size);
int 				input_choice(int min, int max);


int		 			tput_cols();
void				trim_whitespace(char *str);
void 				log_message(const char *message);

#endif
