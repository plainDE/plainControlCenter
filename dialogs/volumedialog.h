#ifndef VOLUMEDIALOG_H
#define VOLUMEDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QRadioButton>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>

class VolumeDialog : public Dialog {
public:
    VolumeDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QRadioButton* alsaRadioButton,
                       QCheckBox* overamplificationCheckBox,
                       QSpinBox* defaultVolumeSpinBox);
};

#endif // VOLUMEDIALOG_H
