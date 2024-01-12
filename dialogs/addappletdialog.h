#ifndef ADDAPPLETDIALOG_H
#define ADDAPPLETDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QScreen>
#include <QListWidget>
#include <QPushButton>

class AddAppletDialog : public Dialog {
public:
    AddAppletDialog(QJsonObject* cfgObj,
                    int panelID,
                    QListWidget* appletsListWidget);
    virtual void setPaneContents() override;

private:
    QListWidget* mAppletsListWidget;
    QJsonObject* mCfgObj;
    int mPanelID;
};

#endif // ADDAPPLETDIALOG_H
