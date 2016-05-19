#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T13:50:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pqIcpPropertyWidget
TEMPLATE = app


SOURCES += main.cpp\
        pqIcpPropertyWidget.cxx \
    impossibleoptionexception.cpp \
    outlierfilteroptions.cpp

HEADERS  += pqIcpPropertyWidget.h \
    impossibleoptionexception.h \
    outlierfilteroptions.h

FORMS    += pqIcpPropertyWidget.ui

DEFINES += BUILD_FROM_QT_CREATOR
