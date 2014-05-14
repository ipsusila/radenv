#include <QApplication>
#include <QDir>
#include <QtDebug>
#include <QSettings>
#include "mainwindow.h"
#include "uioutputview.h"
#include "kpluginmanager.h"

int main(int argc, char *argv[])
{
    //setup applications
    QApplication a(argc, argv);

    //setup settings
    QCoreApplication::setOrganizationName("BATAN");
    QCoreApplication::setOrganizationDomain("www.batan.go.id");
    QCoreApplication::setApplicationName("RADENV Platform");

    QDir dir(a.applicationDirPath());
    dir.cd("plugins");
    if (dir.exists()) {
        QApplication::addLibraryPath(dir.absolutePath());
        qDebug() << "Adding library path: " << dir.absolutePath();
    }

    //create plugin manager
    KPluginManager pm;
    //get latest storage
    QSettings settings;
    QString lastStorage = settings.value("app/storage", "").toString();
    if (!lastStorage.isEmpty())
        pm.setStorage(lastStorage);

    //output handler
    UiOutputView * outview = new UiOutputView();
    KOutput::setupHandler(outview);

    //display main window
    MainWindow w(&pm, outview);
    w.show();
    
    return a.exec();
}
