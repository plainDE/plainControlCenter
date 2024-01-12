#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QFont>
#include <QCompleter>
#include <QStandardItemModel>
#include <QApplication>
#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QProcess>

#include "panes/generalpane.h"
#include "panes/appearancepane.h"
#include "panes/keyboardpane.h"
#include "panes/autostartpane.h"
#include "panes/panelspane.h"

#include "dialogs/usermenudialog.h"
#include "dialogs/datetimedialog.h"
#include "dialogs/volumedialog.h"
#include "dialogs/appmenudialog.h"
#include "dialogs/localipv4dialog.h"
#include "dialogs/windowlistdialog.h"
#include "dialogs/kbindicatordialog.h"
#include "dialogs/workspacesdialog.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,
               QApplication* app = nullptr);
    void readConfig();
    void setAppearance();
    void createUI(QApplication* app);
    void setTransparency();
    ~MainWindow();

private slots:
    void goTo(QString entry);
    void goTo(QModelIndex index);

private:
    void showPane(QString name);
    void showDialog(QString name);

    QFont mFont;
    QFont mTitleFont;
    QJsonObject mCfgObj;
    QWidget* mCurrentWidget;
    QList<QWidget*> mHistory;
    int mHistoryPointer = 0;
    QJsonObject mSearchDBObj;
    QVBoxLayout* mMainLayout;
};
#endif // MAINWINDOW_H
