#-------------------------------------------------
#
# Project created by QtCreator 2011-10-22T14:09:58
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = evaluator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    pcap_class.cpp \
    evaluation.cpp \
    packetmanagement.cpp \
    statistic.cpp \
    protocol_rtsp.cpp \
    textui.cpp \
    protocolhandler.cpp \
    protocol_icmp.cpp

HEADERS += \
    pcap_class.h \
    evaluation.h \
    packetmanagement.h \
    statistic.h \
    protocol_rtsp.h \
    textui.h \
    protocolhandler.h \
    protocol_icmp.h
LIBS    += -lpcap
