#include "protocol_rtp.h"

protocol_rtp::protocol_rtp(u_int16_t sport, u_int16_t dport,statistic_ *stat) : protocolHandler_(sport, dport, stat), rtpFile("RTP2250.log")
{
    frameSize = 0;
    rtpFile.open(QIODevice::WriteOnly | QIODevice::Text);
}
//-------------------------------------------------------------------
protocol_rtp::~protocol_rtp()
{
    rtpFile.close();
}
//-------------------------------------------------------------------
int
protocol_rtp::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length)
{
    if( protocol == IPPROTO_UDP &&
       (sport == source_port && dport == destination_port) ||
       (dport == source_port && sport == destination_port))
    {
        RTPFixedHeader *rtpH = (RTPFixedHeader *)payload;
        RFC2250H *rfc2250_h = (RFC2250H *)(payload + sizeof(RTPFixedHeader));
        statistic->incrementCounter(CNT_RTP);
        std::cout<<ntohs(rtpH->sequence)<<"-"<<
                   ntohl(rtpH->timestamp)<<"-"<<
                   (int)rtpH->payload<<"-"<<
                   std::endl;
        std::cout<<ntohs(rtpH->sequence)<<"-"<<
                   ntohl(rtpH->timestamp)<<"-"<<
                   (int)rtpH->payload<<"-"<<
                   std::endl;

        frameSize += payload_length - 8 - 12 - 4; // UDP, RTP and rfc2250 Header Length
        if (rfc2250_h->end_of_slice)
            process(rfc2250_h->picture_type, rfc2250_h->temporal_refrence);
    }
}
//-------------------------------------------------------------------
int
protocol_rtp::process(__uint16_t fType, __uint16_t tempRef)
{
//    std::cout<<"--->"<< (fType==1? "I":(fType==2?"P":"B"))
//             <<" / " << tempRef
//             <<" / " << frameSize
//             <<std::endl;

    QTextStream rtpOut(&rtpFile);
    QString tmp =  (fType==1? "I ":(fType==2?"P ":"B "));
            tmp += QString::number(tempRef) + " ";
            tmp += QString::number(frameSize) + " ";
    rtpOut << tmp + "\n";
    rtpOut.flush();
    frameSize = 0;
}
//-------------------------------------------------------------------
