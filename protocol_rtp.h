#ifndef PROTOCOL_RTP_H
#define PROTOCOL_RTP_H
#include "protocolhandler.h"


class protocol_rtp : public protocolHandler_
{
public:
            protocol_rtp(u_int16_t sport, u_int16_t dport,statistic_ *stat);
    int     check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    int     process();
};

#endif // PROTOCOL_RTP_H
