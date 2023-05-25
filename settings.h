#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class Settings : public QWidget {
    Q_OBJECT

public:
    Settings(QWidget* parent = nullptr);
    void updateStyle();
    void readConfig();
    void createUI();
    ~Settings();

    bool mAppearanceWidgetVisible = false;
    bool mAutostartWidgetVisible = false;
    bool mPanelsWidgetVisible = false;
    bool mKeyboardWidgetVisible = false;
    bool mLayoutDgVisible = false;
    bool mAppMenuWidgetVisible = false;
    bool mDateTimeWidgetVisible = false;
    bool mLocalIPv4WidgetVisible = false;
    bool mWinListWidgetVisible = false;
    bool mUserMenuWidgetVisible = false;
};

#endif // SETTINGS_H
