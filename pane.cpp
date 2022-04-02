#include "pane.h"

void Pane::saveConfig(QJsonObject config) {
    QString homePath = getenv("HOME");
    QString configPath = homePath + "/.config/plainDE/config.json";

    QJsonDocument doc(config);
    QFile jsonFile(configPath);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson(QJsonDocument::Indented));
}
