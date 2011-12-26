#ifndef STATISTIC_H
#define STATISTIC_H

#include <QtCore>
#include <iostream>

enum modes{SILENT_MODE, VERBOSE_MODE, DEBUG_MODE, CRITICAL_MODE};
enum counter{CNT_QUEUE_SIZE, CNT_TOTAL_DQ, CNT_TOTAL_EQ, CNT_IP, CNT_ICMP,
             CNT_UDP, CNT_TCP, CNT_RTSP, CNT_RTP, CNT_RTCP,
             CNT_PROTOCOL, CNT_SESSION, GOP_SEQ, GOP_P, GOP_B,
             LOSS_SEQ, ESTIMATED_MOTION, ESTIMATED_QUALITY, ESTIMATED_AVERAGE_QUALITY, ESTIMATED_LOSS, counter_size=20};


class statistic_
{
public:
    statistic_();
    void incrementCounter(unsigned short int type);
    void decrementCounter(unsigned short int type);
    double getCounter(unsigned short int type);
    double setCounter(unsigned short int type, double value);
    void messageOut(const short unsigned int Level_, const char *func, const char *message);
//    friend ostream &operator<<(ostream &output, unsigned short int, const char*, const char *str);
private:
    double cnt[counter_size];
    QMutex  mutex;
    int Level;
};

#endif // STATISTIC_H
