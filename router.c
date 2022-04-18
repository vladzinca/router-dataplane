#include "queue.h"
#include "skel.h"

struct route_table_entry *rtable;
int rtable_len;

struct arp_entry *arptable;
int arptable_len;

/* functie care cauta interfata corespunzatoare
	pe care sa trimita pachetul */
struct route_table_entry* get_best_route(struct in_addr dest_ip)
{
	int index = -1;
	for (int i = 0; i < rtable_len; i++)
	{
		if ((dest_ip.s_addr & rtable[i].mask) == rtable[i].prefix)
		{
			if (index == -1)
				index = i;
			else if (rtable[i].mask > rtable[index].mask)
				index = i;
		}
	}
	if (index == -1)
		return NULL;
	else
		return &rtable[index];
}

/* functie care cauta adresa MAC a urmatorului
	hop in tabela statica ARP */
struct arp_entry* get_arp_entry(struct in_addr dest_ip)
{
	int index = -1;
	for (int i = 0; i < arptable_len; i++)
		if (arptable[i].ip == dest_ip.s_addr)
			index = i;
	if (index == -1)
		return NULL;
	else
		return &arptable[index];
}

int main(int argc, char* argv[])
{
	packet m;
	int rc;

	// Do not modify this line
	init(argc - 2, argv + 2);

	/* se aloca memorie pentru tabela de rutare
		si pentru tabela statica ARP */
	rtable = malloc(sizeof(struct route_table_entry) * 100000);
	DIE(rtable == NULL, "memory");

	arptable = malloc(sizeof(struct arp_entry) * 6);
	DIE(arptable == NULL, "memory");

	/* parsare pentru tabela de rutare
		si pentru tabela statica ARP */
	rtable_len = read_rtable(argv[1], rtable);
	arptable_len = parse_arp_table("arp_table.txt", arptable);

	while (1)
	{
		rc = get_packet(&m);
		DIE(rc < 0, "get_packet");
		/* TODO */

		/* extragere header ethernet */
		struct ether_header* ethernet_header = (struct ether_header*)m.payload;

		/* inspectare ca urmatorul header sa fie IPv4 */
		if (ntohs(ethernet_header->ether_type) == ETHERTYPE_IP)
		{
			/* extragere header ip */
			struct iphdr* ip_header =
					(struct iphdr*)(m.payload + sizeof(struct ether_header));

			/* verificare checksum */
			if (ip_checksum((uint8_t*)ip_header, sizeof(struct iphdr)) == 0)
			{
				/* verificare ttl */
				if (ip_header->ttl >= 1)
				{
					/* actualizare ttl */
					ip_header->ttl--;

					/* cautare in tabela de rutare */
					struct in_addr dest_aux;
					dest_aux.s_addr = ip_header->daddr;
					struct route_table_entry* best_route =
													get_best_route(dest_aux);

					/* actualizare checksum */
					ip_header->check = 0;
					ip_header->check =
						ip_checksum((uint8_t*)ip_header, sizeof(struct iphdr));

					/* rescriere adrese L2 */
					struct arp_entry* arp_entry = get_arp_entry(dest_aux);
					memcpy(ethernet_header->ether_dhost,
						   arp_entry->mac, sizeof(arp_entry->mac));

					/* trimiterea noului pachet pe interfata corespunzatoare */
					m.interface = best_route->interface;
					send_packet(&m);
				}
			}
		}
	}
}
