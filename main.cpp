// Main function that is run to boot up the application.

#include "./miscClasses/mainwindow.h"
#include "./miscClasses/PaintModel.h"

#include <QApplication>


/**/
/*
int main(int argc, char *argv[])

NAME

    main(int argc, char *argv[]) - ran upon executing the application

SYNOPSIS

    int main(int argc, char *argv[]);
        argc --> number of arguments
        argv --> the list of arguments

DESCRIPTION

    boot up the application in a full window

RETURNS

    None

*/
/**/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;

    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
