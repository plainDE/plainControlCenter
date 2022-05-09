#include "../pane.h"
#include "../settings.h"

#ifndef APPEARANCEPANE_H
#define APPEARANCEPANE_H


class AppearancePane {
public:
    void readConfig();
    QWidget* createUI(QWidget* controlCenter);
    void setCurrentSettings(QListWidget* iconThemesListWidget,
                            QFontComboBox* fontFamilyComboBox,
                            QSpinBox* fontSizeSpinBox,
                            QListWidget* themesListWidget,
                            QLineEdit* accentLineEdit);
    void prepareUI(QListWidget* iconThemesListWidget,
                   QListWidget* themesListWidget);
    void saveSettings(QListWidget* iconThemesListWidget,
                      QFontComboBox* fontFamilyComboBox,
                      QSpinBox* fontSizeSpinBox,
                      QListWidget* themesListWidget,
                      QLineEdit* accentLineEdit);

};

#endif // APPEARANCEPANE_H
