#include "pane.h"

void Pane::saveConfig(QJsonObject config) {
    QString homePath = getenv("HOME");
    QString configPath = homePath + "/.config/plainDE/config.json";

    QJsonDocument doc(config);
    QFile jsonFile(configPath);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson(QJsonDocument::Indented));
    jsonFile.close();

    QMessageBox msg;
    msg.setWindowTitle("Restart plainDE");
    msg.setText("Changes will take effect after restarting plainDE");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Information);
    msg.exec();
}
