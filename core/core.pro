#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T19:12:45
#
#-------------------------------------------------

QT       += xml core gui sql

TARGET = radpfcore
win32:CONFIG(debug, debug|release):join(TARGET,,,d)

TEMPLATE = lib

DEFINES += KCORE_LIBRARY
DESTDIR = $$OUT_PWD/../radpf

SOURCES += \
    dialoglocation.cpp \
    dialogradionuclide.cpp \
    radionuclidemodel.cpp \
    locationmodel.cpp \
    dialogselectnuclide.cpp \
    kcase.cpp \
    khalflife.cpp \
    kelement.cpp \
    kmodelinfo.cpp \
    kgroupinfo.cpp \
    kradionuclide.cpp \
    imodel.cpp \
    klocation.cpp \
    kport.cpp \
    koutput.cpp \
    klocationport.cpp \
    kstorage.cpp \
    kstoragecontent.cpp \
    kconnector.cpp \
    kreport.cpp \
    kreportport.cpp \
    kmodelscene.cpp \
    kdata.cpp \
    kcalculationinfo.cpp \
    dialogcalculation.cpp \
    kmath.cpp \
    kmodelrunner.cpp \
    uiuserinput.cpp \
    uiautorowtable.cpp \
    ksettingmanager.cpp \
    radquantity.cpp \
    kquantitycontrol.cpp \
    dialoguserinput.cpp \
    uiarrayitemtable.cpp \
    uiarrayitemdelegate.cpp

HEADERS +=\
    kradionuclide.hxx \
    dialoglocation.h \
    dialogradionuclide.h \
    radionuclidemodel.h \
    locationmodel.h \
    dialogselectnuclide.h \
    kcase.h \
    khalflife.h \
    kelement.h \
    kmodelinfo.h \
    kgroupinfo.h \
    imodelfactory.h \
    kradionuclide.h \
    imodel.h \
    klocation.h \
    kport.h \
    koutput.h \
    klocationport.h \
    kstorage.h \
    kradionuclide.hxx \
    kstoragecontent.h \
    kconnector.h \
    iparamseditor.h \
    kreport.h \
    kreportport.h \
    kmodelscene.h \
    kgroup.h \
    iuserinput.h \
    kdata.h \
    kcalculationinfo.h \
    dialogcalculation.h \
    kmath.h \
    radcore.h \
    radglobal.h \
    kmodelrunner.h \
    uiuserinput.h \
    uiautorowtable.h \
    ksettingmanager.h \
    radquantity.h \
    kquantitycontrol.h \
    dialoguserinput.h \
    uiarrayitemtable.h \
    uiarrayitemdelegate.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE664C2FB
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = radpfcore.dll
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

FORMS += \
    dialoglocation.ui \
    dialogradionuclide.ui \
    dialogselectnuclide.ui \
    dialogcalculation.ui
