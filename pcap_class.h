#ifndef PCAP_CLASS_H
#define PCAP_CLASS_H

#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <QThread>
#include <QtCore>

#include "statistic.h"

struct pcap_packet_
{
    struct pcap_pkthdr header;
    const u_char *packet;
};

/**
 * @class pcap object oriented class for capture packets from network
 *
 */
class pcap_class : public QThread
{
public:
    /**
     * Constructor
     * @param dev_name network interface name
     */
    pcap_class(const char *dev_name, statistic_ *stat);
    ~pcap_class();
    /**
     * Run method is thread process capture loop
     */
    virtual void run();

    /**
     * Virtual method to call whenever new packet exist
     * @param header pcap header structure pointer
     * @param packet full packet data pointer
     */
    const u_char * getPacket(const struct pcap_pkthdr *header);

private:
//    static void pcap_packet_received(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

    QQueue<pcap_packet_> packetQueue;
    QMutex  mutex;
    pcap_t *handle;
    char *errbuf;
    statistic_ *statistic;

};

#endif // PCAP_CLASS_H
