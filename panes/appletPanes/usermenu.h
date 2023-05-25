#ifndef USERMENUAPPLETPANE_H
#define USERMENUAPPLETPANE_H

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
#include <QLineEdit>
#include <QRadioButton>

#include "../../settings.h"
#include "../../pane.h"

class UserMenuAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QRadioButton* useDefaultIcon,
                            QRadioButton* useAvatar,
                            QLineEdit* avatarPath,
                            QPushButton* previewButton);
    void saveSettings(QRadioButton* useDefaultIcon,
                      QRadioButton* useAvatar,
                      QLineEdit* avatarPath);
    QWidget* createUI(Settings* controlCenter);
};

#endif // USERMENUAPPLETPANE_H
