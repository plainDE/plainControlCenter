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
                            QCheckBox* panelAnimationCheckBox,
                            QCheckBox* expandPanelCheckBox,
                            QSpinBox* panelOffsetSpinBox,
                            QDoubleSpinBox* panelOpacitySpinBox);
    void saveSettings(QSpinBox* panelHeightSpinBox,
                      QRadioButton* panelTopLocationRadioButton,
                      QCheckBox* panelAnimationCheckBox,
                      QCheckBox* expandPanelCheckBox,
                      QSpinBox* panelOffsetSpinBox,
                      QDoubleSpinBox* panelOpacitySpinBox);
};

#endif // PANEL_H
