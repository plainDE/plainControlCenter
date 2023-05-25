#ifndef PANELS_H
#define PANELS_H

#include "../pane.h"
#include "../settings.h"

#include "appletPanes/appmenu.h"
#include "appletPanes/datetime.h"
#include "appletPanes/localipv4.h"
#include "appletPanes/winlist.h"
#include "appletPanes/usermenu.h"

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

    AppMenuAppletPane* mAppMenuAppletPane;
    DatetimeAppletPane* mDatetimeAppletPane;
    LocalIPv4AppletPane* mLocalIPv4AppletPane;
    WinListAppletPane* mWinListAppletPane;
    UserMenuAppletPane* mUserMenuAppletPane;


private:
    Ui::PanelsPane *ui;
};

#endif // PANELS_H
