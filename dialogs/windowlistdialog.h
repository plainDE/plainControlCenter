#ifndef WINDOWLISTDIALOG_H
#define WINDOWLISTDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>

class WindowListDialog : public Dialog {
public:
    WindowListDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QSpinBox* iconSizeSpinBox,
                       QCheckBox* showTitlesCheckBox);
};

#endif // WINDOWLISTDIALOG_H
