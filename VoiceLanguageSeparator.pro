#-------------------------------------------------
#
# Project created by QtCreator 2021-01-11T08:09:43
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VOSK
TEMPLATE = app
RC_FILE = myapp.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    src/voskprocessor.cpp \
    src/DirProcessor.cpp \
    src/message.cpp \
    src/wavfile.cpp \
    src/mymessaagemodel.cpp \
    src/tcpserver.cpp \
    src/view.cpp \
    src/settingscontroller.cpp


HEADERS += \
    VoskLib/vosk_api.h \
    src/voskprocessor.h \
    src/DirProcessor.h \
    src/message.h \
    src/wavfile.h \
    src/mymessaagemodel.h \
    src/tcpserver.h \
    src/view.h \
    src/settingscontroller.h





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/VoskLib/release/ -lvosk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/VoskLib/debug/ -lvosk
else:unix:!macx: LIBS += -L$$PWD/VoskLib/ -lvosk -lgfortran


INCLUDEPATH += $$PWD/VoskLib
DEPENDPATH += $$PWD/VoskLib

RESOURCES += \
    resources.qrc
