/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 06:31:10 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/17 06:31:58 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "my_prog.h"

void test_resize_arrays()
{
    printf(GRAY "Test de la fonction resize_arrays :\n" RESET);

    t_point *asset = (t_point *)malloc(sizeof(t_point));
    if (!asset)
    {
        printf(RED "Erreur d'allocation pour l'actif lors du test.\n" RESET);
        return;
    }
    asset->nom = ft_strdup("BTC");
    asset->max_dates = 2;
    asset->date_count = 2;
    asset->dates = (char **)malloc(sizeof(char *) * asset->max_dates);
    asset->historique_prix = (float *)malloc(sizeof(float) * asset->max_dates);
    asset->historique_quantite = (float *)malloc(sizeof(float) * asset->max_dates);
    asset->historique_diff_begin = (float *)malloc(sizeof(float) * asset->max_dates);
    asset->historique_percent_begin = (float *)malloc(sizeof(float) * asset->max_dates);

    // Remplissage des données initiales
    asset->dates[0] = ft_strdup("2024-11-15");
    asset->dates[1] = ft_strdup("2024-11-16");
    asset->historique_prix[0] = 1000.0;
    asset->historique_prix[1] = 1500.0;
    asset->historique_quantite[0] = 1.0;
    asset->historique_quantite[1] = 1.5;
    asset->historique_diff_begin[0] = 0.0;
    asset->historique_diff_begin[1] = 500.0;
    asset->historique_percent_begin[0] = 0.0;
    asset->historique_percent_begin[1] = 50.0;

    // Test du redimensionnement
    if (resize_arrays(asset))
        printf(GREEN "Redimensionnement réussi ! Nouvelle taille max : %d\n" RESET, asset->max_dates);
    else
        printf(RED "Redimensionnement échoué.\n" RESET);

    // Nettoyage
    free_asset(asset);
}

void test_load_portfolio()
{
    printf(GRAY "Test de la fonction load_portfolio :\n" RESET);

    // Cas 1 : Fichier inexistant
    t_portfolio *portfolio = load_portfolio("fichier_inexistant.txt");
    if (portfolio && portfolio->asset_count == 0)
        printf(GREEN "Test fichier inexistant réussi.\n" RESET);
    else
        printf(RED "Erreur : gestion des fichiers inexistants incorrecte.\n" RESET);

    // Cas 2 : Fichier corrompu
    FILE *file = fopen("corrupted_file.txt", "w");
    fprintf(file, "Données invalides\n");
    fclose(file);

    portfolio = load_portfolio("corrupted_file.txt");
    if (portfolio == NULL)
        printf(GREEN "Test fichier corrompu réussi.\n" RESET);
    else
        printf(RED "Erreur : gestion des fichiers corrompus incorrecte.\n" RESET);

    // Nettoyage
    free_portfolio(portfolio);
    remove("corrupted_file.txt");
}

void test_archive_sale()
{
    printf(GRAY "Test de la fonction archive_sale :\n" RESET);

    t_portfolio *portfolio = create_portfolio(2);
    t_point *asset = create_asset(portfolio, "BTC", "2024-11-15", 1000.0, 1.0);

    // Cas de vente valide
    archive_sale(asset, portfolio, 0.5, 1500.0, "2024-11-16");
    if (asset->sale_count == 1 && portfolio->dollar_balance == 750.0)
        printf(GREEN "Test vente valide réussi.\n" RESET);
    else
        printf(RED "Erreur : gestion incorrecte de la vente valide.\n" RESET);

    // Cas de quantité supérieure à la disponible
    archive_sale(asset, portfolio, 2.0, 1500.0, "2024-11-17");
    if (asset->sale_count == 1) // La vente ne doit pas être enregistrée
        printf(GREEN "Test vente invalide réussi.\n" RESET);
    else
        printf(RED "Erreur : vente invalide incorrectement enregistrée.\n" RESET);

    // Nettoyage
    free_portfolio(portfolio);
}
