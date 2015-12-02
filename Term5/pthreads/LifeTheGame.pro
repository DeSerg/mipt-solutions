TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    master.cpp \
    worker.cpp \
    support.cpp \
    m_start.cpp \
    m_status.cpp \
    m_run.cpp \
    m_stop.cpp \
    m_quit.cpp \
    m_help.cpp

LIBS += -lpthread

HEADERS += \
    support.h \
    master.h \
    worker.h

DISTFILES += \
    table.csv
