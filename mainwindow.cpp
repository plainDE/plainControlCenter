#include "mainwindow.h"

void MainWindow::readConfig() {
    QString configPath = QDir::homePath() + "/.config/plainDE/config.json";
    if (!QFile::exists(configPath)) {
        qDebug() << "~/.config/plainDE/config.json does not exist. "
                    "Generating new config...";
        system("python3 /usr/share/plainDE/tools/genconfig.py");
    }

    QFile file(configPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = file.readAll();
    file.close();
    mCfgObj = QJsonDocument::fromJson(data.toUtf8()).object();
}

void MainWindow::setAppearance() {
    QIcon::setThemeName("Mint-L-Grey");

    mFont.setFamily(mCfgObj["fontFamily"].toString());
    mFont.setPointSize(mCfgObj["fontSize"].toInt());
    qDebug() << mFont;
    mTitleFont = mFont;
    mTitleFont.setBold(true);
    this->setFont(mFont);

    QString themeName = mCfgObj["theme"].toString();
    QString stylesheetPath = QString("/usr/share/plainDE/styles/%1").arg(themeName);
    QFile stylesheetReader(stylesheetPath);
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    this->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();
}

void MainWindow::createUI(QApplication* app) {
    this->setObjectName("controlCenter");
    this->setWindowTitle("plainControlCenter");
    QString theme = mCfgObj["theme"].toString();

    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);

    QWidget* toolbarWidget = new QWidget();
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbarWidget);
    toolbarLayout->setContentsMargins(5, 5, 5, 5);
    toolbarLayout->setSpacing(6);

    QWidget* buttonsWidget = new QWidget();
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->setSpacing(0);

    QPushButton* backPushButton = new QPushButton("◀");  // U+25C0 - Left-Pointing Triangle
    backPushButton->setMinimumSize(30, 25);
    backPushButton->setMaximumSize(30, 25);
    buttonsLayout->addWidget(backPushButton);

    QPushButton* forwardPushButton = new QPushButton("▶");  // U+25B6 - Right-Pointing Triangle
    forwardPushButton->setMinimumSize(30, 25);
    forwardPushButton->setMaximumSize(30, 25);
    buttonsLayout->addWidget(forwardPushButton);

    toolbarLayout->addWidget(buttonsWidget);

    toolbarLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    QLineEdit* searchBox = new QLineEdit();
    searchBox->setClearButtonEnabled(true);
    searchBox->setPlaceholderText("🔎 " + tr("Search"));  // U+01F50E - magnifier icon

    QHash<QString, QString> dataByEntry;

    QFile searchDBReader("/usr/share/plainDE/pCC-searchdb.json");
    searchDBReader.open(QIODevice::Text | QIODevice::ReadOnly);
    QString searchDB = searchDBReader.readAll();
    searchDBReader.close();
    mSearchDBObj = QJsonDocument::fromJson(searchDB.toUtf8()).object();

    QCompleter* completer = new QCompleter(this);
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setRowCount(mSearchDBObj.count());
    int currentRow = 0;
    foreach (QString entry, mSearchDBObj.keys()) {
        QStringList data = mSearchDBObj[entry].toString().split(';');
        QStandardItem* item = new QStandardItem(entry);
        item->setFont(mFont);
        item->setIcon(QIcon::fromTheme(data[0]));
        model->setItem(currentRow, item);
        ++currentRow;
    }
    completer->setModel(model);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    searchBox->setCompleter(completer);

    connect(completer, SIGNAL(activated(QModelIndex)), this, SLOT(goTo(QModelIndex)));

    toolbarLayout->addWidget(searchBox);

    mMainLayout->addWidget(toolbarWidget);


    QWidget* contentsWidget = new QWidget();
    contentsWidget->setObjectName("contentsWidget");
    QVBoxLayout* contentsLayout = new QVBoxLayout(contentsWidget);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contentsLayout->setSpacing(0);

    QFrame* personalFrame = new QFrame();
    personalFrame->setObjectName("controlCenterFrameA");
    personalFrame->setFont(mFont);
    if (!theme.contains("transparent")) {
        personalFrame->setFrameShape(QFrame::StyledPanel);
        personalFrame->setFrameShadow(QFrame::Raised);
    }
    else {
        personalFrame->setFrameShape(QFrame::NoFrame);
    }
    personalFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout* personalLayout = new QVBoxLayout(personalFrame);

    QLabel* personalLabel = new QLabel(tr("Personal"));
    personalLabel->setFont(mTitleFont);
    personalLayout->addWidget(personalLabel);

    QHBoxLayout* personalButtonsLayout = new QHBoxLayout();
    personalButtonsLayout->setContentsMargins(20, 0, 0, 0);
    personalButtonsLayout->setSpacing(25);

    QToolButton* generalButton = new QToolButton();
    generalButton->setText(tr("General"));
    generalButton->setIcon(QIcon::fromTheme("cs-general"));
    generalButton->setIconSize(QSize(32, 32));
    generalButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    generalButton->setAutoRaise(true);
    generalButton->setMinimumWidth(90);
    generalButton->setMaximumWidth(90);
    personalButtonsLayout->addWidget(generalButton);

    QToolButton* appearanceButton = new QToolButton();
    appearanceButton->setText(tr("Appearance"));
    appearanceButton->setIcon(QIcon::fromTheme("cs-themes"));
    appearanceButton->setIconSize(QSize(32, 32));
    appearanceButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    appearanceButton->setAutoRaise(true);
    appearanceButton->setMinimumWidth(90);
    appearanceButton->setMaximumWidth(90);
    personalButtonsLayout->addWidget(appearanceButton);

    QToolButton* panelsButton = new QToolButton();
    panelsButton->setText(tr("Panels"));
    panelsButton->setIcon(QIcon::fromTheme("cs-applets"));
    panelsButton->setIconSize(QSize(32, 32));
    panelsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    panelsButton->setAutoRaise(true);
    panelsButton->setMinimumWidth(90);
    panelsButton->setMaximumWidth(90);
    personalButtonsLayout->addWidget(panelsButton);

    QToolButton* soundsButton = new QToolButton();
    soundsButton->setText(tr("Sounds"));
    soundsButton->setIcon(QIcon::fromTheme("preferences-system-sound"));
    soundsButton->setIconSize(QSize(32, 32));
    soundsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    soundsButton->setAutoRaise(true);
    soundsButton->setMinimumWidth(90);
    soundsButton->setMaximumWidth(90);
    personalButtonsLayout->addWidget(soundsButton);

    personalButtonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                         QSizePolicy::MinimumExpanding,
                                                         QSizePolicy::Ignored));

    personalLayout->addLayout(personalButtonsLayout);
    contentsLayout->addWidget(personalFrame);


    QFrame* hardwareFrame = new QFrame();
    hardwareFrame->setObjectName("controlCenterFrameB");
    hardwareFrame->setFont(mFont);
    if (!theme.contains("transparent")) {
        hardwareFrame->setFrameShape(QFrame::StyledPanel);
        hardwareFrame->setFrameShadow(QFrame::Raised);
    }
    else {
        hardwareFrame->setFrameShape(QFrame::NoFrame);
    }
    hardwareFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout* hardwareLayout = new QVBoxLayout(hardwareFrame);

    QLabel* hardwareLabel = new QLabel(tr("Hardware"));
    hardwareLabel->setFont(mTitleFont);
    hardwareLayout->addWidget(hardwareLabel);

    QHBoxLayout* hardwareButtonsLayout = new QHBoxLayout(hardwareFrame);
    hardwareButtonsLayout->setContentsMargins(20, 0, 0, 0);
    hardwareButtonsLayout->setSpacing(25);

    QToolButton* keyboardButton = new QToolButton();
    keyboardButton->setText(tr("Keyboard"));
    keyboardButton->setIcon(QIcon::fromTheme("cs-keyboard"));
    keyboardButton->setIconSize(QSize(32, 32));
    keyboardButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    keyboardButton->setAutoRaise(true);
    keyboardButton->setMinimumWidth(90);
    keyboardButton->setMaximumWidth(90);
    hardwareButtonsLayout->addWidget(keyboardButton);

    hardwareButtonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                         QSizePolicy::MinimumExpanding,
                                                         QSizePolicy::Ignored));

    hardwareLayout->addLayout(hardwareButtonsLayout);
    contentsLayout->addWidget(hardwareFrame);


    QFrame* systemFrame = new QFrame();
    systemFrame->setObjectName("controlCenterFrameA");
    systemFrame->setFont(mFont);
    if (!theme.contains("transparent")) {
        systemFrame->setFrameShape(QFrame::StyledPanel);
        systemFrame->setFrameShadow(QFrame::Raised);
    }
    else {
        systemFrame->setFrameShape(QFrame::NoFrame);
    }
    systemFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout* systemLayout = new QVBoxLayout(systemFrame);

    QLabel* systemLabel = new QLabel(tr("System"));
    systemLabel->setFont(mTitleFont);
    systemLayout->addWidget(systemLabel);

    QHBoxLayout* systemButtonsLayout = new QHBoxLayout(systemFrame);
    systemButtonsLayout->setContentsMargins(20, 0, 0, 0);
    systemButtonsLayout->setSpacing(25);

    QToolButton* autostartButton = new QToolButton();
    autostartButton->setText(tr("Autostart"));
    autostartButton->setIcon(QIcon::fromTheme("cs-general"));
    autostartButton->setIconSize(QSize(32, 32));
    autostartButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    autostartButton->setAutoRaise(true);
    autostartButton->setMinimumWidth(90);
    autostartButton->setMaximumWidth(90);
    systemButtonsLayout->addWidget(autostartButton);

    QToolButton* aboutButton = new QToolButton();
    aboutButton->setText(tr("About") + "\nplainDE");
    aboutButton->setIcon(QIcon("/usr/share/plainDE/menuIcon.png"));
    aboutButton->setIconSize(QSize(32, 32));
    aboutButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    aboutButton->setAutoRaise(true);
    aboutButton->setMinimumWidth(90);
    aboutButton->setMaximumWidth(90);
    systemButtonsLayout->addWidget(aboutButton);

    systemButtonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                       QSizePolicy::MinimumExpanding,
                                                       QSizePolicy::Ignored));

    systemLayout->addLayout(systemButtonsLayout);
    contentsLayout->addWidget(systemFrame);


    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));

    mMainLayout->addWidget(contentsWidget);


    QScreen* primaryScreen = app->primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();

    int width = 525, height = 650;
    int x = screenGeometry.x() + ((screenGeometry.width() - width) / 2);
    int y = screenGeometry.y() + ((screenGeometry.height() - height) / 2);
    this->setGeometry(x, y, width, height);
    this->setFixedSize(width, height);

    mCurrentWidget = contentsWidget;
    mHistory.append(mCurrentWidget);

    // Making connections
    connect(generalButton, &QToolButton::clicked, this, [this]() {
        showPane("general");
    });

    connect(appearanceButton, &QToolButton::clicked, this, [this]() {
        showPane("appearance");
    });

    connect(keyboardButton, &QToolButton::clicked, this, [this]() {
        showPane("keyboard");
    });

    connect(autostartButton, &QToolButton::clicked, this, [this]() {
        showPane("autostart");
    });

    connect(panelsButton, &QToolButton::clicked, this, [this]() {
        showPane("panels");
    });

    connect(soundsButton, &QToolButton::clicked, this, [this]() {
        showPane("sounds");
    });

    connect(aboutButton, &QPushButton::clicked, this, [this]() {
        QProcess* process = new QProcess(this);
        process->start("plainAbout", {"--plainControlCenter"});
    });

    connect(backPushButton, &QPushButton::clicked, this, [this]() {
        if (mHistoryPointer > 0) {
            mCurrentWidget->setVisible(false);
            mCurrentWidget = mHistory.at(--mHistoryPointer);
            mCurrentWidget->setVisible(true);
        }
        qDebug() << mHistory << mHistoryPointer;
    });

    connect(forwardPushButton, &QPushButton::clicked, this, [this]() {
        if (mHistoryPointer < mHistory.length() - 1) {
            mCurrentWidget->setVisible(false);
            mCurrentWidget = mHistory.at(++mHistoryPointer);
            mCurrentWidget->setVisible(true);
        }
        qDebug() << mHistory << mHistoryPointer;
    });

    connect(searchBox, &QLineEdit::returnPressed, this, [this, searchBox]() {
        goTo(searchBox->text());
    });
}


void MainWindow::showPane(QString name) {
    mCurrentWidget->setVisible(false);

    int length = mHistory.length();
    for (int i = 0; i < length - mHistoryPointer - 1; ++i) {
        mHistory.removeLast();
    }

    Pane* pane;
    if (!name.compare("general")) {
        pane = new GeneralPane(&mCfgObj);
    }
    else if (!name.compare("appearance")) {
        pane = new AppearancePane(&mCfgObj, this);
    }
    else if (!name.compare("panels")) {
        pane = new PanelsPane(&mCfgObj);
    }
    else if (!name.compare("keyboard")) {
        pane = new KeyboardPane(&mCfgObj);
    }
    else if (!name.compare("autostart")) {
        pane = new AutostartPane(&mCfgObj);
    }
    else if (!name.compare("sounds")) {
        pane = new SoundsPane(&mCfgObj);
    }

    pane->setPaneContents();
    mMainLayout->addWidget(pane);
    mHistory.append(pane);
    mCurrentWidget = pane;
    ++mHistoryPointer;
}

void MainWindow::showDialog(QString name) {
    Dialog* dialog;
    if (!name.compare("usermenu")) {
        dialog = new UserMenuDialog(&mCfgObj);
    }
    else if (!name.compare("datetime")) {
        dialog = new DateTimeDialog(&mCfgObj);
    }
    else if (!name.compare("volume")) {
        dialog = new VolumeDialog(&mCfgObj);
    }
    else if (!name.compare("appmenu")) {
        dialog = new AppMenuDialog(&mCfgObj);
    }
    else if (!name.compare("localipv4")) {
        dialog = new LocalIPv4Dialog(&mCfgObj);
    }
    else if (!name.compare("windowlist")) {
        dialog = new WindowListDialog(&mCfgObj);
    }
    else if (!name.compare("kblayout")) {
        dialog = new KbIndicatorDialog(&mCfgObj);
    }
    else if (!name.compare("workspaces")) {
        dialog = new WorkspacesDialog(&mCfgObj);
    }
    else if (!name.compare("windowlist")) {
        dialog = new WindowListDialog(&mCfgObj);
    }
    dialog->setPaneContents();
    dialog->show();
}

void MainWindow::goTo(QString entry) {
    if (mSearchDBObj.contains(entry)) {
        QStringList path = mSearchDBObj[entry].toString().split(';')[1].split("->");

        // Leg 1
        showPane(path.at(0));

        // Leg 2
        if (path.length() > 1) {
            if (!path.at(1).compare("panelX")) {
                // Leg 3
                QString applet = path.at(2);
                int panelID = 1;
                while (panelID <= 4) {
                    QString panelName = QString("panel%1").arg(QString::number(panelID));
                    QJsonArray applets = mCfgObj[panelName].toObject()["applets"].toArray();
                    if (applets.contains(applet)) {
                        break;
                    }
                    ++panelID;
                }
                showDialog(applet);
            }
        }
    }
}

void MainWindow::goTo(QModelIndex index) {
    QString entry = index.data().toString();
    goTo(entry);
}

void MainWindow::setTransparency() {
    QScreen* screen = this->screen();
    QRect widgetGeometry = this->geometry();
    QPixmap pixmap = screen->grabWindow(0,
                                        widgetGeometry.x(),
                                        widgetGeometry.y(),
                                        widgetGeometry.width(),
                                        widgetGeometry.height());
    QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect();
    blurEffect->setBlurRadius(15);
    blurEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem item;
    item.setPixmap(pixmap);
    item.setGraphicsEffect(blurEffect);
    scene->addItem(&item);
    QImage res(QSize(widgetGeometry.width(), widgetGeometry.height()), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene->render(&ptr, QRectF(), QRectF(0, 0, widgetGeometry.width(), widgetGeometry.height()));

    QPalette palette;
    palette.setBrush(this->backgroundRole(),
                     QBrush(QPixmap::fromImage(res)));
    this->setPalette(palette);
}

MainWindow::MainWindow(QWidget *parent,
                       QApplication* app) : QWidget(parent) {
    readConfig();
    setAppearance();
    createUI(app);
    if (mCfgObj["theme"].toString().contains("transparent")) {
        setTransparency();
    }
}

MainWindow::~MainWindow() {

}
