#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSet>
#include "SpyTank.h"
#include "VideoReceiver.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void onConnectClicked();
    void updateUiState();
    void showError(const QString &error);
    void updateFrame(const QPixmap &pixmap);

private:
    void setupUi();
    void processMovement();

    SpyTank *m_tank;
    VideoReceiver *m_video;
    QLineEdit *m_ipEdit;
    QLineEdit *m_portEdit;
    QPushButton *m_connectBtn;
    QLabel *m_statusLabel;
    QLabel *m_videoLabel;

    QSet<int> m_pressedKeys;
};

#endif // MAINWINDOW_H
