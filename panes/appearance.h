#ifndef APPEARANCEPANE_H
#define APPEARANCEPANE_H

#include "../pane.h"
#include "../settings.h"

class AppearancePane {
public:
    void updateStyle(QWidget* pane);
    void readConfig();
    QWidget* createUI(Settings* controlCenter);
    void setCurrentSettings(QListWidget* iconThemesListWidget,
                            QFontComboBox* fontFamilyComboBox,
                            QSpinBox* fontSizeSpinBox,
                            QListWidget* themesListWidget,
                            QLineEdit* accentLineEdit,
                            QLineEdit* ipColorLineEdit);
    void prepareUI(QListWidget* iconThemesListWidget,
                   QListWidget* themesListWidget);
    void saveSettings(QListWidget* iconThemesListWidget,
                      QFontComboBox* fontFamilyComboBox,
                      QSpinBox* fontSizeSpinBox,
                      QListWidget* themesListWidget,
                      QLineEdit* accentLineEdit,
                      QLineEdit* ipColorLineEdit);

};

#endif // APPEARANCEPANE_H
