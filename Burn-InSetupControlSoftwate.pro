#-------------------------------------------------
#
# Project created by QtCreator 2018-07-25T11:10:00
#
#-------------------------------------------------

QT       += core gui widgets
QT       += network

TARGET = Burn-InSetupControlSoftwate
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#unix:
LIBS += -L/usr/lib/x86_64-linux-gnu/ -lvisa

#unix:
INCLUDEPATH += /usr/include/ni-visa

SOURCES += \
    external/cmstkmodlab/devices/Julabo/FP50ComHandler.cpp \
    external/cmstkmodlab/devices/Julabo/JulaboFP50.cpp \
    external/cmstkmodlab/devices/Julabo/VJulaboFP50.cpp \
    gui/additionalwindow.cpp \
    gui/mainwindow.cpp \
    voltagecontrol/controlkeithleypower.cpp \
    voltagecontrol/controlttipower.cpp \
    voltagecontrol/powercontrolclass.cpp \
    additionalthread.cpp \
    connectioninterfaceclass.cpp \
    daqcontrolclass.cpp \
    databaseinterfaceclass.cpp \
    environmentcontrolclass.cpp \
    hwdescriptionparser.cpp \
    main.cpp \
    systemcontrollerclass.cpp \
    genericinstrumentclass.cpp

HEADERS += \
    external/cmstkmodlab/devices/Julabo/FP50ComHandler.h \
    external/cmstkmodlab/devices/Julabo/JulaboFP50.h \
    external/cmstkmodlab/devices/Julabo/VJulaboFP50.h \
    gui/additionalwindow.h \
    gui/mainwindow.h \
    voltagecontrol/controlkeithleypower.h \
    voltagecontrol/controlttipower.h \
    voltagecontrol/powercontrolclass.h \
    additionalthread.h \
    connectioninterfaceclass.h \
    daqcontrolclass.h \
    databaseinterfaceclass.h \
    environmentcontrolclass.h \
    hwdescriptionparser.h \
    systemcontrollerclass.h \
    genericinstrumentclass.h

FORMS += \
    gui/additionalwindow.ui \
    gui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    settings/hardware_description_desy.xml
