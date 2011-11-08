#ifndef protocolHandler_H
#define protocolHandler_H

#include <pcap.h>
#include <netinet/ip.h>
#include "statistic.h"

class protocolHandler_
{
public:
                    protocolHandler_(u_int16_t sport, u_int16_t dport, statistic_ *stat);
    virtual int     check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    virtual int     process();

protected:
    statistic_      *statistic;
    u_int16_t       source_port;
    u_int16_t       destination_port;
};

#endif // protocolHandler_H
