#include "evaluation.h"
#include <stdlib.h>

evaluation::evaluation(int argc, char *argv[])
{
    int c;
    char optv[] = "f:n:t::";

    bool TextUIenable = true;
    while ( (c = getopt(argc, argv, optv)) != -1)
    {
        switch(c)
        {
        case 'f':
            packetManagement = new packetManagement_(NULL, optarg, &statistic);
            break;
        case 'n':
            packetManagement = new packetManagement_(optarg, NULL, &statistic);
            break;
        case 't':
            TextUIenable = false;
            break;
        case'?':
            if (optopt == 'c')
                std::cerr<<"Option requires an argument."<<std::endl;
            else if (isprint (optopt))
                std::cerr<<"Unknown option "<<optopt<<std::endl;
            else
                std::cerr<<"Unknown option character"<<optopt<<std::endl;

            std::cout<<"Syntax: evaluator <Options> " <<std::endl;
            std::cout<<"Options:" <<std::endl;
            std::cout<<"-f <filename.pcap> : read pcap file (rate 100 pps)" <<std::endl;
            std::cout<<"-n <network device> : capture packet from network device" <<std::endl;
            std::cout<<"-t : disable text user interface" <<std::endl;
            exit(0);
            break;
        default:
            std::cout<<"Error in input arguments."<<std::endl;
            exit(1);
        }
    }
    if (TextUIenable)
        TextUI = new TextUI_(&statistic);
}

evaluation::~evaluation()
{
    if (TextUI!=NULL)
        delete TextUI;
    delete packetManagement;
}
