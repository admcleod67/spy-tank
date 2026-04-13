#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSet>
#include "SpyTank.h"

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

private:
    void setupUi();
    void processMovement();

    SpyTank *m_tank;
    QLineEdit *m_ipEdit;
    QLineEdit *m_portEdit;
    QPushButton *m_connectBtn;
    QLabel *m_statusLabel;

    QSet<int> m_pressedKeys;
};

#endif // MAINWINDOW_H
