TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

QT += core
QT -= gui
TARGET = message_queue

SOURCES += main.cpp \
    messagetype.cpp \
    messagequeue.cpp \
    messagequeuereader.cpp \
    messagequeuewriter.cpp

HEADERS += \
    retcodes.h \
    imessagequeueevents.h \
    messagetype.h \
    messagequeue.h \
    logging.h \
    imessagequeue.h \
    messagequeuereader.h \
    messagequeuewriter.h

