/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:22:53 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 15:34:22 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void	handle_menu(t_portfolio *portfolio)
{
	char	choix[3];
	int		exit_program = 0;

	display_header();
	while (!exit_program)
	{
		display_menu();
		if(!fgets(choix, sizeof(choix), stdin))
			continue;
		if (choix[0] == '1')
			handle_add_asset(portfolio);
		else if (choix[0] == '2')
			handle_update_asset(portfolio);
		else if (choix[0] == '3')
			handle_sell_asset(portfolio);
		else if (choix[0] == '4')
			display_portfolio_and_summary(portfolio);
		else if (choix[0] == '5')
			handle_calculate_profit(portfolio);
		else if (choix[0] == '6')
			handle_display_graph(portfolio);
		else if (choix[0] == '7')
			handle_export_asset(portfolio);
		else if (choix[0] == '8')
			handle_delete_asset(portfolio);
		else if (choix[0] == '9')
		{
			free_portfolio(portfolio);
			portfolio = load_portfolio("portfolio.txt");
		}
		else if (choix[0] == '0')
		{
			save_portfolio(portfolio, "portfolio.txt");
			exit_program = 1;
		}
		else
			printf(RED "Choix invalide. Veuillew réessayer.\n" RESET);
	}
}

void	handle_add_asset(t_portfolio *portfolio)
{
	char	nom[15];
	char	date[12];
	float	prix;
	float	quantite;
	float	total_cost;
	char	confirmation[3];

	printf(GRAY "Nom de l'actif : " RESET);
	scanf("%49s", nom);
	while (!is_snake_case(nom))
	{
		printf(RED "Erreur:" RESET "Le nom doit être en snake_case (lettres, chiffres, undercore/ 15 carac. max). Réessayez : ");
		scanf("%49s", nom);
	}
	printf(GRAY "Date d'achat (YYYY-MM-DD) : " RESET);
	scanf("%11s", date);
	while (!is_valid_date(date))
	{
		printf(RED "Erreur:" RESET "le format de la date est invalide. Réessayez.\n");
		printf(GRAY "Date d'achat (YYYY-MM-DD) : " RESET);
		scanf("%11s", date);
	}
	printf(GRAY "Prix d'achat : " RESET);
	scanf("%f", &prix);
	printf(GRAY "Quantité achetée : " RESET);
	scanf("%f", &quantite);
	total_cost = prix * quantite;
	printf(YELLOW "Voulez-vous utiliser des dollars locaux pour cet achat? (O/N) : " RESET);
	scanf("%2s", confirmation);
	if (confirmation[0] == 'O' || confirmation[0] == 'o')
	{
		if (portfolio->dollar_balance >= total_cost)
		{
			portfolio->dollar_balance = portfolio->dollar_balance - total_cost;
			printf(GREEN "Achat déduit du solde. Nouveau solde: %.2f $\n" RESET, portfolio->dollar_balance);
		}
		else
		{
			printf(RED "Erreur:" RESET "Solde insuffisant pour cet achat. (%.2f $ nécessaires, %.2f $ disponibles)\n", total_cost, portfolio->dollar_balance);
			return ;
		}
	}
	if (!create_asset(portfolio, nom, date, prix, quantite))
		printf(RED "Erreur:" RESET "Impossibler d'ajouter l'actif.\n");
	else
		printf(GREEN "Actif ajouté avec succès!\n" RESET);
}

void	handle_update_asset(t_portfolio *portfolio)
{
	char	nom[15];
	char	date[12];
	char	choix[3];
	float	nouveau_prix;
	float	nouvelle_quantite;
	int		i = 0;
	int		updated = 0;

	printf("Nom de l'actif à mettre à jour : ");
	scanf("%49s", nom);
	while (i < portfolio->asset_count)
	{
		if (ft_strlen(portfolio->assets[i]->nom) > 0 && ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			printf("Souhaitez-vous réaliser un achat? (O/N) : ");
			scanf("%2s", choix);
			if (choix[0] == 'O' || choix[0] == 'o')
			{
				printf("Nouveau prix : ");
				scanf("%f", &nouveau_prix);
				printf("Nouvelle quantité :");
				scanf("%f", &nouvelle_quantite);
				printf("Nouvelle date : ");
				scanf("%11s", date);
				while (!is_valid_date(date))
				{
					printf(RED "Erreur:" RESET " Format de date invalide. Réssayez (YYYY-MM-DD) :");
					scanf("%11s", date);
				}
				update_position(portfolio->assets[i], nouveau_prix, nouvelle_quantite, date);
			}
			else
			{
				printf("Nouveau prix : ");
				scanf("%f", &nouveau_prix);
				printf("Date de modification (YYYY-MM-DD) : ");
				scanf("%11s", date);
				while (!is_valid_date(date))
				{
					printf(RED "Erreur:" RESET " Format de date invalide. Réssayez (YYYY-MM-DD) :");
					scanf("%11s", date);
				}
				update_current_price(portfolio->assets[i], nouveau_prix, date);
			}
			updated = 1;
			printf(GREEN "Modification de l'actif %s réussie.\n" RESET, nom);
		}
		i++;
	}
	if (!updated)
		printf(RED "Erreur:" RESET " Actif non trouvé.\n");
}

void	handle_sell_asset(t_portfolio *portfolio)
{
	char	nom[15];
	char	date[12];
	float	quantite;
	float	prix;
	int		i = 0;
	int		sold = 0;

	printf("Nom de l'actif à vendre :");
	scanf("%49s", nom);
	while (i < portfolio->asset_count)
	{
		if (ft_strlen(portfolio->assets[i]->nom) > 0 && ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			printf("Quantité à vendre :");
			scanf("%f", &quantite);
			printf("Prix de vente : ");
			scanf("%f", &prix);
			printf("Date de vente : ");
			scanf("%11s", date);
			archive_sale(portfolio->assets[i], portfolio, quantite, prix, date);
			sold = 1;
		}
		i++;
	}
	if (!sold)
		printf(RED "Erreur :" RESET "Actif non trouvé.\n");
}

void	handle_export_asset(t_portfolio *portfolio)
{
	char	nom[15];
	char	filename[20];
	int		i = 0;
	int		found = 0;

	printf("Nom de l'actif à exporter : ");
	scanf("%49s", nom);
	while (i < portfolio->asset_count)
	{
		if (ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			found = 1;
			printf("Nom du fichier CSV (ex: actif.csv / 20 carac. max) :");
			scanf("%49s", filename);
			export_asset_to_csv(portfolio->assets[i], filename);
		}
		i++;
	}
	if (!found)
		printf("Erreur: Actif non trouvé.\n");
}

void handle_display_graph(t_portfolio *portfolio)
{
	int i, j;
	int bar_length;

	if (portfolio == NULL || portfolio->asset_count == 0) {
		printf(RED "Erreur : Aucun actif dans le portefeuille.\n" RESET);
		return;
	}

	printf(BOLD "Graphique des bénéfices/déficits :\n" RESET);
	print_line('=', 50);

	for (i = 0; i < portfolio->asset_count; i++) {
		t_point *asset = portfolio->assets[i];
		float diff = asset->historique_prix[asset->date_count - 1] - asset->prix_moyen;
		bar_length = (int)(diff / 10); // Ajustez le facteur selon vos données
		printf(GRAY "%-15s :" RESET, asset->nom);
		if (diff > 0) {
			for (j = 0; j < bar_length; j++)
				printf(GREEN "|" RESET);
			printf(GREEN " +%.2f\n" RESET, diff);
		} else {
			for (j = 0; j < -bar_length; j++)
				printf(RED "|" RESET);
			printf(RED " %.2f\n" RESET, diff);
		}
	}
}

void	handle_calculate_profit(t_portfolio *portfolio)
{
	char	nom[15];
	int		start_index;
	int		end_index;
	int		i = 0;
	int		found = 0;

	if (portfolio == NULL || portfolio->asset_count == 0)
	{
		printf("Erreur: Aucun actif dans le portefeuille.\n");
		return ;
	}
	printf("Nom de l'actif pour lequel calculer le profit ou le déficit: ");
	scanf("%49s", nom);
	while (i < portfolio->asset_count)
	{
		if (ft_strlen(portfolio->assets[i]->nom) > 0 && ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			printf("Index de début (0 à %d) : ", portfolio->assets[i]->date_count - 1);
			scanf("%d", &start_index);
			printf("Index de fin (0 à %d) :", portfolio->assets[i]->date_count - 1);
			scanf("%d", &end_index);
			calculate_profit(portfolio->assets[i], start_index, end_index);
			found = 1;
		}
		i++;
	}
	if (!found)
		printf("Erreur: Actif non trouvé.\n");
}

void handle_delete_asset(t_portfolio *portfolio)
{
	char choix[3];
	char nom[15];
	int i = 0;
	int found = 0;

	if (portfolio == NULL || portfolio->asset_count == 0)
	{
		printf(RED "Erreur : Aucun actif à supprimer.\n" RESET);
		return;
	}

	printf("Souhaitez-vous supprimer tous les actifs vendus ? (O/N) : ");
	scanf("%2s", choix);

	if (choix[0] == 'O' || choix[0] == 'o')
	{
		free_sold_assets(portfolio);
		printf(GREEN "Tous les actifs entièrement vendus ont été supprimés.\n" RESET);
		return;
	}

	printf("Nom de l'actif à supprimer : ");
	scanf("%49s", nom);

	while (i < portfolio->asset_count)
	{
		if (ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			found = 1;
			free_asset(portfolio->assets[i]);
			portfolio->assets[i] = NULL;

			// Réorganisation du tableau
			while (i < portfolio->asset_count - 1)
			{
				portfolio->assets[i] = portfolio->assets[i + 1];
				i++;
			}

			portfolio->assets[portfolio->asset_count - 1] = NULL;
			portfolio->asset_count--;

			printf(GREEN "Actif '%s' supprimé avec succès.\n" RESET, nom);
			return;
		}
		i++;
	}

	if (!found)
		printf(RED "Erreur : Actif '%s' introuvable.\n" RESET, nom);
}

