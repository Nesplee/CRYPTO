/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6.portfolio_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:42:33 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 16:50:37 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

t_portfolio	*create_portfolio(int initial_capacity)
{
	t_portfolio	*portfolio;

	portfolio = (t_portfolio *)malloc(sizeof(t_portfolio));
	if (!portfolio)
		return (NULL);
	portfolio->assets = (t_point **)malloc(sizeof(t_point *) * initial_capacity);
	if (!portfolio->assets)
	{
		free(portfolio);
		return (NULL);
	}
	portfolio->asset_count = 0;
	portfolio->max_assets = initial_capacity;
	portfolio->dollar_balance = 0.0;
	return (portfolio);
}

void add_dollars(t_portfolio *portfolio, float amount, char *date)
{
	t_transaction *transaction;

	if (!portfolio || !date || amount <= 0)
	{
		printf(RED "Erreur: Paramètres invalides.\n" RESET);
		return ;
	}
	if (portfolio->transaction_count >= portfolio->max_transactions)
	{
		if (!resize_transactions(portfolio))
		{
			printf(RED "Erreur: Impossible de redimensionner les transactions.\n" RESET);
			return ;
		}
	}
	transaction = &portfolio->transactions[portfolio->transaction_count];
	transaction->date = ft_strdup(date);
	if (!transaction->date)
	{
		printf(RED "Erreur: Allocation mémoire échouée pour la date.\n" RESET);
		return ;
	}
	transaction->amount = amount;
	portfolio->dollar_balance += amount;
	portfolio->transaction_count++;
	printf(GREEN "Dollars ajoutés: %.2f. Nouveau solde: %.2f.\n" RESET, amount, portfolio->dollar_balance);
}

void withdraw_dollars(t_portfolio *portfolio, float amount, char *date)
{
	t_transaction *transaction;

	if (!portfolio || !date || amount <= 0)
	{
		printf(RED "Erreur: Paramètres invalides.\n" RESET);
		return ;
	}
	if (portfolio->dollar_balance < amount)
	{
		printf(RED "Erreur: Solde insuffisant. Disponible: %.2f.\n" RESET, portfolio->dollar_balance);
		return ;
	}
	if (portfolio->transaction_count >= portfolio->max_transactions)
	{
		if (!resize_transactions(portfolio))
		{
			printf(RED "Erreur: Impossible de redimensionner les transactions.\n" RESET);
			return ;
		}
	}
	transaction = &portfolio->transactions[portfolio->transaction_count];
	transaction->date = ft_strdup(date);
	if (!transaction->date)
	{
		printf(RED "Erreur: Allocation mémoire échouée pour la date.\n" RESET);
		return ;
	}
	transaction->amount = -amount;
	portfolio->dollar_balance -= amount;
	portfolio->transaction_count++;
	printf(GREEN "Dollars retirés: %.2f. Nouveau solde: %.2f.\n" RESET, amount, portfolio->dollar_balance);
}
