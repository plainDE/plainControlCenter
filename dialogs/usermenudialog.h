#ifndef USERMENUDIALOG_H
#define USERMENUDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>

class UserMenuDialog : public Dialog {
public:
    UserMenuDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QRadioButton* customRadioButton,
                       QLineEdit* customLineEdit,
                       QCheckBox* enableShutdownTimerCheckBox,
                       QSpinBox* shutdownTimerSpinBox);
};

#endif // USERMENUDIALOG_H
