#include "pane.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDebug>

void Pane::saveConfig(QJsonObject config) {
    QString homePath = getenv("HOME");
    QString configPath = homePath + "/.config/plainDE/config.json";

    QJsonDocument doc(config);
    QFile jsonFile(configPath);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson(QJsonDocument::Indented));
    jsonFile.close();

    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage request = QDBusMessage::createMethodCall("org.plainDE.plainPanel",
                                                          "/Actions",
                                                          "org.plainDE.actions",
                                                          "reconfigurePanel");
    QDBusMessage response = bus.call(request);

    /* We will show this message if plainPanel is not running at the moment,
     * so user will know everything is ok. */
    if (response.type() == QDBusMessage::ErrorMessage) {
        QMessageBox msg;
        msg.setWindowTitle("Success");
        msg.setText("Settings were saved successfully.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }
}
