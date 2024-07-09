#include "../header/ft_malcolm.h"

volatile bool STOP;

static unsigned char hex_char_to_byte(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    // convertie les lettres hexa en son equi decimal
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return 0;
}

static void convert_mac_address(const char *mac_str, unsigned char *mac)
{
    for (int i = 0; i < 6; ++i)
    {
        // convertir deux caractères hexadécimaux en un seul octet
        // premier chf=deplacement de 4bits vers la gauche -> 0001 devient 0001 0000
        // "|" combinaison avec le 2eme char ->0001 0000 OR 0010 donne 0001 0010
        mac[i] = (hex_char_to_byte(mac_str[i * 3]) << 4) | hex_char_to_byte(mac_str[i * 3 + 1]);
    }
}

static void convert_ip_address(const char *ip_str, unsigned char *ip)
{
    inet_pton(AF_INET, ip_str, ip);
}

// Envoyer une réponse ARP
void send_arp_reply(int fd, const char *ip_source, const char *mac_source, const char *ip_target, const char *mac_target)
{
    // 42=taille standard d'un paquet ARP (14 octets pour l'en-tête Ethernet et 28 octets pour le paquet ARP)
    unsigned char buffer[42];

    // struct ethernet pointe vers header
    struct ethhdr *eth = (struct ethhdr *)buffer;
    // struct arp pointe vers le paquet ARP juste après l'en-tête Ethernet
    struct ether_arp *arp = (struct ether_arp *)(buffer + sizeof(struct ethhdr));

    // conv des addr IP et MAC en binaire (mac=6octets/ip=4octets)
    unsigned char mac_source_bin[6], mac_target_bin[6], ip_source_bin[4], ip_target_bin[4];
    convert_mac_address(mac_source, mac_source_bin);
    convert_mac_address(mac_target, mac_target_bin);
    convert_ip_address(ip_source, ip_source_bin);
    convert_ip_address(ip_target, ip_target_bin);

    // assigne les mac addr bin dans le header de la res
    ft_memcpy(eth->h_source, mac_source_bin, 6);
    ft_memcpy(eth->h_dest, mac_target_bin, 6);
    eth->h_proto = htons(ETH_P_ARP);

    // Decla format "Ethernet" (ARP)
    arp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    // Decla format IPV4 - spécifie que le protocole utilisé est IP (Internet Protocol)
    arp->ea_hdr.ar_pro = htons(ETH_P_IP);
    // Decla long materielle à 6 octets
    arp->ea_hdr.ar_hln = ETHER_ADDR_LEN;
    // Decla long ip à 4 octets
    arp->ea_hdr.ar_pln = sizeof(in_addr_t);
    // Decla que c'est une res ARP
    arp->ea_hdr.ar_op = htons(ARPOP_REPLY);

    // "Sender Hardware Address"= addr mac src (Adresse matérielle de l'expéditeur)
    ft_memcpy(arp->arp_sha, mac_source_bin, 6);
    // "Sender Protocol Address"= addr ip src (Adresse du protocole de l'expéditeur)
    ft_memcpy(arp->arp_spa, ip_source_bin, 4);
    // "Target Hardware Address"= addr mac target (Adresse matérielle de la cible)
    ft_memcpy(arp->arp_tha, mac_target_bin, 6);
    ft_memcpy(arp->arp_tpa, ip_target_bin, 4);

    // sockaddr_ll=utilisée pour spécifier les paramètres
    // de la couche liaison de données lors de l'envoi de paquets bruts
    struct sockaddr_ll device;
    ft_memset(&device, 0, sizeof(device));
    // convertit le nom d'une interface réseau en son index numérique. - spécifie l'interface réseau donc
    device.sll_ifindex = if_nametoindex("eth0");
    // Decla long materielle à 6 octets
    device.sll_halen = ETH_ALEN;
    // sll_addr est un champ de la structure sockaddr_ll qui contient l'adresse matérielle (MAC) de destination
    ft_memcpy(device.sll_addr, mac_target_bin, 6);

    // Messages de débogage pour vérifier les informations du paquet ARP
    char ip_src_str[INET_ADDRSTRLEN], ip_tgt_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, ip_source_bin, ip_src_str, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, ip_target_bin, ip_tgt_str, INET_ADDRSTRLEN);

    printf("Sending ARP reply:\n");
    printf("Source IP: %s\n", ip_src_str);
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           mac_source_bin[0], mac_source_bin[1], mac_source_bin[2],
           mac_source_bin[3], mac_source_bin[4], mac_source_bin[5]);
    printf("Target IP: %s\n", ip_tgt_str);
    printf("Target MAC: %02x:%02x:%02x:%02x:%02x:%02x\n\n",
           mac_target_bin[0], mac_target_bin[1], mac_target_bin[2],
           mac_target_bin[3], mac_target_bin[4], mac_target_bin[5]);

    if (sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&device, sizeof(device)) <= 0)
    {
        printf(RED "[ERROR]" RESET " Send to function failed...\n");
    }
    else
    {
        printf(GREEN "[INFO]" RESET " Sent ARP reply packet.\n");
    }
    STOP = true;
}
