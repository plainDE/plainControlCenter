#ifndef SOUNDSPANE_H
#define SOUNDSPANE_H

#include "../pane.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

class SoundsPane : public Pane {
public:
    SoundsPane(QJsonObject* cfgObj);
    void setPaneContents() override;
    void prepareToSave(QLineEdit* logInSoundLineEdit,
                       QLineEdit* logOutSoundLineEdit);
};

#endif // SOUNDSPANE_H
