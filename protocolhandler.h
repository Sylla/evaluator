#ifndef protocolHandler_H
#define protocolHandler_H

#include <pcap.h>
#include "statistic.h"

class protocolHandler_
{
public:
                    protocolHandler_(statistic_ *stat);
    int             packetInjection(pcap_pkthdr *header, const u_char *packet_);
    virtual int     check(u_int16_t protocol, u_int16_t sport, u_int16_t dport)=0;
    virtual int     process()=0;

protected:
    const u_char    *packet;
    pcap_pkthdr     *header;
    const u_char    *payload;
    statistic_      *statistic;
};

#endif // protocolHandler_H
