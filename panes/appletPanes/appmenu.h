#ifndef APPMENU_H
#define APPMENU_H

#include "../../pane.h"

struct App {
    QString displayedName;
    QIcon icon;
    bool display;
};

enum LineEditType {
    Text,
    Icon
};


class AppMenuAppletPane {
public:
    void readConfig();
    QWidget* createUI();
    void prepareUI(QListWidget* allAppsListWidget,
                   QListWidget* favAppsListWidget);
    void setCurrentSettings(QLineEdit* buttonTextLineEdit,
                            QLineEdit* buttonIconLineEdit,
                            QPushButton* previewButton,
                            QCheckBox* useTriangularTabsCheckBox);
    void saveSettings(QLineEdit* buttonTextLineEdit,
                      QLineEdit* buttonIconLineEdit,
                      QListWidget* favAppsListWidget,
                      QCheckBox* useTriangularTabsCheckBox);
    void makePreview(QPushButton* previewButton,
                     QLineEdit* lineEdit,
                     LineEditType lineEditType);
    void addEntry(QListWidget* allAppsListWidget,
                  QListWidget* favAppsListWidget);
    void removeEntry(QListWidget* favAppsListWidget);
};

#endif // APPMENU_H
