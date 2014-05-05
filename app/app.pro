#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T07:55:30
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = radenv
TEMPLATE = app
win32:CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

SOURCES += main.cpp\
    mainwindow.cpp \
    xmodelview.cpp \
    xmodelwidget.cpp \
    xactionbutton.cpp

HEADERS  += mainwindow.h \
    xmodelview.h \
    xmodelaction.h \
    xmodelwidget.h \
    xactionbutton.h

FORMS    +=

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
DESTDIR = $$OUT_PWD/../radenv

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../radenv/ -lradenvcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../radenv/ -lradenvcored
else:symbian: LIBS += -lradenvcore
else:unix: LIBS += -L$$OUT_PWD/../radenv/ -lradenvcore

RESOURCES += \
    radapp.qrc

