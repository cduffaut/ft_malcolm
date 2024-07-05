#include "../header/ft_malcolm.h"

static bool	tab_len(char **tab, int len)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	
	if (i != len)
		return (false);
	
	return (true);
}

static bool	non_numeric(char **tab)
{
	int	i;
	int j;

	i = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			if (!ft_isdigit(tab[i][j]))
				return (false);	
			j++;
		}
		i++;
	}
	return (true);
}

static int	char_count(char *str, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (str[i])
	{
		if (str[i] == c)
			counter++;
		i++;
	}
	return (counter);
}

static void free_tab(char **tab)
{
	int	i;
	
	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			free (tab[i]);
			tab[i] = NULL;
			i++;
		}
		free (tab);
		tab = NULL;
	}
}

static bool error_msg(char *msg, char **tab, bool boolean)
{
	if (tab)
		free_tab(tab);
	printf ("%s", msg);
	return (boolean);
}

static bool number_range(char **tab)
{
	int	i;
	int	nbr;

	i = 0;
	while (tab[i])
	{
		nbr = ft_atoi(tab[i]);
		if (nbr < 0 || nbr > 255)
			return (false);
		i++;
	}
	return (true);
}

// Crée un tab, vérifie si la len correspond à un type IPV4
static bool	parsing_ip(char *str)
{
	char	**tab;

	tab = NULL;
	if (char_count(str, '.') != 3)
		return (error_msg(RED "[ERROR IP] " RESET "At least one IP does not contain the right amount of dots.\n", NULL, false));
	tab = ft_split(str, '.');
	if (!tab_len(tab, 4))
		return (error_msg (RED "[ERROR IP] " RESET "At least one IP is not in a valid IPV4 format.\n", tab, false));
	if (!non_numeric(tab))
		return (error_msg (RED "[ERROR IP] " RESET "There is some non numeric characters in at least one IP.\n", tab, false));
	if (!number_range(tab))
		return (error_msg (RED "[ERROR IP] " RESET "There is some numbers in at least one IP that does not fit the range 0 to 255.\n", tab, false));
	free_tab(tab);
	return (true);
}

// Return value: 1 = Error Hexadecimal format
// Return value: 2 = Error len per str
static int hexa_and_len(char **tab)
{
	int	i;
	int j;

	i = 0;
	j = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			if ((tab[i][j] < 65 && (tab[i][j] < 48 || tab[i][j] > 57)) || (tab[i][j] >= 71 && tab[i][j] <= 96) || tab[i][j] > 103)
				return (1);
			j++;
		}
		if (j != 2)
			return (2);
		i++;
	}
	return (0);
}

// 2 chiffres par strings
// Entre a et f, A et F, 0 et 9
static bool	parsing_mac(char *str)
{
	char	**tab;
	int		tmp;

	tmp = 0;
	tab = NULL;
	if (char_count(str, ':') != 5)
		return (error_msg(BLUE "[ERROR MAC] " RESET "At least one MAC addr does not contain the right amount of \":\".\n", NULL, false));
	tab = ft_split(str, ':');
	if (!tab_len(tab, 6))
		return (error_msg (BLUE "[ERROR MAC] " RESET "At least one MAC addr is not in a 6 column format.\n", tab, false));	
	tmp = hexa_and_len(tab);
	if (tmp == 1 || tmp == 2)
		return (error_msg (BLUE "[ERROR MAC] " RESET "Each MAC addresses should have two hexadecimal element per column.\n", tab, false));	
	free_tab(tab);
	return (true);
}

bool	parsing(int ac, char **av)
{
	if (ac != 5)
		return (error_msg (MAGENTA "[ERROR INPUTS] " RESET "The numbers of arguments in input is not correct.\n" MAGENTA "Please refer to ther format: " RESET "./ft_malcolm.c [source ip] [source mac] [target ip] [target mac]\n", NULL, false));
	if (!parsing_ip(av[1]))
		return (false);
	if (!parsing_ip(av[3]))
		return (false);
	if (!parsing_mac(av[2]))
		return (false);
	if (!parsing_mac(av[4]))
		return (false);
	if (!ft_strcmp(av[2], av[4]) || !ft_strcmp(av[1], av[3]))
		return (error_msg(RED "[ERROR DOUBLONS] " RESET "IPs addresses or MAC addresses are the same.\n", NULL, false));
	return (true);
}