#include <QtCore/QCoreApplication>
#include "evaluation.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const char *netdev = argv[1];
    evaluation eval_inst(netdev);

    return a.exec();
}
