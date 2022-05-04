#ifndef APPLETS_H
#define APPLETS_H

#include "../pane.h"

class AppletsPane
{
public:
    void readConfig();
    QWidget* createUI(QWidget* controlCenter);
    void prepareUI(QListWidget* allAppletsListWidget,
                   QListWidget* enabledAppletsListWidget);
    void addEntry(QListWidget* allAppletsListWidget,
                  QListWidget* enabledAppletsListWidget);
    void removeEntry(QListWidget* enabledAppletsListWidget);
    void saveSettings(QListWidget* enabledAppletsListWidget);
};

#endif // APPLETS_H
