#ifndef LAYOUTADDDIALOG_H
#define LAYOUTADDDIALOG_H

#include "../dialog.h"

#include <QListWidget>
#include <QPushButton>
#include <QFile>
#include <QJsonDocument>
#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

class LayoutAddDialog : public Dialog {
public:
    LayoutAddDialog(QJsonObject* cfgObj,
                    QListWidget* activeLayoutsListWidget);
    virtual void setPaneContents() override;

private:
    QVariantMap mLayouts;
    QListWidget* mActiveLayoutsListWidget;
};

#endif // LAYOUTADDDIALOG_H
