#ifndef LAYOUTDIALOG_H
#define LAYOUTDIALOG_H

#include "keyboard.h"
#include "../../pane.h"


class LayoutDialog : public QWidget {
public:
    void readConfig();
    void prepareUI();
    QWidget* createUI(KeyboardPane* kbPane,
                      QListWidget* layoutsListWidget);

    LayoutDialog(QWidget* parent = nullptr);

    virtual ~LayoutDialog();
};

#endif // LAYOUTDIALOG_H
