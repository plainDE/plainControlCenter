#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../pane.h"
#include "../../settings.h"

class KeyboardPane {

public:
    void readConfig();
    QWidget* createUI(Settings* controlCenter);
    void prepareUI(QListWidget* layoutSwitchShortcut);
    void setCurrentSettings(QCheckBox* useCountryFlagCheckBox,
                            QListWidget* layoutList,
                            QListWidget* layoutSwitchShortcut);
    void saveSettings(QCheckBox* useCountryFlagCheckBox,
                      QListWidget* layoutList,
                      QListWidget* layoutSwitchShortcut);
    void addLayout(QString layoutName, QListWidget* layoutList);

    Settings* mControlCenter;
};

#endif // KEYBOARD_H
