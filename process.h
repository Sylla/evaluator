#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <QtCore>

struct ports__
{
    unsigned int rtp_client;
    unsigned int rtp_server;
    unsigned int rtcp_client;
    unsigned int rtcp_server;
    QString      session;
    QString      type;
};

class process
{
public:

    process();
    ~process();
    void start();
    bool tokenize(const QString qstr, const char del, const char desc);
private:
    ports__     ports;
};

#endif // PROCESS_H
