#include "TestSpyTank.h"
#include <QSignalSpy>

void TestSpyTank::init() {
    m_server = new QTcpServer(this);
    m_tank = new SpyTank(this);
    QVERIFY(m_server->listen(QHostAddress::LocalHost));
}

void TestSpyTank::cleanup() {
    delete m_tank;
    delete m_server;
}

void TestSpyTank::testHandshake() {
    m_tank->connectToTank("127.0.0.1", m_server->serverPort());

    QVERIFY(m_server->waitForNewConnection(2000));
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    QVERIFY(clientSocket);

    // Wait for the "t1" handshake
    QTRY_COMPARE_WITH_TIMEOUT(clientSocket->bytesAvailable(), 2, 2000);

    QCOMPARE(clientSocket->readAll(), QByteArray("t1"));
    clientSocket->disconnectFromHost();
    delete clientSocket;
}

void TestSpyTank::testCommandFormatting() {
    m_tank->connectToTank("127.0.0.1", m_server->serverPort());
    QVERIFY(m_server->waitForNewConnection(2000));
    QTcpSocket *clientSocket = m_server->nextPendingConnection();
    QVERIFY(clientSocket);

    // Consume handshake
    QTRY_COMPARE_WITH_TIMEOUT(clientSocket->bytesAvailable(), 2, 2000);
    clientSocket->readAll();

    // Send command
    m_tank->sendCommand(SpyTank::Left, SpyTank::Forward);
    QTRY_COMPARE_WITH_TIMEOUT(clientSocket->bytesAvailable(), 2, 2000);
    QCOMPARE(clientSocket->readAll(), QByteArray("11"));

    m_tank->sendCommand(SpyTank::Right, SpyTank::Back);
    QTRY_COMPARE_WITH_TIMEOUT(clientSocket->bytesAvailable(), 2, 2000);
    QCOMPARE(clientSocket->readAll(), QByteArray("22"));

    m_tank->sendCommand(SpyTank::Camera, SpyTank::Stop);
    QTRY_COMPARE_WITH_TIMEOUT(clientSocket->bytesAvailable(), 2, 2000);
    QCOMPARE(clientSocket->readAll(), QByteArray("30"));

    clientSocket->disconnectFromHost();
    delete clientSocket;
}

void TestSpyTank::testConnectionError() {
    QSignalSpy spy(m_tank, &SpyTank::errorOccurred);
    m_tank->connectToTank("127.0.0.1", 1); // Port 1 is likely closed
    QVERIFY(spy.wait(2000));
}

QTEST_MAIN(TestSpyTank)
