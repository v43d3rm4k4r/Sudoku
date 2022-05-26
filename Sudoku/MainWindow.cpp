#include <MainWindow.h>
#include <QRandomGenerator>
#include <QMessageBox>

/// Line edit cells & grids naming:
/// "lineEdit21", where first number is for X, second is for Y

namespace Sudoku {

static constexpr auto kEmptyCellStyleSheet = "color: white;";

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui = std::make_unique<Ui::MainWindow>();
    ui->setupUi(this);

    _fieldModel = std::make_unique<FieldModel>();
    // TODO: first field init, via FieldModel instance init
    _cells = _getCells();

    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            connect(_getCellAtPosition(row, col), &QLineEdit::textChanged, this, &Sudoku::MainWindow::onCellChanged);
        }
    }
    connect(this, &Sudoku::MainWindow::cellChanged, _fieldModel.get(), &Sudoku::FieldModel::onCellChanged);
    connect(this, &Sudoku::MainWindow::cellsHidden, _fieldModel.get(),  &Sudoku::FieldModel::onCellsHidden);
    connect(_fieldModel.get(), &Sudoku::FieldModel::sudokuCompleted, this, &Sudoku::MainWindow::onSudokuCompleted);

    _displayField();
    _hideCells();
    _setTabulation();
}

void MainWindow::onCellChanged(const QString& text) {
    auto sender = qobject_cast<QLineEdit*>(this->sender());
    auto coords = sender->objectName().mid(8, 2);
    if (text.isEmpty()) {
        emit cellChanged({{QString(coords[0]).toInt(), QString(coords[1]).toInt()}, 0});
        return;
    }
    if (!text.toInt()) {
        sender->clear();
        return;
    }
    emit cellChanged({{QString(coords[0]).toInt(), QString(coords[1]).toInt()}, static_cast<quint8>(text.toInt())});
}

void MainWindow::onSudokuCompleted() {
    QMessageBox msgBox;
    msgBox.setText(tr("Sudoku completed successfully!"));
    msgBox.exec();
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

//    quint8 rowCell = 0, colCell = 0;
//    for (quint8 outerGridRow = 0; outerGridRow != 6; outerGridRow += 2) {
//        for (quint8 outerGridCol = 0; outerGridCol != 6; outerGridCol += 2) {
//            auto layout = qobject_cast<QGridLayout*>(ui->gridLayoutCentral->itemAtPosition(outerGridRow, outerGridCol)->layout());
//            if (layout) {


//                for (quint8 innerGridRow = rowCell; innerGridRow < rowCell+3; ++innerGridRow) {     // 0->3->6 в счётчике, 3->6->9 в условии
//                    for (quint8 innerGridCol = colCell; innerGridCol < colCell+3; ++innerGridCol) { // 0->3->6 в счётчике, 3->6->9 в условии
//                        auto widget = layout->itemAtPosition(innerGridRow, innerGridCol);
//                        if (widget) {
//                            cells[innerGridRow][innerGridCol] = widget;
//                        }
//                    }
//                    colCell += 3;
//                }
//                // обнулить новые переменные
//                rowCell += 3;
//            }
//        }
//    }

    for (quint8 row = 0; row < 9; ++row) {
        for (quint8 col = 0; col < 9; ++col) {
            cells[row][col] = _getCellAtPosition(row, col);
        }
    }

    return cells;
}

QGridLayout* MainWindow::_getLayoutAtPosition(quint8 row, quint8 col) {
    auto layout = ui->gridLayoutCentral->findChild<QGridLayout*>("gridLayout" + QString::number(row) + QString::number(col));
    return layout;
}

QLineEdit* MainWindow::_getCellAtPosition(quint8 row, quint8 col) {
    QLineEdit*   cell   = nullptr;
//    QGridLayout* layout = nullptr;

//    if (row <= 2 && col <= 2) {
//        layout = _getLayoutAtPosition(0, 0);
//    }
//    if (row <= 2 && col >= 3 && col <= 5) {
//        layout = _getLayoutAtPosition(0, 1);
//    }
//    if (row <= 2 && col >= 6 && col <= 8) {
//        layout = _getLayoutAtPosition(0, 2);
//    }
//    if (col <= 2 && row >= 3 && row <= 5) {
//        layout = _getLayoutAtPosition(1, 0);
//    }
//    if (col <= 2 && row >= 6 && row <= 8) {
//        layout = _getLayoutAtPosition(2, 0);
//    }
//    if (row >= 3 && col >= 3 && row <= 5 && col <= 5) {
//        layout = _getLayoutAtPosition(1, 1);
//    }
//    if (col >= 6 && row >= 3 && row <= 5) {
//        layout = _getLayoutAtPosition(1, 2);
//    }
//    if (row >= 6 && col >= 3 && col <= 5) {
//        layout = _getLayoutAtPosition(2, 1);
//    }
//    if (row >= 6 && col >= 6) {
//        layout = _getLayoutAtPosition(2, 2);
//    }

    cell = ui->centralwidget->findChild<QLineEdit*>("lineEdit" + QString::number(row) + QString::number(col));
    return cell;
}

void MainWindow::_hideCells() {
    // 0.Проходимся по всем лейаутам и удаляем по одной ячейке
    for (quint8 row = 0; row < kLayoutsInLine; ++row) {
        for (quint8 col = 0; col < kLayoutsInLine; ++col) {
            auto layout = _getLayoutAtPosition(row, col);
            auto randRow = QRandomGenerator::global()->bounded(0, kLayoutsInLine);
            auto randCol = QRandomGenerator::global()->bounded(0, kLayoutsInLine);
            auto widget = qobject_cast<QLineEdit*>(layout->itemAtPosition(randRow, randCol)->widget());
            widget->clear(); // model edits automatically
            widget->setStyleSheet(kEmptyCellStyleSheet);
        }
    }

    // 1.Проходимся по столбцам и удаляем по одной ячейке
    for (quint8 col = 0; col < kCellsInLine; ++col) {
        quint8 rowToDelete = 0;
        QLineEdit* widget  = nullptr;
        do {
            rowToDelete = QRandomGenerator::global()->bounded(0, kLayoutsInLine);
            widget      = _getCellAtPosition(rowToDelete, col);
        } while (widget->text().isEmpty());
        widget->clear();
        widget->setStyleSheet(kEmptyCellStyleSheet);
    }


    // 2.Проходимся по строкам и удаляем по одной ячейке
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        quint8 colToDelete = 0;
        QLineEdit* widget  = nullptr;
        do {
            colToDelete = QRandomGenerator::global()->bounded(0, kLayoutsInLine);
            widget      = _getCellAtPosition(row, colToDelete);
        } while (widget->text().isEmpty());
        widget->clear();
        widget->setStyleSheet(kEmptyCellStyleSheet);
    }

    // 3.Неспрятанные значения делаем read only
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            if (!_cells[row][col]->text().isEmpty()) {
                _cells[row][col]->setReadOnly(true);
            }
        }
    }
    emit cellsHidden();
}

void MainWindow::_setTabulation() {
    // TODO
}

// TODO: add value validation. Only if value is valid, then emit cellChanged()

} // namespace Sudoku


