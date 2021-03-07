QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boutton.cpp \
    fendocu.cpp \
    fenprogrammes.cpp \
    fenvariables.cpp \
    litterales.cpp \
    main.cpp \
    mainwindow.cpp \
    pile.cpp \
    userinput.cpp

HEADERS += \
    boutton.h \
    compception.h \
    fendocu.h \
    fenprogrammes.h \
    fenvariables.h \
    json.hpp \
    litterales.h \
    macro.h \
    mainwindow.h \
    pile.h \
    userinput.h

FORMS += \
    mainwindow.ui



# set PROJECT_PATH to the "project path" with a trailing slash
DEFINES += PROJECT_PATH=\"\\\"$${_PRO_FILE_PWD_}/\\\"\"


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




