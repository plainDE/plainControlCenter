#ifndef AUTOSTARTPANE_H
#define AUTOSTARTPANE_H

#include "../pane.h"

struct App {
    QString displayedName;
    QIcon icon;
    bool display;
};

class AutostartPane
{
public:
    void readConfig();
    QWidget* createUI(QWidget* controlCenter);
    void prepareUI(QListWidget* autostartEntriesListWidget,
                   QListWidget* allAppsListWidget);
    void addEntry(QListWidget* autostartEntriesListWidget,
                  QListWidget* allAppsListWidget);
    void removeEntry(QListWidget* autostartEntriesListWidget);
    void saveSettings(QListWidget* autostartEntriesListWidget);
};

#endif // AUTOSTARTPANE_H
