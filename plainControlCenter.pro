QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets dbus

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    pane.cpp \
    panes/appearance.cpp \
    panes/appletPanes/appmenu.cpp \
    panes/appletPanes/datetime.cpp \
    panes/appletPanes/localipv4.cpp \
    panes/autostart.cpp \
    panes/keyboard/keyboard.cpp \
    panes/keyboard/layoutdialog.cpp \
    panes/panels.cpp \
    settings.cpp

HEADERS += \
    pane.h \
    panes/appearance.h \
    panes/appletPanes/appmenu.h \
    panes/appletPanes/datetime.h \
    panes/appletPanes/localipv4.h \
    panes/autostart.h \
    panes/keyboard/keyboard.h \
    panes/keyboard/layoutdialog.h \
    panes/panels.h \
    settings.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

FORMS += \
    panes/monitors.ui \
    panes/panels.ui
