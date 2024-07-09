#include "../header/ft_malcolm.h"

volatile bool STOP;

// Fonction pour imprimer l'adresse MAC
void print_mac_address(unsigned char *mac)
{
	printf("%02x:%02x:%02x:%02x:%02x:%02x",
		   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
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
		printf(BLUE "[ARP REQUEST] " RESET "An ARP request has been cached!\n");

		// Récupérer l'en-tête ARP
		struct ether_arp *arp_packet = (struct ether_arp *)(buff + sizeof(struct ethhdr));

		// Convertir les adresses IP source et destination de la requête ARP en chaînes de caractères
		char sender_ip[INET_ADDRSTRLEN], target_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, arp_packet->arp_spa, sender_ip, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, arp_packet->arp_tpa, target_ip, INET_ADDRSTRLEN);

		// Afficher les détails de la requête ARP reçue
		printf(YELLOW "[INFO] " RESET "Received ARP request details:\n");
		printf(YELLOW "[INFO] " RESET "Sender MAC: ");
		print_mac_address(arp_packet->arp_sha);
		printf("\n");
		printf(YELLOW "[INFO] " RESET "Sender IP: %s\n", sender_ip);
		printf(YELLOW "[INFO] " RESET "Target MAC: ");
		print_mac_address(arp_packet->arp_tha);
		printf("\n");
		printf(YELLOW "[INFO] " RESET "Target IP: %s\n", target_ip);

		if (ft_strcmp(ip_source, target_ip) == 0 && ft_strcmp(ip_target, sender_ip) == 0)
		{
			printf(MAGENTA "[ARP MATCH] " RESET "IP source match the IP sender request : %s!\n\n", sender_ip);
			// ARP spoofing method
			send_arp_reply(fd, ip_source, mac_source, ip_target, mac_target);
		}
		else
		{
			printf(MAGENTA "[ARP DOESN'T MATCH] " RESET "IP source does not match the IP sender request : %s!\n", sender_ip);
		}
	}
}