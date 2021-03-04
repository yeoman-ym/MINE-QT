/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷main
**************************************************/
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
