#ifndef PANELSPANE_H
#define PANELSPANE_H

#include <pane.h>
#include <QListWidget>
#include <QPushButton>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QHash>
#include <QJsonArray>
#include <QApplication>
#include <QScreen>
#include <QSettings>
#include <QIcon>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

class PanelsPane : public Pane {
public:
    PanelsPane(QJsonObject* cfgObj);
    void setPaneContents();
    void prepareToSave(int panelID,
                       QListWidget* appletsListWidget,
                       QCheckBox* enableAutoHideCheckBox,
                       QSpinBox* autoHideIntervalSpinBox,
                       QSpinBox* thicknessSpinBox,
                       QComboBox* screenComboBox,
                       QCheckBox* stretchCheckBox,
                       QCheckBox* setOnCenterCheckBox,
                       QSpinBox* shiftSpinBox,
                       QRadioButton* topRadioButton,
                       QRadioButton* bottomRadioButton,
                       QRadioButton* leftRadioButton,
                       QRadioButton* rightRadioButton,
                       QSpinBox* launcherIconSizeSpinBox,
                       QLineEdit* bgrImgPathLineEdit,
                       QSpinBox* spacingSpinBox,
                       QSpinBox* marginSpinBox,
                       QDoubleSpinBox* opacitySpinBox);

private:
    void setPanelSettings(int panelID,
                          QListWidget* appletsListWidget,
                          QCheckBox* enableAutoHideCheckBox,
                          QSpinBox* autoHideIntervalSpinBox,
                          QSpinBox* thicknessSpinBox,
                          QComboBox* screenComboBox,
                          QCheckBox* stretchCheckBox,
                          QCheckBox* setOnCenterCheckBox,
                          QSpinBox* shiftSpinBox,
                          QRadioButton* topRadioButton,
                          QRadioButton* bottomRadioButton,
                          QRadioButton* leftRadioButton,
                          QRadioButton* rightRadioButton,
                          QSpinBox* launcherIconSize,
                          QLineEdit* bgrImgPathLineEdit,
                          QSpinBox* spacingSpinBox,
                          QSpinBox* marginSpinBox,
                          QDoubleSpinBox* opacitySpinBox);

    QHash<QString, QString> mAppletHumanNames;
    QHash<QString, QString> mIconNameByApplet;
    QPushButton* mSaveButton;

};

#endif // PANELSPANE_H
