#include <QtCore/QCoreApplication>
#include "evaluation.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    evaluation eval_inst(argc, argv);
    return a.exec();
}
