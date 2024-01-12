#include "pane.h"

QJsonValue Pane::getConfigValue(QString entry) {
    return mCfgObj->operator[](entry);
}

QJsonValue Pane::getConfigValue(QString entry, QString subentry) {
    return mCfgObj->operator[](entry).toObject()[subentry];
}

void Pane::setAppearance(QWidget* widget) {
    // Stylesheet
    QString themeName = getConfigValue("theme").toString();
    QString stylesheetPath = QString("/usr/share/plainDE/styles/%1").arg(themeName);
    QFile stylesheetReader(stylesheetPath);
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    widget->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    // Font
    mFont.setFamily(getConfigValue("fontFamily").toString());
    mFont.setPointSize(getConfigValue("fontSize").toInt());
    mTitleFont = mFont;
    mTitleFont.setBold(true);
    widget->setFont(mFont);
}

void Pane::setBasicUI(QString paneName, QString iconName) {
    QHBoxLayout* paneTitleLayout = new QHBoxLayout();
    paneTitleLayout->setContentsMargins(1, 1, 1, 1);

    QLabel* iconLabel = new QLabel();
    iconLabel->setPixmap(QIcon::fromTheme(iconName).pixmap(32, 32));
    paneTitleLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel(paneName);
    titleLabel->setFont(mTitleFont);
    paneTitleLayout->addWidget(titleLabel);

    paneTitleLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                   QSizePolicy::MinimumExpanding,
                                                   QSizePolicy::Ignored));

    mLayout->addLayout(paneTitleLayout);

    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    contentsLayout->setContentsMargins(10, 2, 2, 2);
    setAppearance(mContentsWidget);
}

void Pane::finalizeUI() {
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(mContentsWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mLayout->addWidget(scrollArea);
}

void Pane::setEntry(QString key, QJsonValue value) {
    mCfgObj->operator[](key) = value;
}

void Pane::saveConfig() {
    QString configPath = QDir::homePath() + "/.config/plainDE/config.json";
    QJsonDocument doc(*mCfgObj);
    QFile file(configPath);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    QDBusConnection bus = QDBusConnection::sessionBus();
    QDBusMessage req = QDBusMessage::createMethodCall("org.plainDE.plainPanel",
                                                      "/Actions",
                                                      "org.plainDE.actions",
                                                      "reconfigurePanel");
    QDBusMessage resp = bus.call(req);

    // plainPanel crashed or is not running at the moment
    if (resp.type() == QDBusMessage::ErrorMessage) {
        QMessageBox msg;
        msg.setWindowTitle(tr("Settings saved"));
        msg.setText(tr("Settings were saved. Unable to reconfigure plainPanel. "
                       "Restart it manually."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
}

void Pane::setPaneContents() {

}

Pane::Pane(QWidget* parent,
           QJsonObject* cfgObj,
           QString paneName,
           QString iconName) : QWidget(parent) {
    mCfgObj = cfgObj;

    QIcon::setThemeName(getConfigValue("iconTheme").toString());

    mLayout = new QVBoxLayout(this);
    mLayout->setContentsMargins(4, 4, 4, 4);

    setAppearance(this);
    setBasicUI(paneName, iconName);
}
