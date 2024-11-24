/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EU2-UTILS-global.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dinguyen <dinguyen@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:00:36 by dinguyen          #+#    #+#             */
/*   Updated: 2024/11/24 05:01:34 by dinguyen         ###   ########.fr       */
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
	int year = 0, month = 0, day = 0;
	int is_leap = 0;

	// Vérifier la longueur exacte et le format général [YYYY-MM-DD]
	if (!date || ft_strlen((char *)date) != 10)
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

	// Extraire les valeurs numériques de l'année, du mois et du jour
	sscanf(date, "%d-%d-%d", &year, &month, &day);

	// Vérifier la validité du mois (1-12)
	if (month < 1 || month > 12)
		return (0);

	// Vérifier la validité du jour (1-31)
	if (day < 1 || day > 31)
		return (0);

	// Vérifier les jours en fonction du mois
	if (month == 4 || month == 6 || month == 9 || month == 11) // Avril, Juin, Septembre, Novembre
	{
		if (day > 30)
			return (0);
	}

	// Vérifier pour février (bissextile ou non)
	if (month == 2)
	{
		if (year % 4 == 0)
		{
			if (year % 100 == 0)
			{
				if (year % 400 == 0)
					is_leap = 1; // Année divisible par 400 -> bissextile
				else
					is_leap = 0; // Année divisible par 100 mais pas par 400 -> pas bissextile
			}
			else
			{
				is_leap = 1; // Année divisible par 4 mais pas par 100 -> bissextile
			}
		}
		else
		{
			is_leap = 0; // Année non divisible par 4 -> pas bissextile
		}

		// Vérification du nombre de jours en février
		if (is_leap == 1 && day > 29)
			return (0);
		if (is_leap == 0 && day > 28)
			return (0);
	}

	// Si toutes les vérifications passent, la date est valide
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
		int text_length = stripped_strlen(text ? text : ""); // Éviter null
		padding = (w.ws_col - text_length) / 2;
		if (padding < 0) padding = 0;
	}

	// Imprimer les espaces puis le texte coloré
	if (text)
	{
		printf("%*s%s%s%s\n", padding, "", color ? color : "", text, RESET);
	}
	else
	{
		// Imprimer une ligne vide si le texte est nul
		printf("\n");
	}
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

void strip_colors(const char *input, char *output, size_t max_len)
{
	size_t i = 0, j = 0;
	int in_color = 0;

	while (input[i] != '\0' && j < max_len - 1)
	{
		if (input[i] == '\033') // Détecte le début d'une séquence ANSI
			in_color = 1;

		if (!in_color)
			output[j++] = input[i];

		if (in_color && input[i] == 'm') // Fin d'une séquence ANSI
			in_color = 0;

		i++;
	}

	output[j] = '\0';
}


//			DATEEEEEESSSS

// Comparateur pour les dates
int compare_dates(const char *date1, const char *date2)
{
	// Les dates sont au format [YYYY-MM-DD], donc une simple comparaison lexicographique suffit
	return strcmp(date1, date2);
}

// Fonction de tri par date
void sort_by_date(t_sale *sales, int count)
{
	int i, j;

	if (!sales || count <= 1)
		return;

	t_sale temp;

	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			// Comparer les dates et échanger si nécessaire
			if (compare_dates(sales[j].date, sales[j + 1].date) > 0)
			{
				// Échanger les ventes
				temp = sales[j];
				sales[j] = sales[j + 1];
				sales[j + 1] = temp;
			}
		}
	}
}

int compare_movements(const void *a, const void *b)
{
	t_movement *move_a = (t_movement *)a;
	t_movement *move_b = (t_movement *)b;
	return strcmp(move_a->date, move_b->date);
}

t_movement *combine_and_sort_date(t_portfolio *portfolio, int *total_count)
{
		if (!portfolio || !total_count)
			return NULL;

		int movement_count = 0;
		int i, j;

		// Calcul du nombre total de mouvements
		movement_count += portfolio->transaction_count;
		for (i = 0; i < portfolio->asset_count; i++)
		{
			movement_count += portfolio->assets[i]->historique_count; // Achats/Updates
			movement_count += portfolio->assets[i]->sale_count;       // Ventes
		}

		// Allouer un tableau de mouvements
		t_movement *movements = malloc(movement_count * sizeof(t_movement));
		if (!movements)
			return NULL;

		int index = 0;

		// Ajouter les transactions
		for (i = 0; i < portfolio->transaction_count; i++)
		{
			movements[index].type = strdup("Transaction");
			movements[index].date = strdup(portfolio->transactions[i].date);
			movements[index].nom = NULL; // Pas de nom pour les transactions
			movements[index].amount = portfolio->transactions[i].amount;
			movements[index].price = 0.0f; // Pas de prix pour les transactions
			movements[index].pnl = 0.0f;   // Pas de PNL pour les transactions
			index++;
		}

		// Ajouter les achats/updates
		for (i = 0; i < portfolio->asset_count; i++)
		{
			t_asset *asset = portfolio->assets[i];
			for (j = 0; j < asset->historique_count; j++)
			{
				movements[index].type = strdup("Achat/Update");
				movements[index].date = strdup(asset->historique[j].date);
				movements[index].nom = strdup(asset->nom);
				movements[index].amount = asset->historique[j].quantite;
				movements[index].price = asset->historique[j].prix;
				movements[index].pnl = 0.0f; // Pas de PNL pour les achats/updates
				index++;
			}
		}

		// Ajouter les ventes
		for (i = 0; i < portfolio->asset_count; i++)
		{
			t_asset *asset = portfolio->assets[i];
			for (j = 0; j < asset->sale_count; j++)
			{
				movements[index].type = strdup("Vente");
				movements[index].date = strdup(asset->sales[j].date);
				movements[index].nom = strdup(asset->nom);
				movements[index].amount = asset->sales[j].quantite_vendue;
				movements[index].price = asset->sales[j].prix_vente;
				movements[index].pnl = asset->sales[j].profit_loss_exit;
				index++;
			}
		}

		// Trier les mouvements par date
		qsort(movements, movement_count, sizeof(t_movement), compare_movements);

		*total_count = movement_count;
		return movements;
}
