#include <QApplication>
#include <src/view.h>
#include <stdlib.h>
#include <QDebug>
#include <fstream>
#include "src/message.h"
#include <QByteArray>


using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    view b;
    b.setArg(argc,argv);
    b.show();
    return a.exec();
}
