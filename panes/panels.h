#ifndef PANELS_H
#define PANELS_H

#include "../pane.h"
#include "../settings.h"

namespace Ui {
class PanelsPane;
}

class PanelsPane : public QWidget {
    Q_OBJECT

public:
    explicit PanelsPane(QWidget *parent = nullptr, Settings* controlCenter = nullptr);
    void readConfig();
    void setCurrentSettings(qint8 panelNumber);
    void saveSettings(qint8 panelNumber);
    ~PanelsPane();

private:
    Ui::PanelsPane *ui;
};

#endif // PANELS_H
