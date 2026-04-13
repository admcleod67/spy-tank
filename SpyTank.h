#ifndef SPYTANK_H
#define SPYTANK_H

#include <QObject>
#include <QTcpSocket>

class SpyTank : public QObject {
    Q_OBJECT
public:
    enum Motor { Left = 1, Right = 2, Camera = 3 };
    enum Direction { Stop = 0, Forward = 1, Back = 2 };

    explicit SpyTank(QObject *parent = nullptr);
    ~SpyTank();

    void connectToTank(const QString &host, quint16 port);
    void disconnectFromTank();
    void sendCommand(Motor motor, Direction dir);

    QAbstractSocket::SocketState state() const;

signals:
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);
    void stateChanged(QAbstractSocket::SocketState socketState);

private slots:
    void onConnected();
    void onSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *m_socket;
};

#endif // SPYTANK_H
