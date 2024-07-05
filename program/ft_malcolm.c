#include "../header/ft_malcolm.h"

//volatile= indiquer au compilateur qu'elle peut être modifiée par un événement externe (comme un signal)
extern volatile bool STOP;

static void     control_c(int value)    
{
        if (value == SIGINT)
        {
                printf(YELLOW "\n[INFO] " RESET);
                printf("Ctrl+C has been pressed...\nShutting down the program properly.\n");
                STOP = true;
        }
}

static int      main_error_msg(char *msg, int value)
{
        printf ("%s", msg);
        return (value);
}

int     main(int ac, char **av)
{
        char            *ip_source = av[1];
        char            *mac_source = av[2];
        char            *ip_target = av[3];
        char            *mac_target = av[4];

        int             recv_size = 0;
        socklen_t       saddr_size = 0;
        STOP = false;
        //Pour stocker l'addr IP à l'origne du paquet
        struct sockaddr saddr;

        struct sigaction sa;
        sa.sa_handler = control_c;
        // pas de comportements spéciaux activés
        sa.sa_flags = 0;
        // aucun autre signal ne sera bloqué pendant que le gestionnaire 
        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGINT, &sa, NULL) == -1)
        {
                perror("sigaction");
                exit(EXIT_FAILURE);
        }

        if (!parsing(ac, av))
                return (1);

        printf (GREEN "[INFO FT_MALCOLM]" RESET " Datas in input are valid ✅\n\n");

        //Allouer un buffer de 65536 octets garantit que le programme peut gérer la taille maximale possible d'un paquet IP
        //unsigned char: assure la compatibilité avec les API et garantit une manipulation correcte des données binaires
        unsigned char    *buff = (unsigned char *)malloc(65536);
        if (!buff)
                return (main_error_msg(RED "[ERROR MALLOC]" RESET " Something went wrong with the allocation of memory...\nEnding the program safely.\n", 1));

        // La fonction socket() renvoie un entier qui correspond à un descripteur du socket nouvellement créé et qui sera passé
        // en paramètre aux fonctions suivantes. En cas d'erreur, la fonction socket() retourne -1
        int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
        if (fd == -1)
                return (main_error_msg(RED "[ERROR]" RESET " Something went wrong with the socket() function...\nEnding the program safely.\n", 1));

        while (!STOP)
        {
                // recvfrom a besoin de connaitre l'espace dispo pour stocker l'addr source (dans struct saddr du coup) 
                saddr_size = sizeof(saddr);

                //fonction bloquante qui attend la capture d'un paquet ARP 
                //retourne le nbr d'octets reçus
                recv_size = recvfrom(fd, buff, 65536, 0, &saddr, &saddr_size);

                if (recv_size < 0)
                {
                        if (errno == EINTR)
                        {
                                // Interruption control + C
                                //  ne pas traiter cette interruption comme une erreur fatale
                                continue;
                        }
                        main_error_msg(RED "[ERROR RECVFROM]" RESET " Something went wrong with the recvfrom() function...\nEnding the program safely.\n", 1);
                        break;
                }
                process_packet(fd, buff, ip_source, ip_target, mac_source, mac_target);
        }
        if (buff)
                free(buff);
        close (fd);
        return (0);
}