#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(400, 200);
    w.show();
    return QApplication::exec();
}
