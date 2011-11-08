#include "process.h"

process::process()
{
}
//-------------------------------------------------------------------
process::~process()
{

}
//-------------------------------------------------------------------
void
process::start()
{
//    const char payload[] = "RTSP/1.0 200 OK\n\rServer: VLC/1.1.3\n\rDate: Wed, 26 Oct 2011 10:49:14 GMT\n\rTransport: RTP/AVP/UDP;unicast;client_port=52988-52989;server_port=56572-56573;ssrc=8B12546D;mode=play\n\rSession: ed14c55c74919373\n\rContent-Length: 0\n\rCache-Control: no-cache\n\rCseq: 4\n\r\n\r";
    const char payload[] = "TEARDOWN rtsp://127.0.0.1:8090/test RTSP/1.0\n\rCSeq: 7\n\rSession: ed14c55c74919373\n\rUser-Agent: LibVLC/1.1.3 (LIVE555 Streaming Media v2010.04.09)\n\rqstr = payload;\n\r\n\r";

    if (tokenize(QString(payload), NULL, NULL))
    {
        if (ports.type == "RTSP")
            std::cout << ports.rtp_client <<" "<< ports.rtcp_client <<" "<< ports.rtp_server <<" "<< ports.rtcp_server << ports.session.toStdString() << std::endl;
        else if (ports.type == "TEARDOWN")
            std::cout << ports.session.toStdString() << std::endl;
    }
    else
        std::cout << "nothing has found !" << std::endl;

    exit(0);
}
//-------------------------------------------------------------------
bool
process::tokenize(const QString qstr, const char del, const char desc)
{
    std::cout << "--->" << qstr.toStdString() << std::endl;

    QStringList qsl;
    if (!del)
    {
        qsl = qstr.split("\n\r");

        if (qsl.first().contains("RTSP/1.0 200 OK"))
            ports.type = "RTSP";
        else if (qsl.first().contains("TEARDOWN"))
            ports.type = "TEARDOWN";
        else
            return false;
    }
    else
        qsl = qstr.split(del);
    QStringListIterator qsli(qsl);

    while(qsli.hasNext())
    {
        switch (del)
        {
        case NULL:
            if (qsli.peekNext().contains("Transport"))
                tokenize(qsli.next(), ';', desc);
            else  if (qsli.peekNext().contains("Session"))
                tokenize(qsli.next(), ':', desc);
            else
                qsli.next();
            break;

        case ';':
            if ( (qsli.peekNext().contains("client_port"))|| (qsli.peekNext().contains("server_port")) )
                tokenize(qsli.next(), '=', desc);
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
                tokenize(qsli.next(), '-', 'c');
            }
            else if ( qsli.peekNext().contains("server_port") != -1 )
            {
                qsli.next();
                tokenize(qsli.next(), '-', 's');
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
