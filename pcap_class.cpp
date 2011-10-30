#include "pcap_class.h"


//-----------------------------------------------------------------------------
pcap_class::pcap_class(const char *dev_name, statistic_ *stat)
{
    //try catch shod be implement
    statistic = stat;
    handle = pcap_open_live(dev_name, BUFSIZ, 1, -1, errbuf);
    if (handle == NULL)
    {
        std::cerr << "Could not open device " << dev_name << " !!!" << std::endl;
    } else
    {
        std::cout<<"Device "<< dev_name <<" has been opened." << std::endl;
    }
    start();
}
//-----------------------------------------------------------------------------
pcap_class::~pcap_class()
{
    pcap_breakloop(handle);
    pcap_close(handle);
    std::cerr << "pcap_class has finished." << std::endl;
}
//-----------------------------------------------------------------------------
void
pcap_class::run()
{
////    pcap_loop(handle, -1, pcap_class::pcap_packet_received, NULL);

    pcap_packet_ pcap_packet;

    while(true)
    {
        pcap_packet.packet = pcap_next(handle, &pcap_packet.header);
        if (pcap_packet.packet != NULL)
        {
            mutex.lock();
            packetQueue.enqueue(pcap_packet);
            statistic->incrementCounter(CNT_TOTAL);
            mutex.unlock();
        }
    }
}
//-----------------------------------------------------------------------------
const u_char *
pcap_class::getPacket(const pcap_pkthdr *header)
{
    pcap_packet_ pcap_packet;
    const u_char *packet=NULL;

    statistic->setCounter(CNT_QUEUE, packetQueue.size());

    if (packetQueue.isEmpty())
        return packet;

    mutex.lock();
    pcap_packet = packetQueue.dequeue();
    mutex.unlock();

    packet = new u_char[pcap_packet.header.len];
    memcpy((void *)packet, (const void *)pcap_packet.packet, pcap_packet.header.len);
    memcpy((void *)header, (const void *)&(pcap_packet.header), sizeof(pcap_pkthdr));

//        std::cout << __func__<<":: DEBUG: "<< pcap_packet.header.len <<std::endl;
//        std::cout << __func__<<":: DEBUG: "<< pcap_packet.header.ts.tv_sec <<std::endl;
//        std::cout << __func__<<":: PacketID : "<< std::hex << ntohs(*((pcap_packet.packet) + 12)) <<std::endl;
//        std::cout << __func__<<":: PacketID : "<< std::hex << ntohs(*(packet + 12)) <<std::endl;

    return packet;
}
//-----------------------------------------------------------------------------
//void
//pcap_class::pcap_packet_received(u_char *args, const pcap_pkthdr *header, const u_char *packet)
//{
//    std::cout << "packet received, but nothing occured..."<<std::endl;
//    packet_received(args, header, packet);
//}
