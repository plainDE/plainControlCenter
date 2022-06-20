#ifndef LAYOUTDIALOG_H
#define LAYOUTDIALOG_H

#include "keyboard.h"
#include "../../pane.h"


class LayoutDialog {
public:
    void readConfig();
    void prepareUI();
    QWidget* createUI(KeyboardPane* kbPane,
                      QListWidget* layoutsListWidget);

    virtual ~LayoutDialog();
};

#endif // LAYOUTDIALOG_H
