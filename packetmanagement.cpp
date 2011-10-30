#include "packetmanagement.h"
#include "protocol_rtsp.h"
#include "protocol_icmp.h"

packetManagement_::packetManagement_(const char *dev_name, statistic_ *stat) : capture(dev_name, stat)
{
    statistic = stat;

    protocolHandler.insert(new protocol_rtsp(statistic));
    protocolHandler.insert(new protocol_icmp(statistic));

    start();
}
//-----------------------------------------------------------------------------
packetManagement_::~packetManagement_()
{
    protocolHandler_ *pthandler;
    protocolHandlerIterator = protocolHandler.begin();
    while(protocolHandler.isEmpty())
    {
        pthandler = *protocolHandlerIterator;
        delete pthandler;
    }
}
//-----------------------------------------------------------------------------
void
packetManagement_::run()
{
    protocolHandler_ *pthandler;
    pcap_pkthdr header;
    const u_char *packet=NULL;

    header.len =0;
    while(true)
    {
        packet = capture.getPacket(&header);
        if (packet)
        {
//            std::cout << __func__<<":: ghagholi: "<< protocolHandler.size() <<std::endl;
//            std::cout << __func__<<":: PacketID : "<< std::hex << ntohs(*(packet + 12)) <<std::endl;

            for (protocolHandlerIterator = protocolHandler.begin();
                 protocolHandlerIterator != protocolHandler.end();
                 ++protocolHandlerIterator)
            {
                pthandler = *protocolHandlerIterator;
                pthandler->packetInjection(&header, packet);
            }
//            std::cout<<"------------------------------------"<<std::endl;
//            std::cout << __func__<<":: PACKET code: 0x"<< std::hex << ntohs(*(packet + 23)) / 0xFF << std::dec << std::endl;
//            std::cout << __func__ << ":: size "<< header.len << std::endl;
            delete [] packet;
        }
    }

}
//-----------------------------------------------------------------------------
