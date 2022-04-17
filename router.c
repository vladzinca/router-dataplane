#include "queue.h"
#include "skel.h"

struct route_table_entry *rtable;
int rtable_len;

struct arp_entry *arptable;
int arptable_len;

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

struct arp_entry* get_best_entry(struct in_addr dest_ip)
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

	rtable = malloc(sizeof(struct route_table_entry) * 100000);
	DIE(rtable == NULL, "memory");

	arptable = malloc(sizeof(struct arp_entry) * 6);
	DIE(arptable == NULL, "memory");

	rtable_len = read_rtable(argv[1], rtable);
	arptable_len = parse_arp_table("arp_table.txt", arptable);

	while (1)
	{
		rc = get_packet(&m);
		DIE(rc < 0, "get_packet");
		/* TODO */
		struct ether_header* ethernet_header = (struct ether_header*)m.payload;

		if (ntohs(ethernet_header->ether_type) == ETHERTYPE_IP)
		{
			struct iphdr* ip_header =
					(struct iphdr*)(m.payload + sizeof(struct ether_header));

			struct in_addr dest_aux;
			dest_aux.s_addr = ip_header->daddr;

			if (ip_checksum((uint8_t*)ip_header, sizeof(struct iphdr)) == 0)
			{
				if (ip_header->ttl >= 1)
				{
					struct route_table_entry* best_route =
													get_best_route(dest_aux);
					struct arp_entry* best_entry = get_best_entry(dest_aux);

					ip_header->ttl--;
					ip_header->check = 0;
					ip_header->check =
						ip_checksum((uint8_t*)ip_header, sizeof(struct iphdr));

					memcpy(ethernet_header->ether_dhost,
						   best_entry->mac, sizeof(best_entry->mac));
					m.interface = best_route->interface;
					send_packet(&m);
				}
			}
		}
	}
}
