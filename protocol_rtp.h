#ifndef PROTOCOL_RTP_H
#define PROTOCOL_RTP_H
#include "protocolhandler.h"
#include <QFile>

struct RFC2250H
{
 u_int16_t temporal_reference;
 u_char    picture_coding_type;
 u_char    vector_code_bits; // FBV,BFC,FFV,FFC from RFC 2250, sec. 3.4
// u_int8_t MBZ:5;
// u_int8_t T:1;
// u_int16_t TemporalRefrence:10;
// u_int8_t AN:1;
// u_int8_t NewPictureHeader:1;
// u_int8_t SequenceHeader:1;
// bool     BeginningofSlice:1;
// bool     EndofSlice:1;
// u_int8_t PictureType:3;
// u_int8_t FBV:1;
// u_int8_t BFC:3;
// u_int8_t FFV:1;
// u_int8_t FFC:3;
};

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
