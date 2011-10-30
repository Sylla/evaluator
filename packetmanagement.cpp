#include "packetmanagement.h"
#include "rtsp.h"

packetManagement_::packetManagement_(const char *dev_name, statistic_ *stat) : capture(dev_name, stat)
{
    statistic = stat;

    protocolHandler.insert(new RTSP(statistic));

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
//            std::cout << __func__<<":: ghagholi: "<< header.len <<std::endl;
//            std::cout << __func__<<":: PacketID : "<< std::hex << ntohs(*(packet + 12)) <<std::endl;

            for (protocolHandlerIterator = protocolHandler.begin();
                 protocolHandlerIterator != protocolHandler.end();
                 ++protocolHandlerIterator)
            {
//                statistic->messageOut(VERBOSE_MODE, __func__, "Got packet and sed to all analyzer...");
                pthandler = *protocolHandlerIterator;
                pthandler->packetInjection(&header, packet);
//                pthandler->check(1,1,1);
            }
//            std::cout<<"------------------------------------"<<std::endl;
//            std::cout << __func__<<":: PACKET code: 0x"<< std::hex << ntohs(*(packet + 23)) / 0xFF << std::dec << std::endl;
//            std::cout << __func__ << ":: size "<< header.len << std::endl;
            delete [] packet;
        }
    }

}
//-----------------------------------------------------------------------------
