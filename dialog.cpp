#include "dialog.h"

void Dialog::setTransparency(QWidget* widget) {
    QScreen* screen = widget->screen();
    QRect widgetGeometry = widget->geometry();
    QPixmap pixmap = screen->grabWindow(0,
                                        widgetGeometry.x(),
                                        widgetGeometry.y(),
                                        widgetGeometry.width(),
                                        widgetGeometry.height());
    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(15);
    blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem item;
    item.setPixmap(pixmap);
    item.setGraphicsEffect(blurEffect);
    scene->addItem(&item);
    QImage res(QSize(widgetGeometry.width(), widgetGeometry.height()),
               QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene->render(&ptr, QRectF(), QRectF(0, 0, widgetGeometry.width(),
                                         widgetGeometry.height()));

    QPalette palette;
    palette.setBrush(widget->backgroundRole(),
                     QBrush(QPixmap::fromImage(res)));
    widget->setPalette(palette);
}

void Dialog::setWindowGeometry() {
    QScreen* screen = this->screen();
    QRect geometry = screen->geometry();
    int width = 500, height = 350;
    int x = (geometry.width() - width) / 2;
    int y = (geometry.height() - height) / 2;
    this->setGeometry(x, y, width, height);
}

Dialog::Dialog(QJsonObject* cfgObj, QString title, QString iconName) : Pane(nullptr,
                                                                            cfgObj,
                                                                            title,
                                                                            iconName) {

}
