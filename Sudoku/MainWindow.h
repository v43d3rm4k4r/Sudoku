#pragma once

#include <QMainWindow>
#include "./ui_MainWindow.h"
#include <FieldModel.h>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Sudoku {

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private:
    void _displayField();
    Cells_t _getCells();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<FieldModel>     _fieldModel;
    Cells_t                         _cells;
};

} // namespace Sudoku


