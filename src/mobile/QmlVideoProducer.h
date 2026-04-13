#ifndef QMLVIDEOPRODUCER_H
#define QMLVIDEOPRODUCER_H

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
#include <QPixmap>
#include <QVideoFrameFormat>

class QmlVideoProducer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)

public:
    explicit QmlVideoProducer(QObject *parent = nullptr) : QObject(parent), m_videoSink(nullptr) {}

    QVideoSink* videoSink() const { return m_videoSink; }
    void setVideoSink(QVideoSink *sink) {
        if (m_videoSink == sink) return;
        m_videoSink = sink;
        emit videoSinkChanged();
    }

public slots:
    void handleFrame(const QPixmap &pixmap) {
        if (!m_videoSink) return;

        QImage img = pixmap.toImage();
        if (img.isNull()) return;

        // Ensure the image format is compatible with QVideoFrame
        if (img.format() != QImage::Format_RGBA8888 && img.format() != QImage::Format_RGB32) {
            img = img.convertToFormat(QImage::Format_RGBA8888);
        }

        QVideoFrame frame(img);
        m_videoSink->setVideoFrame(frame);
    }

signals:
    void videoSinkChanged();

private:
    QVideoSink *m_videoSink;
};

#endif // QMLVIDEOPRODUCER_H
