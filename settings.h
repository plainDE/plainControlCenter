#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QJsonObject>

class Settings {
public:
    Settings();
    void updateStyle();
    void readConfig();
    void createUI();
    ~Settings();

public:
    QWidget* controlCenterWidget;

};
#endif // SETTINGS_H
