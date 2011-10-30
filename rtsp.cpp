#include "rtsp.h"

RTSP::RTSP(statistic_ *stat): protocolHandler_(stat)
{
}
//-----------------------------------------------------------------------------
int
RTSP::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport)
{
//    std::cout << "-------------->" << protocol << "-" << sport << "-" << dport << std::endl;
    if( protocol == IPPROTO_TCP )
    {
//        std::cout << "---------------_____>" << sport << std::endl;
        statistic->incrementCounter(CNT_RTSP);
        process();
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
int
RTSP::process()
{
    // Reply: Transport: client_port=XXXXX-XXXXX;server_port=XXXXX-XXXXX; Session
    // Create new session
    statistic->messageOut(VERBOSE_MODE,  __func__ , "i`ve got RTSP packet :))" );
}
//-----------------------------------------------------------------------------
