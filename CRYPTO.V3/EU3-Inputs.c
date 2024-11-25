/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EU3-Inputs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 00:35:15 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/25 04:23:08 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "A6.utils.h"

int input_name(char *name, size_t size)
{
	if (!name || size <= 1)
		return 0;

	// Demander à l'utilisateur d'entrer un nom
	print_centered("Veuillez entrer un nom :", LIGHT_BLUE);

	// Lire l'entrée de l'utilisateur
	if (!fgets(name, size, stdin)) // Lecture dans un tampon
	{
		print_centered("Erreur : Lecture échouée.", RED);
		return 0;
	}

	// Nettoyage du '\n' et des espaces superflus
	name[strcspn(name, "\n")] = '\0';  // Supprimer le caractère '\n' si présent
	trim_whitespace(name);

	// Vérifier si l'entrée est vide
	if (name[0] == '\0')
	{
		print_centered("Erreur : Entrée invalide (vide).", RED);
		return 0;
	}

	// Afficher le nom capturé (debug)
	char debug_message[100];
	snprintf(debug_message, sizeof(debug_message), "Nom capturé : '%s'", name);
	print_centered(debug_message, GRAY);

	return 1; // Nom valide
}

float input_amount()
{
	char buffer[50];
	float amount;

	// Demander à l'utilisateur d'entrer un montant
	print_centered("Veuillez entrer un montant :", GRAY);

	if (!fgets(buffer, sizeof(buffer), stdin)) // Lecture dans un tampon
	{
		print_centered("Erreur : Lecture échouée.", RED);
		return -1;
	}

	// Nettoyage du '\n' final et suppression des espaces
	buffer[strcspn(buffer, "\n")] = '\0';  // Supprimer le caractère '\n' si présent
	trim_whitespace(buffer);

	// Vérifier si le montant est valide
	if (sscanf(buffer, "%f", &amount) != 1 || amount <= 0)
	{
		print_centered("Montant invalide. Veuillez entrer un montant valide.", RED);
		pause_if_needed(1); // Attendre que l'utilisateur appuie sur Entrée pour continuer
		return -1;
	}

	// Afficher le montant capturé (debug)
	char debug_message[100];
	snprintf(debug_message, sizeof(debug_message), "Montant capturé : %.5f", amount);
	print_centered(debug_message, GRAY);

	return amount;
}

int input_date(char *date, size_t size)
{
    if (!date || size <= 1) // Validation des paramètres
        return 0;

    while (1) // Boucle infinie pour permettre plusieurs tentatives
    {
        // Demander à l'utilisateur d'entrer une date
        print_centered("Veuillez entrer une date au format [YYYY-MM-DD] :", LIGHT_BLUE);

        if (!fgets(date, size, stdin)) // Lecture sécurisée
        {
            print_centered("Erreur : Lecture échouée.", RED);
            return 0;
        }

        // Nettoyage du '\n' final
        date[strcspn(date, "\n")] = '\0';
        trim_whitespace(date);

        // Remplacer des séparateurs alternatifs
        size_t len = ft_strlen(date);
        for (size_t i = 0; i < len; i++)
        {
            if (date[i] == '.' || date[i] == '/')
                date[i] = '-';
        }

        // Validation des paramètres et affichage des erreurs spécifiques
        if (ft_strlen(date) != 10 || date[4] != '-' || date[7] != '-')
        {
            print_centered("Erreur : Format attendu [YYYY-MM-DD].", RED);
            continue;
        }

        int year, month, day;
        sscanf(date, "%4d-%2d-%2d", &year, &month, &day);

        // Vérifier l'année
        if (year < 1)
        {
            print_centered("Erreur : L'année doit être valide.", RED);
            continue;
        }

        // Vérifier le mois
        if (month < 1 || month > 12)
        {
            print_centered("Erreur : Le mois doit être compris entre 1 et 12.", RED);
            continue;
        }

        // Vérifier les jours dans chaque mois
        int max_days = 31;
        if (month == 4 || month == 6 || month == 9 || month == 11) // Mois avec 30 jours
        {
            max_days = 30;
        }
        else if (month == 2) // Février
        {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            {
                max_days = 29; // Année bissextile
            }
            else
            {
                max_days = 28; // Année non bissextile
            }
        }

        // Valider le jour
        if (day < 1 || day > max_days)
        {
            char error_message[100];
            snprintf(error_message, sizeof(error_message),
                     "Erreur : Ce mois contient seulement %d jours.", max_days);
            print_centered(error_message, RED);
            continue;
        }

        // Si tout est valide
        char success_message[100];
        snprintf(success_message, sizeof(success_message), "Date capturée : '%s'", date);
        print_centered(success_message, GREEN);
        return 1; // Date valide
    }
}



int	input_choice(int min, int max)
{
	char	buffer[50]; // Tampon pour stocker la saisie
	int		choice;

	if (!fgets(buffer, sizeof(buffer), stdin)) // Lecture sécurisée
	{
		print_centered("Erreur : Lecture échouée.", RED);
		return (-1);
	}

	// Vider le tampon si l'entrée dépasse la taille du buffer
	if (strchr(buffer, '\n') == NULL)
	{
		int	c;
		while ((c = getchar()) != '\n' && c != EOF);
	}

	// Conversion en entier et validation
	if (sscanf(buffer, "%d", &choice) != 1 || choice < min || choice > max)
	{
		char	error_message[100];
		snprintf(error_message, sizeof(error_message), "Erreur : Choix invalide. Veuillez entrer un nombre entre %d et %d.", min, max);
		print_centered(error_message, RED);
		return (-1);
	}

	return (choice);
}

