#include "SpyTank.h"
#include <QDebug>

SpyTank::SpyTank(QObject *parent) : QObject(parent), m_socket(new QTcpSocket(this)) {
    connect(m_socket, &QTcpSocket::connected, this, &SpyTank::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &SpyTank::disconnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &SpyTank::onSocketError);
    connect(m_socket, &QTcpSocket::stateChanged, this, &SpyTank::stateChanged);
}

SpyTank::~SpyTank() {
    disconnectFromTank();
}

void SpyTank::connectToTank(const QString &host, quint16 port) {
    if (m_socket->state() == QAbstractSocket::UnconnectedState) {
        m_socket->connectToHost(host, port);
    }
}

void SpyTank::disconnectFromTank() {
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
    }
}

void SpyTank::sendCommand(Motor motor, Direction dir) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        QString cmd = QString("%1%2").arg(static_cast<int>(motor)).arg(static_cast<int>(dir));
        m_socket->write(cmd.toLatin1());
        qDebug() << "Sent command:" << cmd;
    }
}

QAbstractSocket::SocketState SpyTank::state() const {
    return m_socket->state();
}

void SpyTank::onConnected() {
    m_socket->write("t1");
    qDebug() << "Connected and handshake sent.";
    emit connected();
}

void SpyTank::onSocketError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    emit errorOccurred(m_socket->errorString());
}
