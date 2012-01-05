#include "protocol_rtp.h"

protocol_rtp::protocol_rtp(u_int16_t sport, u_int16_t dport,statistic_ *stat) : protocolHandler_(sport, dport, stat), rtpFile("RTP2250.log")
{
    seqLoss = 0;
    frameSize = 0;
    estimatedLoss = 0;
    estimatedQuality = 5;
    qualitySum = 0;
    qualityCount = 0;
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

        // Check for RTP packet is MPEG2 stream
        if (rtpH->payload != 32)
            return 0;

        RFC2250H *rfc2250_inst = (RFC2250H *)(payload + sizeof(RTPFixedHeader));

        // Loss detection
        if (seqPrev != 0)
            lossDetection(rtpH, rfc2250_inst);

        // Calculate Number of P and B frame in GOP
        if(!GOP_eval.valid)
            calculateGOP(rtpH, rfc2250_inst);

        // Quality estimation
        qualityEstimation(rtpH, rfc2250_inst);

        // Calculate size of each frame (single or fragmented)
        frameSize += payload_length - sizeof(udphdr) - sizeof(RTPFixedHeader) - sizeof(RFC2250H);
        if (rfc2250_inst->end_of_slice)         // Important section will call methods at the last packet of segmented frame
        {
            motionEstimation(rtpH, rfc2250_inst);

            process(rfc2250_inst->picture_type, rfc2250_inst->temporal_refrence);
        }


        seqPrev = ntohs(rtpH->sequence);
    }
}
//-------------------------------------------------------------------
int
protocol_rtp::process(__uint16_t fType, __uint16_t tempRef)
{
    QTextStream rtpOut(&rtpFile);
    QString tmp =  (fType==1? "I ":(fType==2?"P ":"B "));
            tmp += QString::number(tempRef) + " ";
            tmp += QString::number(frameSize) + " ";

            tmp += QString::number(seqLoss) + " ";
            tmp += QString::number(estimatedLoss) + " ";
            tmp += QString::number(estimatedMotion) + " ";
            tmp += QString::number(estimatedQuality) + " ";
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
                        statistic->setCounter(GOP_SEQ, GOP_eval.seqTimes);
                        statistic->setCounter(GOP_P, GOP_eval.Ptimes);
                        statistic->setCounter(GOP_B, GOP_eval.Btimes);
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
protocol_rtp::lossDetection(RTPFixedHeader *rtp, RFC2250H *rfc)
{
        lossOccur = (ntohs(rtp->sequence) - seqPrev == 1)?false:true;
        if (GOP_eval.valid)
        {
            if (lossOccur && (ntohs(rtp->sequence) - seqPrev) > 1 )
            {
                seqLoss = ntohs(rtp->sequence) - seqPrev;
                statistic->setCounter(LOSS_SEQ, seqLoss);
                statistic->setCounter(ESTIMATED_LOSS, estimatedLoss);
            }else
                seqLoss = 0;
//                estimatedLoss = ((double)seqLoss/GOP_eval.seqTimes);
                estimatedLoss = log10((double)seqLoss/GOP_eval.seqTimes) + 2;
//                estimatedLoss = ((double)seqLoss/(GOP_eval.seqTimes * 3 ));
//                estimatedLoss = estimatedLoss + pow( ((double)seqLoss/(GOP_eval.seqTimes)), 1.5);
//                estimatedLoss = (estimatedLoss + pow( ((double)seqLoss/(GOP_eval.seqTimes)), 1.5))/2;
        }

}
//-------------------------------------------------------------------
void
protocol_rtp::qualityEstimation(RTPFixedHeader *rtp, RFC2250H *rfc)
{

//    double quality = tan(estimatedMotion) * estimatedLoss;
    ++qualityCount;
    double quality = tan(estimatedMotion) * estimatedLoss * 5;
//    double quality = 5.0 - tan(estimatedMotion) * estimatedLoss * 5;
    qualitySum += quality;
    estimatedQuality = qualitySum / qualityCount;
//    estimatedQuality = (estimatedQuality + quality) / 2;
//    estimatedQuality = (estimatedQuality + abs(estimatedQuality - quality)) / 2;
    statistic->setCounter(ESTIMATED_QUALITY, quality);
    statistic->setCounter(ESTIMATED_AVERAGE_QUALITY, estimatedQuality);
}
//-------------------------------------------------------------------
void
protocol_rtp::motionEstimation(RTPFixedHeader *rtp, RFC2250H *rfc)
{
    if (lossOccur)
        latestIframeSize = 0;
    else if (rfc->picture_type == I)
        latestIframeSize = frameSize;
    else if (latestIframeSize > frameSize && rfc->picture_type == P)
        estimatedMotion = ( estimatedMotion + ((double)frameSize / latestIframeSize))/2;

    statistic->setCounter(ESTIMATED_MOTION, estimatedMotion*100);
}
//-------------------------------------------------------------------
