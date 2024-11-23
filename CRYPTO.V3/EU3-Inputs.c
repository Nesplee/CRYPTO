/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EU3-Inputs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 00:35:15 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/22 02:13:51 by dinguyen         ###   ########.fr       */
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
	snprintf(debug_message, sizeof(debug_message), "Montant capturé : %.2f", amount);
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
		print_centered("Veuillez entrer une date au format YYYY-MM-DD :", LIGHT_BLUE);

		if (!fgets(date, size, stdin)) // Lecture sécurisée
		{
			print_centered("Erreur : Lecture échouée.", RED);
			return 0;
		}

		// Nettoyage du '\n' final
		date[strcspn(date, "\n")] = '\0';
		trim_whitespace(date);

		// Vérifier si la date est valide
		if (!is_valid_date(date))
		{
			print_centered("Erreur : Format de date invalide (YYYY-MM-DD attendu). Veuillez réessayer.", RED);
			continue; // Redemander une date si le format est invalide
		}

		// Affichage de la date capturée (debug)
		char debug_message[100];
		snprintf(debug_message, sizeof(debug_message), "Date capturée : '%s'", date);
		print_centered(debug_message, GRAY);

		return 1; // Si la date est valide, sortir de la boucle et retourner 1
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
