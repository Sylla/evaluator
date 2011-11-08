#include "protocol_rtcp.h"

protocol_rtcp::protocol_rtcp(u_int16_t sport, u_int16_t dport, statistic_ *stat) : protocolHandler_(sport, dport, stat)
{
}
//-------------------------------------------------------------------
int
protocol_rtcp::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length)
{
    if( protocol == IPPROTO_UDP &&
       (sport == source_port && dport == destination_port) ||
       (dport == source_port && sport == destination_port))
    {
        statistic->incrementCounter(CNT_RTCP);
    }
}
//-------------------------------------------------------------------
int
protocol_rtcp::process()
{

}
//-------------------------------------------------------------------
