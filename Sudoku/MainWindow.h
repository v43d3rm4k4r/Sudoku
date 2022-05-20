#pragma once

#include <QMainWindow>
#include "./ui_MainWindow.h"

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private:
    std::unique_ptr<Ui::MainWindow> ui;
};
