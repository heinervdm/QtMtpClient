TEMPLATE = lib
TARGET = QtMpdClient
VERSION = 0.2.0
CONFIG += dll
QT += core
HEADERS += qmpdmonitor.h \
    qmpdstatus.h \
    qmpdsong.h \
    qmpdclient.h
SOURCES += qmpdmonitor.cpp \
    qmpdstatus.cpp \
    qmpdsong.cpp \
    qmpdclient.cpp
FORMS += 
RESOURCES += 
LIBS += -lmpdclient
