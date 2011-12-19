#ifndef PROTOCOL_RTP_H
#define PROTOCOL_RTP_H
#include "protocolhandler.h"
#include "rfc2250.h"
#include <netinet/udp.h>
#include <QFile>

enum FT{I=1,P,B};
//-------------------------------------------------------------------
struct GOP_eval_struct{
    GOP_eval_struct()
    {
        start       = false;
        valid       = false;
        Ptimes      = 0;
        Btimes      = 0;
        seqTimes    = 0;
    }
    bool        start;
    bool        valid;
    u_int8_t    Ptimes;
    u_int8_t    Btimes;
    u_int8_t    seqTimes;
};
//-------------------------------------------------------------------
class protocol_rtp : public protocolHandler_
{
public:
            protocol_rtp(u_int16_t sport, u_int16_t dport,statistic_ *stat);
            ~protocol_rtp();
    int     check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    int     process(__uint16_t fType, __uint16_t tempRef);
    void    calculateGOP(RTPFixedHeader *rtp, RFC2250H *rfc);
    void    processLoss(RTPFixedHeader *rtp, RFC2250H *rfc);
private:
    QFile       rtpFile;
    __uint32_t  frameSize;
    u_int16_t   seqPrev, seqLoss;
    bool        lossOccur;
    GOP_eval_struct  GOP_eval;
};

#endif // PROTOCOL_RTP_H
