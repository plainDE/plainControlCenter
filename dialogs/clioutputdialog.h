#ifndef CLIOUTPUTDIALOG_H
#define CLIOUTPUTDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QListWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

struct Condition {
    int i;
    QLabel* ifLabel;
    QLineEdit* condLineEdit;
    QLabel* thenLabel;
    QLineEdit* iconLineEdit;
    QLineEdit* textLineEdit;
    QPushButton* rmButton;
};

class CLIOutputDialog : public Dialog {
public:
    CLIOutputDialog(QJsonObject* cfgObj,
                    int panelID,
                    bool newApplet,
                    int item,
                    QListWidget* appletsListWidget);
    ~CLIOutputDialog();
    virtual void setPaneContents() override;
    void readConfig();
    void newCondition(QBoxLayout* innerConditionsLayout, QString ifValue, QString thenIcon, QString thenText);
    void prepareToSave(QLineEdit* appletNameLineEdit,
              QRadioButton* stdoutRadioButton,
              QRadioButton* conditionRadioButton,
              QLineEdit* commandLineEdit,
              QSpinBox* intervalSpinBox,
              QRadioButton* stdoutCondRadioButton,
              QLineEdit* elseIconLineEdit,
              QLineEdit* elseTextLineEdit,
              QLineEdit* waitIconLineEdit,
              QLineEdit* waitTextLineEdit);

private:
    QList<Condition*> mConditionList;
    QListWidget* mAppletsListWidget;
    int mItem;
    bool mNewApplet;
    QJsonObject mAppletObj;
};

#endif // CLIOUTPUTDIALOG_H
