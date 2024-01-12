#ifndef GENERALPANE_H
#define GENERALPANE_H

#include "../pane.h"

#include <QCheckBox>
#include <QPushButton>
#include <QPixmap>
#include <QSpinBox>

class GeneralPane : public Pane {
public:
    GeneralPane(QJsonObject* cfgObj);
    void setPaneContents();
    void prepareToSave(QCheckBox* enableAnimationCheckBox,
                       QCheckBox* useTriangularTabsCheckBox);
};

#endif // GENERALPANE_H
