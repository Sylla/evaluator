#ifndef PACKETMANAGEMENT_H
#define PACKETMANAGEMENT_H

#include "pcap_class.h"
#include "protocolhandler.h"
#include <iomanip>


class packetManagement_ : public QThread
{
public:
    packetManagement_(const char *dev_name, statistic_ *stat);
    ~packetManagement_();
    void run();

    pcap_class capture;
private:
    QSet<protocolHandler_ *> protocolHandler;
    QSet<protocolHandler_ *>::iterator protocolHandlerIterator;
    statistic_ *statistic;
};

#endif // PACKETMANAGEMENT_H
