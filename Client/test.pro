QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT +=websockets
QT +=xml
QT +=network

SOURCES += \
    edit.cpp \
    loadjson_sovd.cpp \
    main.cpp \
    mainwindow.cpp \
    parsexml.cpp \
    securitymanager.cpp \
    simulation.cpp
HEADERS += \
    edit.h \
    loadjson_sovd.h \
    mainwindow.h \
    parsexml.h \
    securitymanager.h \
    simulation.h
FORMS += \
    mainwindow.ui
INCLUDEPATH += C:\Users\mobenbra\Downloads\PCAN-ISO-TP\Include
LIBS += "C:/Users\mobenbra/Downloads/PCAN-ISO-TP/x64/VC_LIB/PCAN-ISO-TP.lib"
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RESOURCES += \
    ressources.qrc
