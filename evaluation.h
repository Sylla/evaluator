#ifndef EVALUATION_H
#define EVALUATION_H

#include "packetmanagement.h"
#include "statistic.h"
#include "textui.h"


class evaluation
{
public:
    evaluation(const char *netdev);
    ~evaluation();

private:
    packetManagement_ packetManagement;
    statistic_ statistic;
    TextUI_ TextUI;
};

#endif // EVALUATION_H
