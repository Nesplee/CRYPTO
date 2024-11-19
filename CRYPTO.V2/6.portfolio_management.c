/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6.portfolio_management.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 15:42:33 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 00:37:55 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

/*t_portfolio	*create_portfolio(int initial_capacity)
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
}*/

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
	portfolio->transactions = (t_transaction *)malloc(sizeof(t_transaction) * initial_capacity);
	if (!portfolio->transactions)
	{
		free(portfolio->assets);
		free(portfolio);
		return (NULL);
	}
	portfolio->asset_count = 0;
	portfolio->max_assets = initial_capacity;
	portfolio->transaction_count = 0;
	portfolio->max_transactions = initial_capacity;
	portfolio->dollar_balance = 0.0;
	return (portfolio);
}


void add_dollars(t_portfolio *portfolio, float amount, char *date)
{
	t_transaction *transaction;

	if (!portfolio || !date || amount <= 0|| !is_valid_date(date))
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

t_portfolio_manager *create_portfolio_manager(int initial_capacity)
{
	t_portfolio_manager *manager;

	manager = (t_portfolio_manager *)malloc(sizeof(t_portfolio_manager));
	if (!manager)
		return (NULL);
	manager->portfolios = (t_portfolio **)malloc(sizeof(t_portfolio *) * initial_capacity);
	if (!manager->portfolios)
	{
		free(manager);
		return (NULL);
	}
	manager->portfolio_count = 0;
	manager->max_portfolios = initial_capacity;
	return (manager);
}

int add_portfolio(t_portfolio_manager *manager, char *name)
{
	if (!manager || !name)
		return (0);

	if (manager->portfolio_count >= manager->max_portfolios)
	{
		int new_size = manager->max_portfolios * 2;
		t_portfolio **new_array = (t_portfolio **)malloc(sizeof(t_portfolio *) * new_size);

		if (!new_array)
			return (0);

		int i = 0;
		while (i < manager->portfolio_count)
		{
			new_array[i] = manager->portfolios[i];
			i++;
		}

		free(manager->portfolios);

		manager->portfolios = new_array;
		manager->max_portfolios = new_size;
	}

	manager->portfolios[manager->portfolio_count] = create_portfolio(10); // Capacité initiale pour chaque portefeuille
	if (!manager->portfolios[manager->portfolio_count])
		return (0);

	manager->portfolio_count++;
	printf(GREEN "Portefeuille '%s' créé avec succès.\n" RESET, name);
	return (1);
}



void delete_portfolio(t_portfolio_manager *manager, int index)
{
	int i;

	if (!manager || index < 0 || index >= manager->portfolio_count)
	{
		printf(RED "Erreur: Index invalide.\n" RESET);
		return ;
	}
	free_portfolio(manager->portfolios[index]); // Libère la mémoire du portefeuille
	i = index;
	while (i < manager->portfolio_count - 1)
	{
		manager->portfolios[i] = manager->portfolios[i + 1];
		i++;
	}
	manager->portfolio_count--;
	printf(GREEN "Portefeuille supprimé avec succès.\n" RESET);
}
