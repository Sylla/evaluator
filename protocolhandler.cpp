#include "protocolhandler.h"

protocolHandler_::protocolHandler_(u_int16_t sport, u_int16_t dport, statistic_ *stat)
{
    source_port     = sport;
    destination_port= dport;
    statistic       = stat;
}
//-----------------------------------------------------------------------------
int
protocolHandler_::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length)
{
    return false;
}
//-----------------------------------------------------------------------------
int
protocolHandler_::process()
{
    return false;
}
