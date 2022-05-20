#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui = std::make_unique<Ui::MainWindow>();
    ui->setupUi(this);
}

