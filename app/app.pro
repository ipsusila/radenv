#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T07:55:30
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#radcore version
RADENVCORE = radenvcore
RADENVCOREV = 1
#change here fore diff core

VERSION = 1.0.0
TARGET = radenv
TEMPLATE = app
win32:CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
    RADENVCORE = $$join(RADENVCORE,,,d)
}
RADENVCORE = $$join(RADENVCORE,,,$$RADENVCOREV)

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

win32: LIBS += -L$$OUT_PWD/../radenv/ -l$$RADENVCORE
else:symbian: LIBS += -l$$RADENVCORE
else:unix: LIBS += -L$$OUT_PWD/../radenv/ -l$$RADENVCORE

RESOURCES += \
    radapp.qrc

