#ifndef VIDEORECEIVER_H
#define VIDEORECEIVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QByteArray>

class VideoReceiver : public QObject {
    Q_OBJECT
public:
    explicit VideoReceiver(QObject *parent = nullptr);
    ~VideoReceiver();

    void start(const QString &host, quint16 port = 8196);
    void stop();

    // Testing support
    void processData(const QByteArray &data);

signals:
    void frameReceived(const QPixmap &pixmap);
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onFinished();

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_reply;
    QByteArray m_buffer;

    const QByteArray JPEG_START = QByteArray::fromHex("FFD8");
    const QByteArray JPEG_END = QByteArray::fromHex("FFD9");
};

#endif // VIDEORECEIVER_H
