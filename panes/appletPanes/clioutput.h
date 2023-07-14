#ifndef CLIOUTPUTAPPLETPANE_H
#define CLIOUTPUTAPPLETPANE_H

#include "../../settings.h"

#include <QObject>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QScrollArea>
#include <QListWidget>
#include <QListWidgetItem>

struct ConditionLine {
    QHBoxLayout* layout;
    QLineEdit* outputLineEdit;
    QLineEdit* iconLineEdit;
    QLineEdit* textLineEdit;
};

class CLIOutputAppletPane : public QObject {
    Q_OBJECT

public:
    CLIOutputAppletPane(QListWidget* appletsListWidget = nullptr);
    void createUI(Settings* controlCenter);
    void readConfig();
    ~CLIOutputAppletPane();

    QWidget* mCLIOutputAppletPaneWidget;
    QListWidget* mAppletsListWidget;
};

#endif // CLIOUTPUTAPPLETPANE_H
