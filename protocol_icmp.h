#ifndef PROTOCOL_ICMP_H
#define PROTOCOL_ICMP_H

#include "protocolhandler.h"
#include <netinet/ip.h>

class protocol_icmp : public protocolHandler_
{
public:
    protocol_icmp(statistic_ *stat);
    int check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    int process();

};

#endif // PROTOCOL_ICMP_H
