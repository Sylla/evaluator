#ifndef STATISTIC_H
#define STATISTIC_H

#include <QtCore>
#include <iostream>

enum modes{SILENT_MODE, VERBOSE_MODE, DEBUG_MODE, CRITICAL_MODE};
enum counter{CNT_QUEUE_SIZE, CNT_TOTAL_DQ, CNT_TOTAL_EQ, CNT_IP, CNT_ICMP,
             CNT_UDP, CNT_TCP, CNT_RTSP, CNT_RTP, CNT_RTCP,
             CNT_PROTOCOL, CNT_SESSION,ESTIMATED_MOTION,ESTIMATED_QUALITY,
             counter_size=14};

class statistic_
{
public:
    statistic_();
    void incrementCounter(unsigned short int type);
    void decrementCounter(unsigned short int type);
    unsigned long int getCounter(unsigned short int type);
    unsigned long int setCounter(unsigned short int type, unsigned long int value);
    void messageOut(const short unsigned int Level_, const char *func, const char *message);
//    friend ostream &operator<<(ostream &output, unsigned short int, const char*, const char *str);
private:
    unsigned long int cnt[counter_size];
    QMutex  mutex;
    int Level;
};

#endif // STATISTIC_H
