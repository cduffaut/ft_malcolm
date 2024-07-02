#include "../header/ft_malcolm.h"

volatile bool STOP = false;

static void control_c(int value)
{
	if (value == SIGINT)
	{
		printf(YELLOW "\n[INFO] "RESET);
		printf("Ctrl+C has been pressed...\nShutting down the program properly.\n");
		STOP = true;
	}
}

static int main_error_msg(char *msg, int value)
{
	printf ("%s", msg);
	return (value);
}

int	main(int ac, char **av)
{
	char *ip_source = av[1];
	char *mac_source = av[2];
	char *ip_target = av[3];
	char *mac_target = av[4]; 
 	
	signal(SIGINT, control_c);
	
	if (!parsing(ac, av))
		return (0);

	printf (GREEN "[INFO FT_MALCOLM]" RESET " Datas in input are valid ✅\n\n");

	struct sockaddr_in sa_target;
	//convertir cette chaîne de charactères en entier
	int res = inet_pton(AF_INET, ip_target, &(sa_target.sin_addr));
	if (res == -1 || res == 0)
		return (main_error_msg(RED "[ERROR]" RESET " Something went wrong with the inet_pton() function...\nEnding the program safely.\n", 1));

	// La fonction socket() renvoie un entier qui correspond à un descripteur du socket nouvellement créé et qui sera passé
	// en paramètre aux fonctions suivantes. En cas d'erreur, la fonction socket() retourne -1
	int fd = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
	if (fd == -1)
		return (main_error_msg(RED "[ERROR]" RESET " Something went wrong with the socket() function...\nEnding the program safely.\n", 1));

	while (true)
	{
		data_size = recvfrom(fd , buffer , 65536 , 0 , &saddr , &saddr_size);

		printf (CYAN "Dans la boucle...\n" RESET);
		sleep (3);
		if (STOP)
			break;
	}
	return (0);
}