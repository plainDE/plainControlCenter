QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets dbus

CONFIG += release

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    pane.cpp \
    panes/appearance.cpp \
    panes/appletPanes/appmenu.cpp \
    panes/appletPanes/clioutput.cpp \
    panes/appletPanes/datetime.cpp \
    panes/appletPanes/localipv4.cpp \
    panes/appletPanes/usermenu.cpp \
    panes/appletPanes/volume.cpp \
    panes/appletPanes/winlist.cpp \
    panes/appletPanes/workspaces.cpp \
    panes/autostart.cpp \
    panes/keyboard/keyboard.cpp \
    panes/keyboard/layoutdialog.cpp \
    panes/panels.cpp \
    settings.cpp

HEADERS += \
    pane.h \
    panes/appearance.h \
    panes/appletPanes/appmenu.h \
    panes/appletPanes/clioutput.h \
    panes/appletPanes/datetime.h \
    panes/appletPanes/localipv4.h \
    panes/appletPanes/usermenu.h \
    panes/appletPanes/volume.h \
    panes/appletPanes/winlist.h \
    panes/appletPanes/workspaces.h \
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
    panes/panels.ui
