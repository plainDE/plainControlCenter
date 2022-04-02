#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class settings : public QWidget
{
    Q_OBJECT

public:
    settings(QWidget *parent = nullptr);
    ~settings();
};
#endif // SETTINGS_H
