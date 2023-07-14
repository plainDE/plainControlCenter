#ifndef WINLISTAPPLETPANE_H
#define WINLISTAPPLETPANE_H

#include <QWidget>
#include <QSpinBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QFile>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QSpacerItem>

#include "../../settings.h"
#include "../../pane.h"

class WinListAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QSpinBox* sizeSpinBox, QCheckBox* showTitlesCheckBox);
    void saveSettings(QSpinBox* sizeSpinBox, QCheckBox* showTitlesCheckBox);
    QWidget* createUI(Settings* controlCenter);
};

#endif // WINLISTAPPLETPANE_H
