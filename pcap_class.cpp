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
    const u_char *packet_temp;

    while(true)
    {
        packet_temp = pcap_next(handle, &pcap_packet.header);
        if (packet_temp != NULL)
        {
            mutex.lock();
            pcap_packet.packet = new u_char[pcap_packet.header.len + 1];
            memcpy((void *)(pcap_packet.packet), (const void *)packet_temp, pcap_packet.header.len);
            memset((void *)(pcap_packet.packet + pcap_packet.header.len), NULL, 1);
            packetQueue.enqueue(pcap_packet);
            statistic->incrementCounter(CNT_TOTAL_EQ);
            mutex.unlock();
        }
    }
}
//-----------------------------------------------------------------------------
const u_char *
pcap_class::getPacket(const pcap_pkthdr *header)
{
    pcap_packet_ pcap_packet;

    statistic->setCounter(CNT_QUEUE_SIZE, packetQueue.size());

    if (packetQueue.isEmpty())
        return NULL;

    mutex.lock();
    pcap_packet = packetQueue.dequeue();
    statistic->incrementCounter(CNT_TOTAL_DQ);
    mutex.unlock();

    memcpy((void *)header, (const void *)&(pcap_packet.header), sizeof(pcap_pkthdr));

    return pcap_packet.packet;
}
//-----------------------------------------------------------------------------
//void
//pcap_class::pcap_packet_received(u_char *args, const pcap_pkthdr *header, const u_char *packet)
//{
//    std::cout << "packet received, but nothing occured..."<<std::endl;
//    packet_received(args, header, packet);
//}
//-----------------------------------------------------------------------------
void
pcap_class::print_packet(pcap_pkthdr *header_, const u_char *packet_)
{
    unsigned int i;

    std::cout << "Packet length: " << header_->len << std::endl;

    for(i=0; i < header_->len ; i++)
    {
        if (i%16 == 0) std::cout << std::endl;

//        std::cout.width(1);
//        std::cout.fill('0');
        std::cout << std::left << std::hex << " " << (int)packet_[i] << std::dec;
//        if (i% 8 == 0) std::cout << " - ";
    }

    std::cout << std::endl << "------------------------------------------------------"<< std::endl;
}
