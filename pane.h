#ifndef PANE_H
#define PANE_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QFont>
#include <QDir>
#include <QFile>
#include <QString>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QFontComboBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QScreen>
#include <QGuiApplication>

namespace Pane {
    void saveConfig(QJsonObject config);
}

#endif
