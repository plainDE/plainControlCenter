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

    bool mAppearanceWidgetVisible;
    bool mAutostartWidgetVisible;
    bool mPanelsWidgetVisible;
    bool mKeyboardWidgetVisible;
    bool mLayoutDgVisible;
};

#endif // SETTINGS_H
