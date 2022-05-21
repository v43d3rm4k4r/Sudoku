#include <MainWindow.h>

/// Line edit cells & grids naming:
/// "lineEdit21", where first number is for X, second is for Y

namespace Sudoku {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = std::make_unique<Ui::MainWindow>();
    ui->setupUi(this);

    _fieldModel = std::make_unique<FieldModel>();
    // TODO: first field init, via FieldModel instance init
    _cells = _getCells();
    _displayField();
}

void MainWindow::_displayField() {
    for (quint8 row = 0; row < 9; ++row) {
        for (quint8 col = 0; col < 9; ++col) {
            _cells[row][col]->setText(QString::number(_fieldModel->field()[row][col].value));
        }
    }
}

Cells_t MainWindow::_getCells() {
    Cells_t cells;
    cells.resize(9);
    for (auto& col : cells)
        col.resize(9);

    for (quint8 outerGridRow = 0; outerGridRow != 6; outerGridRow += 2) {
        for (quint8 outerGridCol = 0; outerGridRow != 6; outerGridRow += 2) {
            auto layout = qobject_cast<QGridLayout*>(ui->gridLayoutCentral->itemAtPosition(outerGridRow, outerGridCol)->layout());
            if (layout) {
                for (quint8 innerGridRow = 0; innerGridRow < 3; ++innerGridRow) {
                    for (quint8 innerGridCol = 0; innerGridCol < 3; ++innerGridCol) {
                        cells[innerGridRow][outerGridCol] = qobject_cast<QLineEdit*>(layout->itemAtPosition(innerGridRow, innerGridCol)->widget());
                    }
                }
            }
        }
    }
    return cells;
}

// TODO: add value validation. Only if value is valid, then emit cellChanged()

} // namespace Sudoku


