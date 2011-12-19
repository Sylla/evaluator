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
        statistic->incrementCounter(CNT_RTP);
        RTPFixedHeader *rtpH = (RTPFixedHeader *)payload;

        if (rtpH->payload != 32)
            return 0;

        RFC2250H *rfc2250_inst = (RFC2250H *)(payload + sizeof(RTPFixedHeader));

        std::cout<<ntohs(rtpH->sequence)<<"-"<<seqPrev<<std::endl;

        if (seqPrev != 0)
        {
            lossOccur = (ntohs(rtpH->sequence) - seqPrev > 1)?true:false;
            seqLoss   = (lossOccur)?seqPrev:seqLoss;
        }

        // Calculate Number of P and B frame in GOP
        if(!GOP_eval.valid)
            calculateGOP(rtpH, rfc2250_inst);
        else if(lossOccur)
            processLoss(rtpH, rfc2250_inst);

        frameSize += payload_length - sizeof(udphdr) - sizeof(RTPFixedHeader) - sizeof(RFC2250H);
        if (rfc2250_inst->end_of_slice)
            process(rfc2250_inst->picture_type, rfc2250_inst->temporal_refrence);


        seqPrev = ntohs(rtpH->sequence);
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
void
protocol_rtp::calculateGOP(RTPFixedHeader *rtp, RFC2250H *rfc)
{
    if (!GOP_eval.valid && !lossOccur)
        if(GOP_eval.start)
        {
                switch(rfc->picture_type)
                {
                case I:
                    if(GOP_eval.Ptimes !=0)
                    {
                        GOP_eval.valid = true;
                        GOP_eval.start = false;
                    }
                    break;
                case P:
                    (rfc->end_of_slice)? ++GOP_eval.Ptimes:0;
                    break;
                case B:
                    (rfc->end_of_slice)? ++GOP_eval.Btimes:0;
                    break;
                }
                (GOP_eval.start)?++GOP_eval.seqTimes:0;
        }else
             GOP_eval.start = (rfc->picture_type == I)? true: false;
}
//-------------------------------------------------------------------
void
protocol_rtp::processLoss(RTPFixedHeader *rtp, RFC2250H *rfc)
{
    std::cout<<"ready for calculate loss effect";
}
//-------------------------------------------------------------------
