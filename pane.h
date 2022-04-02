#ifndef PANE_H
#define PANE_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QFont>
#include <QDir>
#include <QFile>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QFontComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>

namespace Pane {
    void saveConfig(QJsonObject config);
}

#endif
