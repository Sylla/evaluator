#ifndef TEXTUI_H
#define TEXTUI_H

#include <iostream>
#include <QThread>
#include "statistic.h"

class TextUI_ : public QThread
{
public:
    TextUI_(statistic_ *stat);
    void run();
private:
    statistic_ *statistic;
};

#endif // TEXTUI_H
