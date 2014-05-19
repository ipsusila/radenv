#include <QApplication>
#include <QDir>
#include <QtDebug>
#include <QSettings>
#include <QIcon>
#include <QVariant>
#include "mainwindow.h"
#include "uioutputview.h"
#include "kapplication.h"

int main(int argc, char *argv[])
{
    //setup applications
    KApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/std/app-icon.png"));


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

    //get latest storage
    QSettings settings;
    QString lastStorage = settings.value("app/storage", "").toString();
    if (!lastStorage.isEmpty())
        a.setStorage(lastStorage);

    //output handler
    UiOutputView * outview = new UiOutputView();
    KOutput::setupHandler(outview);

    //display main window
    MainWindow w(outview);
    w.show();
    
    return a.exec();
}
