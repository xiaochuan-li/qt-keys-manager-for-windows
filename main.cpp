#include <QHotkey>
#include <QApplication>
#include <QDebug>

#include <QSharedMemory>

#include <QMessageBox>
#include "mainwindow.h"
#include "autorun.h"   //开机自动运行


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    autoRun();
    QObject::connect(&window, SIGNAL(pageClose()),&app,SLOT(quit()));

    QSharedMemory shared("apa");
    if(shared.attach())
    {
        QMessageBox::information(NULL,QStringLiteral("Warning"),QStringLiteral("Application is alreadly running!"));
        return 0;
    }
    window.show();
    window.activateWindow();
    shared.create(1);
    return app.exec();
}
