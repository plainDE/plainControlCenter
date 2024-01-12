#ifndef DIALOG_H
#define DIALOG_H

#include "pane.h"

#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

class Dialog : public Pane {
public:
    Dialog(QJsonObject* cfgObj, QString title, QString iconName);
    void setTransparency(QWidget* widget);
    void setWindowGeometry();
};

#endif // DIALOG_H
