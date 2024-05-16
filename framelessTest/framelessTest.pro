QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    framelessWidget/framelesswidget.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    framelessWidget/framelesswidget.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../FFmpegVideoPlay/release/ -ldwmapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../FFmpegVideoPlay/debug/ -ldwmapi

INCLUDEPATH += $$PWD/../FFmpegVideoPlay/dwmapi
DEPENDPATH += $$PWD/../FFmpegVideoPlay/dwmapi

DESTDIR = $$PWD/bin/release

RESOURCES += \
    $$PWD/framelessWidget/qrc.qrc