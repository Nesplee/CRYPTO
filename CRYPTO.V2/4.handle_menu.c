/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4.handle_menu.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:22:53 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 01:43:47 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

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
	clear_stdin();
	while (!is_snake_case(nom))
	{
		printf(RED "Erreur:" RESET "Le nom doit être en snake_case (lettres, chiffres, undercore/ 15 carac. max). Réessayez : ");
		scanf("%49s", nom);
		clear_stdin();
	}
	printf(GRAY "Date d'achat (YYYY-MM-DD) : " RESET);
	scanf("%11s", date);
	clear_stdin();
	while (!is_valid_date(date))
	{
		printf(RED "Erreur:" RESET "le format de la date est invalide. Réessayez.\n");
		printf(GRAY "Date d'achat (YYYY-MM-DD) : " RESET);
		scanf("%11s", date);
		clear_stdin();
	}
	printf(GRAY "Prix d'achat : " RESET);
	scanf("%f", &prix);
	clear_stdin();
	printf(GRAY "Quantité achetée : " RESET);
	scanf("%f", &quantite);
	clear_stdin();
	total_cost = prix * quantite;
	printf(YELLOW "Voulez-vous utiliser des dollars locaux pour cet achat? (O/N) : " RESET);
	scanf("%2s", confirmation);
	clear_stdin();
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
	int found = 0;

	if (!portfolio || !portfolio->assets || portfolio->asset_count == 0)
	{
		printf(RED "Erreur : Aucun actif disponible pour la mise à jour.\n" RESET);
		return;
	}

	printf("Nom de l'actif à mettre à jour : ");
	if (!fgets(nom, sizeof(nom), stdin) || nom[0] == '\n')
	{
		printf(RED "Erreur: Lecture du nom échouée.\n" RESET);
		return;
	}
	nom[strcspn(nom, "\n")] = '\0';

	while (i < portfolio->asset_count)
	{
		if (!portfolio->assets[i] || !portfolio->assets[i]->nom)
		{
			i++;
			continue;
		}

		if (ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			found = 1;
			printf("Souhaitez-vous réaliser un achat? (O/N) : ");
			if (!fgets(choix, sizeof(choix), stdin) || choix[0] == '\n')
			{
				printf(RED "Erreur: Lecture échouée.\n" RESET);
				clear_stdin();
				i++;
				continue;
			}
			choix[strcspn(choix, "\n")] = '\0'; // Supprime le '\n'

			if (choix[0] == 'O' || choix[0] == 'o')
			{
				printf("Nouveau prix : ");
				if (scanf("%f", &nouveau_prix) != 1)
				{
					printf(RED "Erreur: Lecture du prix échouée.\n" RESET);
					clear_stdin();
					i++;
					continue;
				}
				clear_stdin();

				printf("Nouvelle quantité : ");
				if (scanf("%f", &nouvelle_quantite) != 1)
				{
					printf(RED "Erreur: Lecture de la quantité échouée.\n" RESET);
					clear_stdin();
					i++;
					continue;
				}
				clear_stdin();

				printf("Nouvelle date : ");
				if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
				{
					printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
					clear_stdin();
					i++;
					continue;
				}
				date[strcspn(date, "\n")] = '\0'; // Supprime le '\n'

				while (!is_valid_date(date))
				{
					printf(RED "Erreur: Format de date invalide. Réessayez (YYYY-MM-DD) : ");
					if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
					{
						printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
						clear_stdin();
						i++;
						continue;
					}
					date[strcspn(date, "\n")] = '\0'; // Supprime le '\n'
				}
				update_position(portfolio->assets[i], nouveau_prix, nouvelle_quantite, date);

				printf(GREEN "Actif '%s' mis à jour avec un nouvel achat.\n" RESET, nom);
			}
			else
			{
				printf("Nouveau prix : ");
				if (scanf("%f", &nouveau_prix) != 1)
				{
					printf(RED "Erreur: Lecture du prix échouée.\n" RESET);
					clear_stdin();
					i++;
					continue;
				}
				clear_stdin();

				printf("Date de modification (YYYY-MM-DD) : ");
				if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
				{
					printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
					clear_stdin();
					i++;
					continue;
				}
				date[strcspn(date, "\n")] = '\0'; // Supprime le '\n'

				while (!is_valid_date(date))
				{
					printf(RED "Erreur: Format de date invalide. Réessayez (YYYY-MM-DD) : ");
					if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
					{
						printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
						clear_stdin();
						i++;
						continue;
					}
					date[strcspn(date, "\n")] = '\0'; // Supprime le '\n'
				}

				update_current_price(portfolio->assets[i], nouveau_prix, date);
				printf(GREEN "Actif '%s' mis à jour avec un nouveau prix.\n" RESET, nom);
			}
		}
		i++;
	}

	if (!found)
		printf(RED "Erreur: Actif non trouvé.\n" RESET);
	else
		printf(GREEN "Modification de l'actif %s réussie.\n" RESET, nom);
	return ;
}

/*void	handle_sell_asset(t_portfolio *portfolio)
{
	char	nom[15], date[12];
	float	quantite, prix;
	int		i = 0, sold = 0;

	printf("Nom de l'actif à vendre : ");
	scanf("%14s", nom);
	clear_stdin();

	while (i < portfolio->asset_count && !sold)
	{
		if (ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			printf("Quantité à vendre : ");
			scanf("%f", &quantite);
			clear_stdin();
			printf("Prix de vente : ");
			scanf("%f", &prix);
			clear_stdin();
			printf("Date de vente : ");
			scanf("%11s", date);
			clear_stdin();

			archive_sale(portfolio->assets[i], portfolio, quantite, prix, date);
			free_sold_assets(portfolio);
			sold = 1;
		}
		else
		{
			i++;
		}
	}
	if (!sold)
		printf(RED "Erreur:" RESET " Actif non trouvé.\n");
}*/

void	handle_sell_asset(t_portfolio *portfolio)
{
	char	nom[15], date[12];
	float	quantite, prix;
	int		i = 0, sold = 0;

	printf("Nom de l'actif à vendre : ");
	scanf("%14s", nom);
	clear_stdin();

	while (i < portfolio->asset_count && !sold)
	{
		if (ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			printf("Quantité à vendre : ");
			scanf("%f", &quantite);
			clear_stdin();

			// Vérification de la quantité disponible AVANT appel à archive_sale
			if (quantite > portfolio->assets[i]->historique_quantite[portfolio->assets[i]->date_count - 1])
			{
				printf(RED "Erreur:" RESET " Quantité demandée dépasse la quantité disponible.\n");
				return;
			}

			printf("Prix de vente : ");
			scanf("%f", &prix);
			clear_stdin();
			printf("Date de vente : ");
			scanf("%11s", date);
			clear_stdin();

			// Archivage de la vente
			archive_sale(portfolio->assets[i], portfolio, quantite, prix, date);
			free_sold_assets(portfolio);
			sold = 1;
		}
		else
		{
			i++;
		}
	}
	if (!sold)
		printf(RED "Erreur:" RESET " Actif non trouvé.\n");
}

void	handle_export_asset(t_portfolio *portfolio)
{
	char	nom[15];
	char	filename[20];
	int		i = 0;
	int		found = 0;

	printf("Nom de l'actif à exporter : ");
	scanf("%49s", nom);
	clear_stdin();
	while (i < portfolio->asset_count)
	{
		if (ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			found = 1;
			printf("Nom du fichier CSV (ex: actif.csv / 20 carac. max) :");
			scanf("%49s", filename);
			clear_stdin();
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
	clear_stdin();
	while (i < portfolio->asset_count)
	{
		if (ft_strlen(portfolio->assets[i]->nom) > 0 && ft_strcmp(portfolio->assets[i]->nom, nom) == 0)
		{
			printf("Index de début (0 à %d) : ", portfolio->assets[i]->date_count - 1);
			scanf("%d", &start_index);
			clear_stdin();
			printf("Index de fin (0 à %d) :", portfolio->assets[i]->date_count - 1);
			scanf("%d", &end_index);
			clear_stdin();
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
	clear_stdin();

	if (choix[0] == 'O' || choix[0] == 'o')
	{
		free_sold_assets(portfolio);
		printf(GREEN "Tous les actifs entièrement vendus ont été supprimés.\n" RESET);
		return;
	}

	printf("Nom de l'actif à supprimer : ");
	scanf("%49s", nom);
	clear_stdin();

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

