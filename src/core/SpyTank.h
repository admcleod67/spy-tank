#ifndef SPYTANK_H
#define SPYTANK_H

#include <QObject>
#include <QTcpSocket>

class SpyTank : public QObject {
    Q_OBJECT
    Q_PROPERTY(QAbstractSocket::SocketState state READ state NOTIFY stateChanged)
public:
    enum Motor { Left = 1, Right = 2, Camera = 3 };
    Q_ENUM(Motor)
    enum Direction { Stop = 0, Forward = 1, Back = 2 };
    Q_ENUM(Direction)

    explicit SpyTank(QObject *parent = nullptr);
    ~SpyTank();

    Q_INVOKABLE void connectToTank(const QString &host, quint16 port);
    Q_INVOKABLE void disconnectFromTank();
    Q_INVOKABLE void sendCommand(Motor motor, Direction dir);

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
