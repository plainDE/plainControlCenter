QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets dbus network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    dialogs/addappletdialog.cpp \
    dialogs/addautostartentrydialog.cpp \
    dialogs/appmenudialog.cpp \
    dialogs/clioutputdialog.cpp \
    dialogs/datetimedialog.cpp \
    dialogs/kbindicatordialog.cpp \
    dialogs/launcherdialog.cpp \
    dialogs/layoutadddialog.cpp \
    dialogs/localipv4dialog.cpp \
    dialogs/usermenudialog.cpp \
    dialogs/volumedialog.cpp \
    dialogs/windowlistdialog.cpp \
    dialogs/workspacesdialog.cpp \
    panes/appearancepane.cpp \
    panes/autostartpane.cpp \
    panes/generalpane.cpp \
    panes/keyboardpane.cpp \
    main.cpp \
    mainwindow.cpp \
    pane.cpp \
    panes/panelspane.cpp \
    panes/soundspane.cpp

HEADERS += \
    dialog.h \
    dialogs/addappletdialog.h \
    dialogs/addautostartentrydialog.h \
    dialogs/appmenudialog.h \
    dialogs/clioutputdialog.h \
    dialogs/datetimedialog.h \
    dialogs/kbindicatordialog.h \
    dialogs/launcherdialog.h \
    dialogs/layoutadddialog.h \
    dialogs/localipv4dialog.h \
    dialogs/usermenudialog.h \
    dialogs/volumedialog.h \
    dialogs/windowlistdialog.h \
    dialogs/workspacesdialog.h \
    panes/appearancepane.h \
    panes/autostartpane.h \
    panes/generalpane.h \
    panes/keyboardpane.h \
    mainwindow.h \
    pane.h \
    panes/panelspane.h \
    panes/soundspane.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS +=
