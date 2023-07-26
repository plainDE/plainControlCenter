#ifndef VOLUMEAPPLETPANE_H
#define VOLUMEAPPLETPANE_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>

#include "../../settings.h"
#include "../../pane.h"


class VolumeAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QRadioButton* alsaRadioButton,
                            QRadioButton* pulseaudioRadioButton);
    void saveSettings(QRadioButton* alsaRadioButton,
                      QRadioButton* pulseaudioRadioButton);
    QWidget* createUI(Settings* controlCenter);
};

#endif // VOLUMEAPPLETPANE_H
