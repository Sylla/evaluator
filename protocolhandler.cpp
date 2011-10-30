#include "protocolhandler.h"
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <stdlib.h>

protocolHandler_::protocolHandler_(statistic_ *stat)
{
    statistic = stat;
}
//-----------------------------------------------------------------------------
int protocolHandler_::packetInjection(pcap_pkthdr *header_, const u_char *packet_)
{
    packet  = packet_;
    header  = header_;
    ether_header *ethernethdr = (ether_header *)packet;

    if (!(ntohs(ethernethdr->ether_type) == ETHERTYPE_IP))
        return false;

    iphdr *ip_header = (iphdr *)(packet + sizeof(struct ether_header));

//    statistic->messageOut(VERBOSE_MODE, __func__," check for packet ...");
//    std::cout << __func__<<":: PacketID : 0x"<< std::hex << ntohs(*((packet_) + 14)) <<std::endl;
//    std::cout << __func__<<":: PacketID hdr: 0x"<< std::hex << ntohs(ip_header->tot_len) <<std::endl;
//    std::cout << __func__<<":: PacketID hdr: 0x" << std::hex << (ntohs(ip_header->protocol) >> 8)  <<std::endl;
//    std::cout << __func__<<":: PacketID hdr:" << (ntohs(ip_header->ihl) >> 8 )* 4  <<std::endl;

    if ((ntohs(ip_header->protocol) >> 8)== IPPROTO_TCP)
    {
        statistic->incrementCounter(CNT_TCP);
        tcphdr *tcp_header = (tcphdr *)(packet + ETHER_HDR_LEN + (ntohs(ip_header->ihl) >> 8) * 4);
        payload = packet + ETHER_HDR_LEN + (ntohs(ip_header->ihl) >> 8) * 4 + sizeof(struct tcphdr);
        return check(ntohs(ip_header->protocol) >> 8, ntohs(tcp_header->source), ntohs(tcp_header->dest));
    }
    else if ((ntohs(ip_header->protocol)>> 8) == IPPROTO_UDP)
    {
        statistic->incrementCounter(CNT_UDP);
        udphdr *udp_header = (udphdr *) (packet + ETHER_HDR_LEN + (ntohs(ip_header->ihl) >> 8) * 4);
        payload = packet + ETHER_HDR_LEN + (ntohs(ip_header->ihl) >> 8) * 4 + sizeof(struct udphdr);
//        std::cout <<(ntohs(ip_header->protocol) >> 8 )<< "-"<< ntohs(udp_header->source)<<"-"<<(ntohs(ip_header->ihl) >> 8)<< "-"<< ntohs(udp_header->dest)<< std::endl;
        return check((ntohs(ip_header->protocol) >> 8 ) , ntohs(udp_header->source), ntohs(udp_header->dest));
    }
    return false;
}
//-----------------------------------------------------------------------------
