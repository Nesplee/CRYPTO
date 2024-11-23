/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EU2-UTILS-global.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:00:36 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/23 15:09:57 by dinguyen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "A6.utils.h"

void	ft_putchar(char c)
{
	printf("%c", c);
}

int	ft_strlen(const char *str)
{
	int	i = 0;

	if (str == NULL)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	ft_putnbr(int n)
{
	char	r;

	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		ft_putchar('\n');
		return ;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	if (n > 9)
		ft_putnbr(n / 10);
	r = (n % 10 ) + '0';
	ft_putchar(r);
}

void	print_line(char c, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		ft_putchar(c);
		i++;
	}
	ft_putchar('\n');
}

static int ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	is_snake_case(const char *str)
{
	int	i = 0;

	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int is_only_spaces(const char *str)
{
	int i = 0;

	if (!str || str[0] == '\0')
		return 1; // Chaîne vide considérée comme "espaces seulement"

	while (str[i] != '\0')
	{
		if (str[i] != ' ') // Si un caractère non-espace est trouvé
			return 0;
		i++;
	}

	return 1; // Tous les caractères sont des espaces
}

int is_valid_date(const char *date)
{
	int i = 0;

	if (ft_strlen((char *)date) != 10)
		return (0);

	while (date[i])
	{
		if (i == 4 || i == 7)
		{
			if (date[i] != '-')
				return (0);
		}
		else if (date[i] < '0' || date[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i = 0;

	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;

	return (s1[i] - s2[i]);
}

char	*ft_strdup(const char *str)
{
	char	*duplicate;
	int		len;
	int		i;

	if (str == NULL)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	duplicate = (char *)malloc(sizeof(char) * (len + 1));
	if (duplicate == NULL)
		return (NULL);
	while (str[i])
	{
		duplicate[i] = str[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}

void clear_screen(void)
{
	printf("\033[H\033[J"); // Escape code pour effacer l'écran
}

int get_terminal_width(void)
{
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
		return 80;
	return w.ws_col;
}

size_t stripped_strlen(const char *str)
{
		if (!str) // Si la chaîne est NULL, on retourne 0
			return 0;

		size_t len = 0;

		while (*str)
		{
			if (*str == '\033') // Détection de l'échappement pour les couleurs
			{
				// Ignorer toute la séquence d'échappement
				while (*str && *str != 'm')
					str++;
				if (*str) // Avancer après 'm'
					str++;
			}
			else
			{
				len++; // Compter les caractères utiles
				str++;
			}
		}

		return len;
}


void print_centered(const char *text, const char *color)
{
	struct winsize w;
	int padding;

	// Récupérer la largeur du terminal
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
	{
		// Valeur par défaut si la largeur n'est pas récupérée
		padding = 40;
	}
	else
	{
		// Calculer le nombre d'espaces nécessaires pour centrer
		int text_length = stripped_strlen(text);
		padding = (w.ws_col - text_length) / 2;
		if (padding < 0) padding = 0;
	}

	// Imprimer les espaces puis le texte coloré
	printf("%*s%s%s%s\n", padding, "", color, text, RESET);
}

void clear_stdin()
{
	while (getchar() != '\n' && !feof(stdin));
}

void pause_if_needed(int pause_needed)
{
	if (pause_needed)
	{
		print_centered("Appuyez sur Entrée pour continuer...", GRAY);
		clear_stdin();  // Nettoie tout résidu d'entrée dans le tampon
	}
}


void display_asset_manager_header()
{
    print_centered(RED "								 █████╗ ███████╗███████╗███████╗████████╗    ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗   " RESET, RED);
    print_centered(RED "								██╔══██╗██╔════╝██╔════╝██╔════╝╚══██╔══╝    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗  " RESET, RED);
    print_centered(RED "								███████║███████╗███████╗█████╗     ██║       ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝ " RESET, RED);
    print_centered(RED "								██╔══██║╚════██║╚════██║██╔══╝     ██║       ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗  " RESET, RED);
    print_centered(RED "								██║  ██║███████║███████║███████╗   ██║       ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║  " RESET, RED);
    print_centered(RED "								╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝   ╚═╝       ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝  " RESET, RED);
    print_centered(RED "                                                                                                    v1. by Nesplee" RESET, RED);
}

int	ask_confirmation(const char *message)
{
	char	response;

	print_centered(message, LIGHT_BLUE);

	while (1)
	{
		// Lecture de la réponse
		response = getchar();

		// Nettoyer le buffer restant si l'utilisateur a saisi plus d'un caractère
		while (getchar() != '\n');

		// Vérification de la réponse
		if (response == 'o' || response == 'O')
		{
			print_centered("Confirmation: OUI.", GREEN);
			return (1);
		}
		else if (response == 'n' || response == 'N')
		{
			print_centered("Confirmation: NON.", RED);
			return (0);
		}
		else
		{
			print_centered("Réponse invalide. Veuillez répondre par 'O' ou 'N'.", RED);
		}
	}
}

void trim_whitespace(char *str)
{
	char *end;

	// Supprimer les espaces en début
	while (isspace((unsigned char)*str)) str++;

	// Supprimer les espaces en fin
	if (*str == 0) return; // Chaîne vide

	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	// Terminer par un '\0'
	*(end + 1) = 0;
}


void log_message(const char *message)
{
	FILE *log_file = fopen("debug.log", "a");
	if (log_file)
	{
		fprintf(log_file, "%s\n", message);
		fclose(log_file);
	}
}

