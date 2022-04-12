#include "queue.h"
#include "skel.h"

int main(int argc, char *argv[])
{
	packet m;
	int rc;

	// Do not modify this line
	init(argc - 2, argv + 2);

	while (1)
	{
		rc = get_packet(&m);
		DIE(rc < 0, "get_packet");
		/* TODO */
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;
		if (ntohs(eth_hdr->ether_dhost) == ntohs(eth_hdr->ether_shost) || ntohs(eth_hdr->ether_dhost) == INADDR_BROADCAST)
		{
			if (ntohs(eth_hdr->ether_type) == ETHERTYPE_IP)
			{

			}
			else if (ntohs(eth_hdr->ether_type) == ETHERTYPE_ARP)
			{

			}
		}
	}
}
