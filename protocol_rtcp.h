#ifndef PROTOCOL_RTCP_H
#define PROTOCOL_RTCP_H
#include "protocolhandler.h"


class protocol_rtcp : public protocolHandler_
{
public:
            protocol_rtcp(u_int16_t sport, u_int16_t dport, statistic_ *stat);
    int     check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    int     process();
};

#endif // PROTOCOL_RTCP_H
