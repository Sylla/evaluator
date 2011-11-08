#include "protocol_icmp.h"

protocol_icmp::protocol_icmp(statistic_ *stat) : protocolHandler_(0, 0, stat)
{
}
//-------------------------------------------------------------------
int
protocol_icmp::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length)
{
    if (protocol == IPPROTO_ICMP)
        statistic->incrementCounter(CNT_ICMP);
}
//-------------------------------------------------------------------
int
protocol_icmp::process()
{
}
//-------------------------------------------------------------------
