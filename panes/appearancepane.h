#ifndef APPEARANCEPANE_H
#define APPEARANCEPANE_H

#include "pane.h"

#include <QJsonObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QDir>

class AppearancePane : public Pane {
public:
    AppearancePane(QJsonObject* cfgObj,
                   QWidget* parent);
    virtual void setPaneContents() override;
    void prepareToSave(QComboBox* iconThemeComboBox,
                       QFontComboBox* fontComboBox,
                       QSpinBox* fontSizeSpinBox,
                       QComboBox* themeComboBox,
                       QLineEdit* accentLineEdit,
                       QLineEdit* ipColorLineEdit);

private:
    QWidget* mParent;
};

#endif // APPEARANCEPANE_H
