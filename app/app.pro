#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T07:55:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = radpf
TEMPLATE = app
win32:CONFIG(debug, debug|release):join(TARGET,,,d)

SOURCES += main.cpp\
        mainwindow.cpp \ 
    xmodelview.cpp \
    xmodelwidget.cpp \
    xactionbutton.cpp \
    xoutputview.cpp

HEADERS  += $$PWD/../core/imodelfactory.h\
    mainwindow.h \
    xmodelview.h \
    xmodelaction.h \
    xmodelwidget.h \
    xactionbutton.h \
    xoutputview.h

FORMS    +=

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
DESTDIR = $$OUT_PWD/../radpf

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../radpf/ -lradpfcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../radpf/ -lradpfcored
else:symbian: LIBS += -lradpfcore
else:unix: LIBS += -L$$OUT_PWD/../radpf/ -lradpfcore

RESOURCES += \
    radapp.qrc

