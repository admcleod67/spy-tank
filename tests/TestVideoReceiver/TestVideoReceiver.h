#ifndef TESTVIDEORECEIVER_H
#define TESTVIDEORECEIVER_H

#include <QtTest>
#include "VideoReceiver.h"

class TestVideoReceiver : public QObject {
    Q_OBJECT

private slots:
    void testFrameExtraction();
    void testPartialData();
    void testInvalidData();
};

#endif // TESTVIDEORECEIVER_H
