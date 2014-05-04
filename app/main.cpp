#include <QApplication>
#include <QDir>
#include <QDebug>
#include "mainwindow.h"
#include "xoutputview.h"
#include "kpluginmanager.h"

int main(int argc, char *argv[])
{
    //setup applications
    QApplication a(argc, argv);
    QDir dir(a.applicationDirPath());
    dir.cd("plugins");
    if (dir.exists()) {
        QApplication::addLibraryPath(dir.absolutePath());
        qDebug() << "Adding library path: " << dir.absolutePath();
    }

    //create plugin manager
    KPluginManager pm;
    pm.setStorage("/home/ips/workspace/smea.db");

    //output handler
    XOutputView * outview = new XOutputView();
    KOutput::setupHandler(outview);

    //display main window
    MainWindow w(&pm, outview);
    w.show();
    
    return a.exec();
}
