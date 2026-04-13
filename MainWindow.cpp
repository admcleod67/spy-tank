#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_tank(new SpyTank(this)), m_video(new VideoReceiver(this)) {
    setupUi();
    connect(m_tank, &SpyTank::connected, this, &MainWindow::updateUiState);
    connect(m_tank, &SpyTank::disconnected, this, &MainWindow::updateUiState);
    connect(m_tank, &SpyTank::stateChanged, this, &MainWindow::updateUiState);
    connect(m_tank, &SpyTank::errorOccurred, this, &MainWindow::showError);
    connect(m_video, &VideoReceiver::frameReceived, this, &MainWindow::updateFrame);
    connect(m_video, &VideoReceiver::errorOccurred, this, &MainWindow::showError);

    setWindowTitle("iSpy Tank Remote");
    updateUiState();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Connection row
    QHBoxLayout *connLayout = new QHBoxLayout();
    m_ipEdit = new QLineEdit("10.10.1.1");
    m_portEdit = new QLineEdit("8150");
    m_connectBtn = new QPushButton("Connect");
    connLayout->addWidget(new QLabel("IP:"));
    connLayout->addWidget(m_ipEdit);
    connLayout->addWidget(new QLabel("Port:"));
    connLayout->addWidget(m_portEdit);
    connLayout->addWidget(m_connectBtn);
    layout->addLayout(connLayout);

    connect(m_connectBtn, &QPushButton::clicked, this, &MainWindow::onConnectClicked);

    // Controls Info
    QLabel *info = new QLabel("Use WASD or Arrow Keys to drive. Q/E to tilt camera.");
    info->setAlignment(Qt::AlignCenter);
    layout->addWidget(info);

    // Video Feed
    m_videoLabel = new QLabel("Video Feed (Connect to see)");
    m_videoLabel->setAlignment(Qt::AlignCenter);
    m_videoLabel->setMinimumSize(320, 240);
    m_videoLabel->setStyleSheet("background-color: black; color: white;");
    layout->addWidget(m_videoLabel, 1); // Give it stretch factor

    // Status
    m_statusLabel = new QLabel("Status: Disconnected");
    layout->addWidget(m_statusLabel);

    central->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::onConnectClicked() {
    if (m_tank->state() == QAbstractSocket::UnconnectedState) {
        m_tank->connectToTank(m_ipEdit->text(), m_portEdit->text().toUShort());
    } else {
        m_tank->disconnectFromTank();
    }
}

void MainWindow::updateUiState() {
    QAbstractSocket::SocketState state = m_tank->state();
    switch (state) {
        case QAbstractSocket::UnconnectedState:
            m_statusLabel->setText("Status: Disconnected");
            m_connectBtn->setText("Connect");
            m_ipEdit->setEnabled(true);
            m_portEdit->setEnabled(true);
            m_video->stop();
            m_videoLabel->setPixmap(QPixmap());
            m_videoLabel->setText("Video Feed (Connect to see)");
            break;
        case QAbstractSocket::ConnectingState:
            m_statusLabel->setText("Status: Connecting...");
            m_connectBtn->setText("Cancel");
            break;
        case QAbstractSocket::ConnectedState:
            m_statusLabel->setText("Status: Connected");
            m_connectBtn->setText("Disconnect");
            m_ipEdit->setEnabled(false);
            m_portEdit->setEnabled(false);
            m_video->start(m_ipEdit->text());
            break;
        default:
            m_statusLabel->setText(QString("Status: %1").arg(state));
            break;
    }
}

void MainWindow::showError(const QString &error) {
    QMessageBox::critical(this, "Error", error);
}

void MainWindow::updateFrame(const QPixmap &pixmap) {
    m_videoLabel->setPixmap(pixmap.scaled(m_videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    m_pressedKeys.insert(event->key());
    processMovement();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    m_pressedKeys.remove(event->key());
    processMovement();
}

void MainWindow::processMovement() {
    if (m_tank->state() != QAbstractSocket::ConnectedState) return;

    // Movement logic
    bool forward = m_pressedKeys.contains(Qt::Key_W) || m_pressedKeys.contains(Qt::Key_Up);
    bool back = m_pressedKeys.contains(Qt::Key_S) || m_pressedKeys.contains(Qt::Key_Down);
    bool left = m_pressedKeys.contains(Qt::Key_A) || m_pressedKeys.contains(Qt::Key_Left);
    bool right = m_pressedKeys.contains(Qt::Key_D) || m_pressedKeys.contains(Qt::Key_Right);

    // Camera logic
    bool camUp = m_pressedKeys.contains(Qt::Key_Q);
    bool camDown = m_pressedKeys.contains(Qt::Key_E);

    // Tank movement
    if (forward && !back) {
        if (left && !right) {
            m_tank->sendCommand(SpyTank::Left, SpyTank::Stop);
            m_tank->sendCommand(SpyTank::Right, SpyTank::Forward);
        } else if (right && !left) {
            m_tank->sendCommand(SpyTank::Left, SpyTank::Forward);
            m_tank->sendCommand(SpyTank::Right, SpyTank::Stop);
        } else {
            m_tank->sendCommand(SpyTank::Left, SpyTank::Forward);
            m_tank->sendCommand(SpyTank::Right, SpyTank::Forward);
        }
    } else if (back && !forward) {
        if (left && !right) {
            m_tank->sendCommand(SpyTank::Left, SpyTank::Stop);
            m_tank->sendCommand(SpyTank::Right, SpyTank::Back);
        } else if (right && !left) {
            m_tank->sendCommand(SpyTank::Left, SpyTank::Back);
            m_tank->sendCommand(SpyTank::Right, SpyTank::Stop);
        } else {
            m_tank->sendCommand(SpyTank::Left, SpyTank::Back);
            m_tank->sendCommand(SpyTank::Right, SpyTank::Back);
        }
    } else if (left && !right) {
        m_tank->sendCommand(SpyTank::Left, SpyTank::Back);
        m_tank->sendCommand(SpyTank::Right, SpyTank::Forward);
    } else if (right && !left) {
        m_tank->sendCommand(SpyTank::Left, SpyTank::Forward);
        m_tank->sendCommand(SpyTank::Right, SpyTank::Back);
    } else {
        m_tank->sendCommand(SpyTank::Left, SpyTank::Stop);
        m_tank->sendCommand(SpyTank::Right, SpyTank::Stop);
    }

    // Camera tilt
    if (camUp && !camDown) {
        m_tank->sendCommand(SpyTank::Camera, SpyTank::Forward);
    } else if (camDown && !camUp) {
        m_tank->sendCommand(SpyTank::Camera, SpyTank::Back);
    } else {
        m_tank->sendCommand(SpyTank::Camera, SpyTank::Stop);
    }
}
