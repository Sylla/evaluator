#ifndef PACKETMANAGEMENT_H
#define PACKETMANAGEMENT_H

#include "pcap_class.h"
#include "protocolhandler.h"
#include <iomanip>


class packetManagement_ : public QThread
{
public:
            packetManagement_(const char *dev_name, const char *file_name, statistic_ *stat);
            ~packetManagement_();
    void    run();

private:
    QSet<protocolHandler_ *>            protocolHandler;
    QSet<protocolHandler_ *>::iterator  protocolHandlerIterator;
    statistic_                          *statistic;
    pcap_class                          capture;

    bool    packetProcess(pcap_pkthdr *header, const u_char *packet_);
    bool    callInstances(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length);
};

#endif // PACKETMANAGEMENT_H
