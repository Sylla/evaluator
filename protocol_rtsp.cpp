#include "protocol_rtsp.h"
#include "protocol_rtp.h"
#include "protocol_rtcp.h"

protocol_rtsp::protocol_rtsp(u_int16_t sport, u_int16_t dport , QSet<protocolHandler_ *> *protocolHandler__, statistic_ *stat)
    : protocolHandler_(sport, dport, stat)
{
    protocolHandler = protocolHandler__;
}
//-----------------------------------------------------------------------------
protocol_rtsp::~protocol_rtsp()
{
    QMultiHash<QString, protocolHandler_ *>::iterator     sessionHandlerIterator = sessionHandler.begin();
    while (sessionHandlerIterator != sessionHandler.end())
    {
        delete sessionHandlerIterator.value();
        protocolHandler->remove(sessionHandlerIterator.value());
        ++sessionHandlerIterator;
    }
    sessionHandler.clear();
}
//-----------------------------------------------------------------------------
int
protocol_rtsp::check(u_int16_t protocol, u_int16_t sport, u_int16_t dport, const u_char *payload, unsigned int payload_length)
{
    if( protocol == IPPROTO_TCP && (sport == source_port || dport == source_port))
    {
        statistic->incrementCounter(CNT_RTSP);
        statistic->setCounter(CNT_SESSION, sessionHandler.size());

        if (process(QString((char*)payload), NULL, NULL))
        {
            if (ports.type == "RTSP")
            {
                protocolHandler_ *temp;

                temp = new protocol_rtp(ports.rtp_server, ports.rtp_client, statistic);
                protocolHandler->insert(temp);
                sessionHandler.insert(ports.session, temp);

                temp = new protocol_rtcp(ports.rtcp_server, ports.rtcp_client, statistic);
                protocolHandler->insert(temp);
                sessionHandler.insert(ports.session, temp);

//                std::cout << ports.rtp_client <<" "<< ports.rtcp_client <<" "<< ports.rtp_server <<" "<< ports.rtcp_server << ports.session.toStdString() << std::endl;
            }
            else if (ports.type == "TEARDOWN")
            {
                QMultiHash<QString, protocolHandler_ *>::iterator     sessionHandlerIterator = sessionHandler.begin();
                while (sessionHandlerIterator != sessionHandler.end())
                {
                    if (sessionHandlerIterator.key() == ports.session)
                    {
                        delete sessionHandlerIterator.value();
                        protocolHandler->remove(sessionHandlerIterator.value());
                    }
                    ++sessionHandlerIterator;
                }
                sessionHandler.remove(ports.session);
            }
        }
    }
    return false;
}
//-----------------------------------------------------------------------------
int
protocol_rtsp::process(const QString qstr, const char del, const char desc)
{

    QStringList qsl;
    if (!del)
    {
        qsl = qstr.split("\r\n");
        ports.type = " ";

        if (qsl.first().contains("TEARDOWN"))
            ports.type = "TEARDOWN";
        else if (!qsl.first().contains("RTSP/1.0 200 OK"))
            return false;
    }
    else
        qsl = qstr.split(del);

    QStringListIterator qsli(qsl);

//    std::cout << qstr.toStdString() << "   "<< (ports.type=="RTSP"?"RTSP":(ports.type=="TEARDOWN"?"TEARDOWN":" ")) << std::endl;
    while(qsli.hasNext())
    {
        switch (del)
        {
            // Line search
        case NULL:
//            std::cout << qsli.peekNext().toStdString() << "   "<< (ports.type=="RTSP"?"RTSP":(ports.type=="TEARDOWN"?"TEARDOWN":" ")) << std::endl;
            if (qsli.peekNext().contains("Transport"))
            {
                ports.type = "RTSP";
                process(qsli.next(), ';', desc);
            }
            else if ((ports.type == "RTSP" || ports.type == "TEARDOWN") && qsli.peekNext().contains("Session"))
                process(qsli.next(), ':', desc);
            else
                qsli.next();
            break;

        case ';':
            if ( (qsli.peekNext().contains("client_port"))|| (qsli.peekNext().contains("server_port")) )
                process(qsli.next(), '=', desc);
            else
                qsli.next();
            break;

        case ':':
            if (qsli.next().contains("Session"))
            {
                ports.session = qsli.next();
            }else
                qsli.next();
            break;

        case '=':
            if ( qsli.peekNext().contains("client_port") != -1 )
            {
                qsli.next();
                process(qsli.next(), '-', 'c');
            }
            else if ( qsli.peekNext().contains("server_port") != -1 )
            {
                qsli.next();
                process(qsli.next(), '-', 's');
            }
            break;

        case '-':
            if(desc == 'c')
            {
                ports.rtp_client = qsli.next().toUInt();
                ports.rtcp_client = qsli.next().toUInt();
            }
            else if(desc == 's')
            {
                ports.rtp_server = qsli.next().toUInt();
                ports.rtcp_server = qsli.next().toUInt();
            }
            else
                std::cout << __func__ << __LINE__ << "Error occured !" << std::endl;
            break;
        }
    }
    return true;
}
//-----------------------------------------------------------------------------
