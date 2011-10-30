#include "protocol_rtsp.h"

protocol_rtsp::protocol_rtsp(statistic_ *stat) : protocolHandler_(stat)
{
}
//-----------------------------------------------------------------------------
int
protocol_rtsp::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport)
{
//    std::cout << "-------------->" << protocol << "-" << sport << "-" << dport << std::endl;
    if( protocol == IPPROTO_TCP && (sport == 8090 || dport == 8090) )
    {
//        std::cout << "---------------_____>" << sport << std::endl;
        statistic->incrementCounter(CNT_RTSP);
//        process();
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
int
protocol_rtsp::process()
{
    // Reply: Transport: client_port=XXXXX-XXXXX;server_port=XXXXX-XXXXX; Session
    // Create new session
//    statistic->messageOut(VERBOSE_MODE,  __func__ , "i`ve got RTSP packet :))" );
}
//-----------------------------------------------------------------------------
