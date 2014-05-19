#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T07:55:30
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

#radcore version, edit as needed
RADENVCORE = radenvcore
RADENVCOREV = 1
MARBLEPATH = c:/FreeSW/marbledev_q5
#change here fore diff core

VERSION = 1.0.0
TARGET = radenv
TEMPLATE = app
win32:CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
    RADENVCORE = $$join(RADENVCORE,,,d)
}
win32:RADENVCORE = $$join(RADENVCORE,,,$$RADENVCOREV)

SOURCES += main.cpp\
    mainwindow.cpp \
    xmodelview.cpp \
    xmodelwidget.cpp \
    xactionbutton.cpp \
    dialogoption.cpp \
    mapview.cpp \
    dialogabout.cpp

HEADERS  += mainwindow.h \
    xmodelview.h \
    xmodelaction.h \
    xmodelwidget.h \
    xactionbutton.h \
    dialogoption.h \
    mapview.h \
    dialogabout.h

FORMS    += \
    dialogoption.ui \
    dialogabout.ui

INCLUDEPATH += $$PWD/../core $$MARBLEPATH/include
DEPENDPATH += $$PWD/../core
DESTDIR = $$OUT_PWD/../radenv

symbian: LIBS += -l$$RADENVCORE -L$$MARBLEPATH/lib -lmarblewidget -lastro
win32: LIBS += -L$$OUT_PWD/../radenv/ -l$$RADENVCORE -L$$MARBLEPATH -lmarblewidget -lastro
else: LIBS += -L$$OUT_PWD/../radenv/ -l$$RADENVCORE -L$$MARBLEPATH/lib -lmarblewidget -lastro

RESOURCES += \
    radapp.qrc

