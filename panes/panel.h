#ifndef PANEL_H
#define PANEL_H

#include "../pane.h"

class PanelPane {
public:
    void readConfig();
    QWidget* createUI(QWidget* controlCenter);
    void setCurrentSettings(QSpinBox* panelHeightSpinBox,
                            QRadioButton* panelTopLocationRadioButton,
                            QRadioButton* panelBottomLocationRadioButton,
                            QCheckBox* panelAnimationCheckBox);
    void saveSettings(QSpinBox* panelHeightSpinBox,
                      QRadioButton* panelTopLocationRadioButton,
                      QCheckBox* panelAnimationCheckBox);
};

#endif // PANEL_H
