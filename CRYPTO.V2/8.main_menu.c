/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8.main_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:35:39 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/19 02:57:46 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void main_menu(t_portfolio_manager *manager)
{
	char choix[3];
	int exit_program = 0;
	int	error_flag = 0;

	while (!exit_program)
	{

		clear_screen();
		display_menu_header("MENU PRINCIPAL");
		if (error_flag)
		{
			printf(RED "Une erreur s'est produite. Veuillez réessayer.\n" RESET);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
			error_flag = 0;
			continue;
		}
		printf(GRAY "1 - Gérer les portefeuilles\n" RESET);
		printf(GRAY "2 - Créer un nouveau portefeuille\n" RESET);
		printf(GRAY "3 - Charger les portefeuilles\n" RESET);
		printf(GRAY "0 - Quitter\n" RESET);
		printf(GRAY "===========================================\n" RESET);
		printf(GRAY "Entrez votre choix : " RESET);

		if (!fgets(choix, sizeof(choix), stdin))
		{
			printf(RED "Erreur : Entrée invalide.\n" RESET);
			error_flag = 1;
			continue;
		}

		if (choix[0] == '1')
			portfolio_menu(manager);
		else if (choix[0] == '2')
		{
			char name[50];
			printf("Entrez un nom pour le portefeuille : ");
			if (!fgets(name, sizeof(name), stdin) || name[0] == '\n')
			{
				printf(RED "Erreur: Lecture du nom échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			name[strcspn(name, "\n")] = '\0';
			if (!add_portfolio(manager, name))
			{
				printf(RED "Erreur: Impossible de créer le portefeuille.\n" RESET);
				error_flag = 1;
			}
			else
			{
				printf(GREEN "Portefeuille '%s' créé avec succès !\n" RESET, name);
				printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
				clear_stdin();
			}
		}
		else if (choix[0] == '3')
		{
			manager = load_all_portfolios("portfolios.txt");
			if (!manager)
			{
				printf(RED "Erreur: Chargement des portefeuilles a échoué.\n" RESET);
				error_flag = 1;
			}
			else
				printf(GREEN "Portefeuilles chargés avec succès.\n" RESET);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '0')
		{
			exit_program = 1;
			printf(GREEN "Merci d'avoir utilisé le gestionnaire de portefeuille.\n" RESET);
		}
		else
		{
			printf(RED "Choix invalide. Veuillez réessayer.\n" RESET);
			error_flag = 1;
		}
	}
	save_all_portfolios(manager, "portfolios.txt");
}

void portfolio_menu(t_portfolio_manager *manager)
{
	char choix[3];
	int error_flag = 0;
	int index;

	while (1)
	{
		clear_screen(); // Nettoie l'écran à chaque itération
		display_menu_header("GERER LES PORTEFEUILLES");

		if (error_flag)
		{
			printf(RED "Une erreur s'est produite. Veuillez réessayer.\n" RESET);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
			error_flag = 0;
			continue;
		}

		printf(GRAY "1 - Afficher tous les portefeuilles\n" RESET);
		printf(GRAY "2 - Sélectionner un portefeuille\n" RESET);
		printf(GRAY "3 - Supprimer un portefeuille\n" RESET);
		printf(GRAY "0 - Retour\n" RESET);
		printf(GRAY "===========================================\n" RESET);
		printf(GRAY "Entrez votre choix : " RESET);

		if (!fgets(choix, sizeof(choix), stdin))
		{
			error_flag = 1;
			continue;
		}

		if (choix[0] == '1')
		{
			if (!manager || manager->portfolio_count == 0)
			{
				printf(RED "Erreur : Aucun portefeuille disponible.\n" RESET);
				printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
				clear_stdin();
				continue;
			}
			display_all_portfolios(manager);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '2')
		{
			printf("Entrez l'index du portefeuille : ");
			if (!fgets(choix, sizeof(choix), stdin) || sscanf(choix, "%d", &index) != 1)
			{
				printf(RED "Erreur: Lecture de l'index échouée.\n" RESET);
				clear_stdin();
				error_flag = 1;
				continue;
			}
			if (index >= 0 && index < manager->portfolio_count)
			{
				wallet_menu(manager->portfolios[index]);
			}
			else
			{
				printf(RED "Erreur: Index invalide.\n" RESET);
				error_flag = 1;
			}
		}
		else if (choix[0] == '3')
		{
			printf("Entrez l'index du portefeuille à supprimer : ");
			if (!fgets(choix, sizeof(choix), stdin) || sscanf(choix, "%d", &index) != 1)
			{
				printf(RED "Erreur: Lecture de l'index échouée.\n" RESET);
				clear_stdin();
				error_flag = 1;
				continue;
			}
			if (index >= 0 && index < manager->portfolio_count)
				delete_portfolio(manager, index);
			else
			{
				printf(RED "Erreur: Index invalide. Veuillez réessayer.\n" RESET);
				error_flag = 1;
			}
		}
		else if (choix[0] == '0')
			return;
		else
		{
			printf(RED "Choix invalide. Veuillez réessayer.\n" RESET);
			error_flag = 1;
		}
	}
}

void wallet_menu(t_portfolio *portfolio)
{
	char	choix[3];       // Buffer pour les choix du menu
	int		error_flag = 0; // Indicateur d'erreur

	while (1)
	{
		clear_screen(); // Nettoie l'écran à chaque itération
		display_menu_header("GERER UN PORTEFEUILLE");

		if (error_flag)
		{
			printf(RED "Une erreur s'est produite. Veuillez réessayer.\n" RESET);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
			error_flag = 0;
			continue;
		}

		printf(GRAY "1 - Afficher le portefeuille\n" RESET);
		printf(GRAY "2 - Ajouter des dollars\n" RESET);
		printf(GRAY "3 - Retirer des dollars\n" RESET);
		printf(GRAY "4 - Gérer les actifs\n" RESET);
		printf(GRAY "0 - Retour\n" RESET);
		printf(GRAY "===========================================\n" RESET);
		printf(GRAY "Entrez votre choix : " RESET);

		if (!fgets(choix, sizeof(choix), stdin))
		{
			error_flag = 1;
			continue;
		}

		if (choix[0] == '1')
		{
			display_portfolio(portfolio);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '2') // Ajouter des dollars
		{
			float	amount;
			char	montant_str[10]; // Buffer dédié pour lire le montant
			char	date[12];

			printf("Montant à ajouter : ");
			if (!fgets(montant_str, sizeof(montant_str), stdin) || sscanf(montant_str, "%f", &amount) != 1 || amount <= 0)
			{
				printf(RED "Erreur: Montant invalide. Entrez un nombre supérieur à 0.\n" RESET);
				error_flag = 1;
				continue;
			}

			printf("Date (YYYY-MM-DD) : ");
			if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
			{
				printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			date[strcspn(date, "\n")] = '\0'; // Retirer le '\n'

			add_dollars(portfolio, amount, date);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '3') // Retirer des dollars
		{
			float	amount;
			char	montant_str[10]; // Buffer dédié pour lire le montant
			char	date[12];

			printf("Montant à retirer : ");
			if (!fgets(montant_str, sizeof(montant_str), stdin) || sscanf(montant_str, "%f", &amount) != 1 || amount <= 0)
			{
				printf(RED "Erreur: Montant invalide. Entrez un nombre supérieur à 0.\n" RESET);
				error_flag = 1;
				continue;
			}

			printf("Date (YYYY-MM-DD) : ");
			if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
			{
				printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			date[strcspn(date, "\n")] = '\0'; // Retirer le '\n'

			withdraw_dollars(portfolio, amount, date);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '4')
		{
			asset_menu(portfolio);
		}
		else if (choix[0] == '0')
		{
			return;
		}
		else
		{
			printf(RED "Choix invalide. Veuillez réessayer.\n" RESET);
			error_flag = 1;
		}
	}
}

/*void wallet_menu(t_portfolio *portfolio)
{
	char choix[3];
	int error_flag = 0;

	while (1)
	{
		clear_screen(); // Nettoie l'écran à chaque itération
		display_menu_header("GERER UN PORTEFEUILLE");

		if (error_flag)
		{
			printf(RED "Une erreur s'est produite. Veuillez réessayer.\n" RESET);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
			error_flag = 0;
			continue;
		}
		printf(GRAY "1 - Afficher le portefeuille\n" RESET);
		printf(GRAY "2 - Ajouter des dollars\n" RESET);
		printf(GRAY "3 - Retirer des dollars\n" RESET);
		printf(GRAY "4 - Gérer les actifs\n" RESET);
		printf(GRAY "0 - Retour\n" RESET);
		printf(GRAY "===========================================\n" RESET);
		printf(GRAY "Entrez votre choix : " RESET);

		if (!fgets(choix, sizeof(choix), stdin))
		{
			error_flag = 1;
			continue;
		}

		if (choix[0] == '1')
		{
			display_portfolio(portfolio);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '2')
		{
			float amount;
			char date[12];
			printf("Montant à ajouter : ");
			if (!fgets(choix, sizeof(choix), stdin) || sscanf(choix, "%f", &amount) != 1)
			{
				printf(RED "Erreur: Lecture du montant échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			printf("Date (YYYY-MM-DD) : ");
			if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
			{
				printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			date[strcspn(date, "\n")] = '\0'; // Retirer le '\n'
			add_dollars(portfolio, amount, date);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '3')
		{
			float amount;
			char date[12];
			printf("Montant à retirer : ");
			if (!fgets(choix, sizeof(choix), stdin) || sscanf(choix, "%f", &amount) != 1)
			{
				printf(RED "Erreur: Lecture du montant échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			printf("Date (YYYY-MM-DD) : ");
			if (!fgets(date, sizeof(date), stdin) || date[0] == '\n')
			{
				printf(RED "Erreur: Lecture de la date échouée.\n" RESET);
				error_flag = 1;
				continue;
			}
			date[strcspn(date, "\n")] = '\0'; // Retirer le '\n'
			withdraw_dollars(portfolio, amount, date);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '4')
		{
			asset_menu(portfolio);
		}
		else if (choix[0] == '0')
		{
			return;
		}
		else
		{
			printf(RED "Choix invalide. Veuillez réessayer.\n" RESET);
			error_flag = 1;
		}
	}
}*/

void asset_menu(t_portfolio *portfolio)
{
	char choix[3];
	int error_flag = 0;

	while (1)
	{
		clear_screen();
		display_menu_header("GERER LES ACTIFS");

		if (error_flag)
		{
			printf(RED "Une erreur s'est produite. Veuillez réessayer.\n" RESET);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
			error_flag = 0;
			continue;
		}
		printf(GRAY "1 - Ajouter un actif\n" RESET);
		printf(GRAY "2 - Mettre à jour un actif\n" RESET);
		printf(GRAY "3 - Vendre un actif\n" RESET);
		printf(GRAY "4 - Afficher les actifs\n" RESET);
		printf(GRAY "0 - Retour\n" RESET);
		printf(GRAY "===========================================\n" RESET);
		printf(GRAY "Entrez votre choix : " RESET);

		if (!fgets(choix, sizeof(choix), stdin))
		{
			printf(RED "Erreur : Entrée invalide.\n" RESET);
			error_flag = 1;
			continue;
		}
		if (choix[0] == '1')
		{
			handle_add_asset(portfolio);
		}
		else if (choix[0] == '2')
		{
			handle_update_asset(portfolio);
		}
		else if (choix[0] == '3')
		{
			handle_sell_asset(portfolio);
		}
		else if (choix[0] == '4')
		{
			display_portfolio(portfolio);
			printf(GRAY "Appuyez sur Entrée pour continuer...\n" RESET);
			clear_stdin();
		}
		else if (choix[0] == '0')
		{
			return;
		}
		else
		{
			printf(RED "Choix invalide. Veuillez réessayer.\n" RESET);
			error_flag = 1;
		}
	}
}
