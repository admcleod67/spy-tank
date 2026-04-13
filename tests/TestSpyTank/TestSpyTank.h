#ifndef TESTSPYTANK_H
#define TESTSPYTANK_H

#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>
#include "SpyTank.h"

class TestSpyTank : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testHandshake();
    void testCommandFormatting();
    void testConnectionError();

private:
    QTcpServer *m_server;
    SpyTank *m_tank;
};

#endif // TESTSPYTANK_H
