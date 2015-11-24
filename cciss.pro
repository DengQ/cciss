#-------------------------------------------------
#
# Project created by QtCreator 2015-11-21T13:03:21
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cciss
TEMPLATE = app

INCLUDEPATH += ccissConnectionFactory

SOURCES += main.cpp\
        widget.cpp \
    ccissConnectionFactory/ccissServer.cpp \
    ccissConnectionFactory/ccissHandShake.cpp \
    ccissConnectionFactory/ccissReceptionTask.cpp \
    ccissConnectionFactory/ccissClient.cpp \
    ccissConnectionFactory/ccissNetworkEnvironment.cpp \
    ccissConnectionFactory/ccissTransmissionTask.cpp \
    ccissConnectionFactory/ModuleIpConfig.cpp

HEADERS  += widget.h \
    ccissConnectionFactory/ccissServer.h \
    ccissConnectionFactory/ccissHandShake.h \
    ccissConnectionFactory/ccissReceptionTask.h \
    ccissConnectionFactory/ccissClient.h \
    ccissConnectionFactory/ccissNetworkEnvironment.h \
    ccissConnectionFactory/ccissTransmissionTask.h \
    ccissConnectionFactory/ModuleIpConfig.h

CONFIG += mobility
MOBILITY = 

