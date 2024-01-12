#ifndef KBINDICATORDIALOG_H
#define KBINDICATORDIALOG_H

#include <dialog.h>

#include <QJsonObject>
#include <QRadioButton>
#include <QPushButton>

class KbIndicatorDialog : public Dialog {
public:
    KbIndicatorDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QRadioButton* flagRadioButton);
};

#endif // KBINDICATORDIALOG_H
