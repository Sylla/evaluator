#include "packetmanagement.h"
#include "protocol_rtsp.h"
#include "protocol_icmp.h"

#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>


packetManagement_::packetManagement_(const char *dev_name, statistic_ *stat) : capture(dev_name, stat)
{
    statistic = stat;

    protocolHandler.insert(new protocol_rtsp(8090, 8090, &protocolHandler, statistic));
    protocolHandler.insert(new protocol_icmp(statistic));

    start();
}
//-----------------------------------------------------------------------------
packetManagement_::~packetManagement_()
{
    protocolHandler_ *pthandler;
    protocolHandlerIterator = protocolHandler.begin();
    while(protocolHandler.isEmpty())
    {
        pthandler = *protocolHandlerIterator;
        delete pthandler;
    }
    protocolHandler.clear();
}
//-----------------------------------------------------------------------------
void
packetManagement_::run()
{
    pcap_pkthdr header;
    const u_char *packet=NULL;

    header.len =0;
    while(true)
    {
        statistic->setCounter(CNT_PROTOCOL, protocolHandler.size());
        packet = capture.getPacket(&header);
        if (packet)
        {
            packetProcess(&header, packet);
            delete [] packet;
            packet = NULL;
        }
    }

}
//-----------------------------------------------------------------------------
bool
packetManagement_::callInstances(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length)
{
    for (protocolHandlerIterator = protocolHandler.begin();
         protocolHandlerIterator != protocolHandler.end();
         ++protocolHandlerIterator)
    {
        (*protocolHandlerIterator)->check(protocol, sport, dport, payload, payload_length);
    }
    return true;
}
//-----------------------------------------------------------------------------
bool
packetManagement_::packetProcess(pcap_pkthdr *header_, const u_char *packet_)
{
    const u_char *payload = NULL;
    unsigned int payload_length = 0;
    ether_header *ethernethdr = (ether_header *)packet_;

    if (!(ntohs(ethernethdr->ether_type) == ETHERTYPE_IP))
        return false;

    iphdr *ip_header = (iphdr *)(packet_ + sizeof(struct ether_header));

//print_packet(header_, packet_);

    if (ip_header->protocol == IPPROTO_TCP)
    {
        statistic->incrementCounter(CNT_TCP);
        tcphdr *tcp_header = (tcphdr *)((uchar *)ip_header + (ip_header->ihl << 2));

        payload_length = header_->len - ETHER_HDR_LEN - (ip_header->ihl << 2) - (tcp_header->doff << 2) ;
        if (payload_length != 0)
        {
            payload = (const u_char *)tcp_header + (tcp_header->doff << 2) ;
            return callInstances(ip_header->protocol , ntohs(tcp_header->source), ntohs(tcp_header->dest), payload, payload_length);
        }
    }
    else if (ip_header->protocol == IPPROTO_UDP)
    {
        statistic->incrementCounter(CNT_UDP);
        udphdr *udp_header = (udphdr *) ((uchar *)ip_header + (ip_header->ihl << 2));

        payload_length = header_->len - ETHER_HDR_LEN - (ip_header->ihl << 2) - sizeof(struct udphdr);
        if (payload_length != 0)
        {
            payload = (const u_char *)udp_header + sizeof(struct udphdr);
            return callInstances(ip_header->protocol, ntohs(udp_header->source), ntohs(udp_header->dest), payload, payload_length);
        }
    } else if (ip_header->protocol == IPPROTO_ICMP)
    {
        return callInstances(ip_header->protocol, 0, 0, payload, payload_length);
    }
    return false;
}
//-----------------------------------------------------------------------------
