#include "TestVideoReceiver.h"
#include <QSignalSpy>
#include <QBuffer>
#include <QImage>
#include <QPainter>

QByteArray createTestJpeg() {
    QImage image(10, 10, QImage::Format_RGB32);
    image.fill(Qt::red);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG");
    return ba;
}

void TestVideoReceiver::testFrameExtraction() {
    VideoReceiver receiver;
    QSignalSpy spy(&receiver, &VideoReceiver::frameReceived);

    QByteArray jpegData = createTestJpeg();
    receiver.processData(jpegData);

    QCOMPARE(spy.count(), 1);
}

void TestVideoReceiver::testPartialData() {
    VideoReceiver receiver;
    QSignalSpy spy(&receiver, &VideoReceiver::frameReceived);

    QByteArray jpegData = createTestJpeg();
    int mid = jpegData.length() / 2;

    receiver.processData(jpegData.left(mid));
    QCOMPARE(spy.count(), 0);

    receiver.processData(jpegData.mid(mid));
    QCOMPARE(spy.count(), 1);
}

void TestVideoReceiver::testInvalidData() {
    VideoReceiver receiver;
    QSignalSpy spy(&receiver, &VideoReceiver::frameReceived);

    receiver.processData(QByteArray("not a jpeg"));
    QCOMPARE(spy.count(), 0);

    // After invalid data, it should still work with valid data
    receiver.processData(createTestJpeg());
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestVideoReceiver)
