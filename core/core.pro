#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T19:12:45
#
#-------------------------------------------------

QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0
TARGET = radenvcore
win32:CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

TEMPLATE = lib

DEFINES += RADENV_BUILD_MODULE
DESTDIR = $$OUT_PWD/../radenv

SOURCES += \
    dialoglocation.cpp \
    dialogradionuclide.cpp \
    radionuclidemodel.cpp \
    locationmodel.cpp \
    dialogselectnuclide.cpp \
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
    kdata.cpp \
    kcalculationinfo.cpp \
    dialogcalculation.cpp \
    kmath.cpp \
    uiuserinput.cpp \
    uiautorowtable.cpp \
    ksettingmanager.cpp \
    radquantity.cpp \
    kquantitycontrol.cpp \
    dialoguserinput.cpp \
    uiarrayitemtable.cpp \
    uiarrayitemdelegate.cpp \
    kassessment.cpp \
    uiassessmentexplorer.cpp \
    uioutputview.cpp \
    kscenario.cpp \
    kscenarioevaluator.cpp \
    dialogscenario.cpp \
    dialogassessment.cpp \
    dialogassessmentbrowser.cpp \
    kdbconnector.cpp \
    khtmlreport.cpp \
    kapplication.cpp \
    koptions.cpp

HEADERS +=\
    kradionuclide.hxx \
    dialoglocation.h \
    dialogradionuclide.h \
    radionuclidemodel.h \
    locationmodel.h \
    dialogselectnuclide.h \
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
    kgroup.h \
    iuserinput.h \
    kdata.h \
    kcalculationinfo.h \
    dialogcalculation.h \
    kmath.h \
    radcore.h \
    radglobal.h \
    uiuserinput.h \
    uiautorowtable.h \
    ksettingmanager.h \
    radquantity.h \
    kquantitycontrol.h \
    dialoguserinput.h \
    uiarrayitemtable.h \
    uiarrayitemdelegate.h \
    iserializable.h \
    kassessment.h \
    uiassessmentexplorer.h \
    uioutputview.h \
    kscenario.h \
    kscenarioevaluator.h \
    dialogscenario.h \
    dialogassessment.h \
    dialogassessmentbrowser.h \
    kdbconnector.h \
    khtmlreport.h \
    kapplication.h \
    koptions.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE664C2FB
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = radenvcore.dll
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
    dialogcalculation.ui \
    dialogscenario.ui \
    dialogassessment.ui \
    dialogassessmentbrowser.ui

RESOURCES += \
    core.qrc
