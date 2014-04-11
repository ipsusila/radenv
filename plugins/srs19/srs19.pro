#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T21:36:50
#
#-------------------------------------------------

QT       += core gui

TARGET = srs19
TEMPLATE = lib
CONFIG += plugin
win32:CONFIG(debug, debug|release):join(TARGET,,,d)

DEFINES += SRS19_LIBRARY

SOURCES += \
    srs19factory.cpp \
    discharge.cpp \
    dischargeitemdelegate.cpp \
    dischargeitemtable.cpp \
    transport.cpp \
    atmospherictransport.cpp \
    rivertransport.cpp \
    estuarinetransport.cpp \
    coastaltransport.cpp \
    smalllaketransport.cpp \
    largelaketransport.cpp \
    widgetatmosphericdischarge.cpp \
    widgetwaterdischarge.cpp \
    sedimenteffect.cpp \
    constantvalue.cpp \
    constantvaluetable.cpp \
    constantvaluedelegate.cpp \
    widgetconstantvalue.cpp \
    irrigation.cpp \
    vegetation.cpp \
    storedforage.cpp \
    animalfeed.cpp \
    animaltransport.cpp \
    foodchain.cpp \
    quantity.cpp \
    defaultvalues.cpp \
    sludgetransport.cpp \
    grounddeposition.cpp \
    sewagedischarge.cpp \
    aquaticfoodransport.cpp

HEADERS += $$PWD/../smeacore/imodelfactory.h\
    srs19.h\
    srs19factory.h \
    discharge.h \
    dischargeitemdelegate.h \
    dischargeitemtable.h \
    transport.h \
    atmospherictransport.h \
    rivertransport.h \
    estuarinetransport.h \
    coastaltransport.h \
    smalllaketransport.h \
    largelaketransport.h \
    widgetatmosphericdischarge.h \
    widgetwaterdischarge.h \
    sedimenteffect.h \
    constantvalue.h \
    constantvaluetable.h \
    constantvaluedelegate.h \
    widgetconstantvalue.h \
    irrigation.h \
    vegetation.h \
    storedforage.h \
    animalfeed.h \
    animaltransport.h \
    foodchain.h \
    quantity.h \
    defaultvaluetable.h \
    defaultvalues.h \
    sludgetransport.h \
    grounddeposition.h \
    sewagedischarge.h \
    aquaticfoodtransport.h


symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE16A5774
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = srs19.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../radpf/ -lradpfcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../radpf/ -lradpfcored
else:symbian: LIBS += -lradpfcore
else:unix: LIBS += -L$$OUT_PWD/../../radpf/ -lradpfcore

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core
DESTDIR = $$OUT_PWD/../../radpf/plugins

RESOURCES += \
    srs19.qrc

FORMS += \
    widgetatmosphericdischarge.ui \
    widgetwaterdischarge.ui \
    widgetconstantvalue.ui
