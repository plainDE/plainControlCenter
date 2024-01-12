#ifndef WORKSPACESDIALOG_H
#define WORKSPACESDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QRadioButton>
#include <QPushButton>

class WorkspacesDialog : public Dialog {
public:
    WorkspacesDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QRadioButton* useTitlesRadioButton);
};

#endif // WORKSPACESDIALOG_H
