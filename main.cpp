// Main function that is run to boot up the application.

#include "mainwindow.h"
#include "PaintModel.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;

    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
