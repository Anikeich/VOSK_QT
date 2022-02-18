#include "mainwindow.h"
#include <QApplication>
#include <src/separator_view.h>
#include <stdlib.h>
#include <QDebug>
#include <fstream>
#include "src/message.h"
#include <QByteArray>


using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     separator_view view;
    view.setArg(argc,argv);
    view.show();
    return a.exec();
}
