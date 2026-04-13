#include "VideoReceiver.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QBuffer>
#include <QImageReader>

VideoReceiver::VideoReceiver(QObject *parent) : QObject(parent), m_networkManager(new QNetworkAccessManager(this)), m_reply(nullptr) {}

VideoReceiver::~VideoReceiver() {
    stop();
}

void VideoReceiver::start(const QString &host, quint16 port) {
    stop();
    m_buffer.clear();

    QUrl url(QString("http://%1:%2").arg(host).arg(port));
    QNetworkRequest request(url);
    m_reply = m_networkManager->get(request);

    connect(m_reply, &QNetworkReply::readyRead, this, &VideoReceiver::onReadyRead);
    connect(m_reply, &QNetworkReply::finished, this, &VideoReceiver::onFinished);
}

void VideoReceiver::stop() {
    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void VideoReceiver::onReadyRead() {
    m_buffer.append(m_reply->readAll());

    while (true) {
        int startPos = m_buffer.indexOf(JPEG_START);
        if (startPos == -1) {
            m_buffer.clear();
            return;
        }

        if (startPos > 0) {
            m_buffer.remove(0, startPos);
            startPos = 0;
        }

        int endPos = m_buffer.indexOf(JPEG_END);
        if (endPos == -1) return;

        int frameLength = endPos - startPos + JPEG_END.length();
        QByteArray frameData = m_buffer.mid(0, frameLength);
        m_buffer.remove(0, frameLength);

        QPixmap pixmap;
        if (pixmap.loadFromData(frameData, "JPEG")) {
            emit frameReceived(pixmap);
        }
    }
}

void VideoReceiver::onFinished() {
    if (m_reply->error() != QNetworkReply::OperationCanceledError) {
        emit errorOccurred(m_reply->errorString());
    }
}
