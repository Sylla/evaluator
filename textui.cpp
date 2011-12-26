#include "textui.h"

TextUI_::TextUI_(statistic_ *stat)
{
    statistic = stat;
    start();
}

//-----------------------------------------------------------------------------

void TextUI_::run()
{
    while(true)
    {
        usleep(50000);
        system("clear");
        std::cout << "Video Quality Evaluation Program"<<std::endl;
        std::cout << "Amir Hossein Mandegar<amandegar@gmail.com>"<<std::endl;
        std::cout << "-------------------------------------------------------------------------------------"<<std::endl;
        std::cout << " total captured packet(EQ): " << statistic->getCounter(CNT_TOTAL_EQ)<<std::endl;
        std::cout << " total captured packet(DQ): " << statistic->getCounter(CNT_TOTAL_DQ)<<std::endl;
        std::cout << " total ICMP  packet: " << statistic->getCounter(CNT_ICMP)<<std::endl;
        std::cout << " total UDP  packet: " << statistic->getCounter(CNT_UDP)<<std::endl;
        std::cout << " total TCP  packet: " << statistic->getCounter(CNT_TCP)<<std::endl;
        std::cout << " total RTSP packet: " << statistic->getCounter(CNT_RTSP)<<std::endl;
        std::cout << " total RTP  packet: " << statistic->getCounter(CNT_RTP)<<std::endl;
        std::cout << " total RTCP packet: " << statistic->getCounter(CNT_RTCP)<<std::endl;
        std::cout << "----------------------------------------"<<std::endl;
        std::cout << " Registered Protocol instance: " << statistic->getCounter(CNT_PROTOCOL)<<std::endl;
        std::cout << " Registered Sessions by RTSP: " << statistic->getCounter(CNT_SESSION)<<std::endl;
        std::cout << " Queue length: " << statistic->getCounter(CNT_QUEUE_SIZE)<<std::endl;
        std::cout << "----------------------------------------"<<std::endl;
        std::cout << " GOP Sequence: " << statistic->getCounter(GOP_SEQ)<<std::endl;
        std::cout << " GOP P frame times: " << statistic->getCounter(GOP_P)<<std::endl;
        std::cout << " GOP B frame times: " << statistic->getCounter(GOP_B)<<std::endl;
        std::cout << "----------------------------------------"<<std::endl;
        std::cout << " Loss Sequence(latest): " << statistic->getCounter(LOSS_SEQ)<<std::endl;
        std::cout << " Estimated Loss: " << statistic->getCounter(ESTIMATED_LOSS)<<std::endl;
        std::cout << " Estimate Motion: " << statistic->getCounter(ESTIMATED_MOTION)<<std::endl;
        std::cout << " Estimated Quality: " << statistic->getCounter(ESTIMATED_QUALITY)<<std::endl;
        std::cout << " Estimated Average Quality: " << statistic->getCounter(ESTIMATED_AVERAGE_QUALITY)<<std::endl;
    }
}
//-----------------------------------------------------------------------------
