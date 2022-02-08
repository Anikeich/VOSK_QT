QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


SOURCES +=  tst_test.cpp \
    ../../src/voskprocessor.cpp

INCLUDEPATH += $$PWD/../../VoskLib
INCLUDEPATH += $$PWD/../../src



unix:!macx: LIBS += -L$$PWD/../../VoskLib/ -lvosk -lgfortran

HEADERS += \
    ../../src/voskprocessor.h \
    ../../VoskLib/vosk_api.h


