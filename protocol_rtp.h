#ifndef PROTOCOL_RTP_H
#define PROTOCOL_RTP_H
#include "protocolhandler.h"
#include "rfc2250.h"
#include <ccrtp/rtppkt.h>
#include <QFile>

class protocol_rtp : public protocolHandler_
{
public:
            protocol_rtp(u_int16_t sport, u_int16_t dport,statistic_ *stat);
            ~protocol_rtp();
    int     check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    int     process(__uint16_t fType, __uint16_t tempRef);
private:
    QFile       rtpFile;
    __uint32_t  frameSize;
};

#endif // PROTOCOL_RTP_H
