/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A1.master.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:37:10 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 18:09:01 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A1_MASTER_H
# define A1_MASTER_H

# ifndef INITIAL_SIZE
#  define INITIAL_SIZE 15
# endif

# ifndef MAX_PORTFOLIOS
#  define MAX_PORTFOLIOS 20
# endif

# define RESET "\033[0m"
# define GRAY "\033[90m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define LIGHT_BLUE "\033[94m"
# define BLUE "\033[34m"
#define MAGENTA "\033[1;35m"
# define BOLD "\033[1m"
# define WHITE "\033[37m"
# define UNDERLINE "\033[4m"

typedef struct s_portfolio_manager t_portfolio_manager;
typedef struct s_portfolio t_portfolio;
typedef struct s_sale t_sale;
typedef struct s_asset t_asset;
typedef struct s_transaction t_transaction;

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "A2.menu.h"
#include "A3.portfolio.h"
#include "A4.handle.h"
#include "A5.system.h"
#include "A6.utils.h"


#endif
