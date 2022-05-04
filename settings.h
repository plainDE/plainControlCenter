#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QJsonObject>

class settings : public QWidget
{
    Q_OBJECT

public:
    settings(QWidget *parent = nullptr);
    ~settings();

};
#endif // SETTINGS_H
