#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../pane.h"

class KeyboardPane {

public:
    void readConfig();
    QWidget* createUI(QWidget* controlCenter);
    void prepareUI(QListWidget* layoutSwitchShortcut);
    void setCurrentSettings(QCheckBox* useCountryFlagCheckBox,
                            QListWidget* layoutList,
                            QListWidget* layoutSwitchShortcut);
    void saveSettings(QCheckBox* useCountryFlagCheckBox,
                      QListWidget* layoutList,
                      QListWidget* layoutSwitchShortcut);
    void addLayout(QString layoutName, QListWidget* layoutList);
};

#endif // KEYBOARD_H
