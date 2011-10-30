#include "evaluation.h"

evaluation::evaluation(const char *netdev) : packetManagement(netdev, &statistic), TextUI(&statistic)
{
}

evaluation::~evaluation()
{

}
