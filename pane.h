#ifndef PANE_H
#define PANE_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QIcon>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QLabel>
#include <QScrollArea>
#include <QDir>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QMessageBox>

class Pane : public QWidget {
    Q_OBJECT
public:
    Pane(QWidget* parent = nullptr,
         QJsonObject* cfgObj = nullptr,
         QString paneName = "",
         QString iconName = "");
    QJsonValue getConfigValue(QString entry);
    QJsonValue getConfigValue(QString entry, QString subentry);
    void setAppearance(QWidget* widget);
    void setTransparency(QWidget* pane);
    void setBasicUI(QString paneName, QString iconName);
    void finalizeUI();
    void setEntry(QString key, QJsonValue value);
    void saveConfig();

    virtual void setPaneContents();

    QJsonObject* mCfgObj;
    QWidget* mContentsWidget;
    QBoxLayout* mLayout;
    QFont mFont;
    QFont mTitleFont;
};

#endif // PANE_H
