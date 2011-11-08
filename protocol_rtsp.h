#ifndef RTSP_H
#define RTSP_H

#include "protocolhandler.h"
#include <QtCore>

struct ports__
{
    unsigned int rtp_client;
    unsigned int rtp_server;
    unsigned int rtcp_client;
    unsigned int rtcp_server;
    QString      session;
    QString      type;
};
//-------------------------------------------------------------------
class protocol_rtsp : public protocolHandler_
{
public:
        protocol_rtsp(u_int16_t sport, u_int16_t dport, QSet<protocolHandler_ *> *protocolHandler__, statistic_ *stat);
        ~protocol_rtsp();
    int check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
    int process(const QString qstr, const char del, const char desc);

private:
    QSet<protocolHandler_ *>                    *protocolHandler;
    QMultiHash<QString, protocolHandler_ *>     sessionHandler;
    ports__     ports;
};

#endif // RTSP_H
