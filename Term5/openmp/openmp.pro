TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    worker.cpp \
    m_run.cpp \
    master.cpp \
    m_start.cpp \
    m_quit.cpp \
    m_stop.cpp \
    m_help.cpp \
    m_status.cpp \
    support.cpp

DISTFILES += \
    table.csv

HEADERS += \
    support.h \
    master.h \
    worker.h

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp
