#include "../pane.h"
#include "../settings.h"

#ifndef APPEARANCEPANE_H
#define APPEARANCEPANE_H


class AppearancePane
{
public:
    void readConfig();
    QWidget* createUI(QWidget* controlCenter);
    void setCurrentSettings(QListWidget* iconThemesListWidget,
                            QFontComboBox* fontFamilyComboBox,
                            QSpinBox* fontSizeSpinBox,
                            QRadioButton* lightRadioButton,
                            QRadioButton* darkRadioButton,
                            QLineEdit* accentLineEdit);
    void prepareUI(QListWidget* iconThemesListWidget);
    void saveSettings(QListWidget* iconThemesListWidget,
                      QFontComboBox* fontFamilyComboBox,
                      QSpinBox* fontSizeSpinBox,
                      QRadioButton* lightRadioButton,
                      QLineEdit* accentLineEdit);

};

#endif // APPEARANCEPANE_H
