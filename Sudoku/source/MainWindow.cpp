#include <MainWindow.h>
#include <QRandomGenerator>
#include <QMessageBox>
#include <CellEventFilter.h>

namespace Sudoku {

static constexpr auto kDefaultCellStyleSheet = "color: black;";
static constexpr auto kEmptyCellStyleSheet   = "color: white;";

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent} {
    ui = std::make_unique<Ui::MainWindow>();
    ui->setupUi(this);

    _fieldModel = std::make_unique<FieldModel>();
    _cells = _getCells();

    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            connect(_getCellAtPosition(row, col), &QLineEdit::textChanged, this, &Sudoku::MainWindow::onCellChanged);
        }
    }
    connect(this, &Sudoku::MainWindow::cellChanged, _fieldModel.get(), &Sudoku::FieldModel::onCellChanged);
    connect(this, &Sudoku::MainWindow::cellsHidden, _fieldModel.get(), &Sudoku::FieldModel::onCellsHidden);
    connect(_fieldModel.get(), &Sudoku::FieldModel::sudokuCompleted, this, &Sudoku::MainWindow::onSudokuCompleted);

    setFixedSize(minimumWidth(), minimumHeight());

    // TODO _initAnimations
    _initAnimations();

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
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Sudoku completed successfully!"));
    msgBox.exec();

    _displayField();
    _resetCellsState();
    _hideCells();
    _setTabulation();
}

void MainWindow::_initAnimations() {
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            auto widget = _getCellAtPosition(row, col);
            widget->installEventFilter(new CellEventFilter(widget));
        }
    }
}

void MainWindow::_displayField() {
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            _cells[row][col]->setText(QString::number(_fieldModel->field()[row][col].value));
        }
    }
}

Cells_t MainWindow::_getCells() {
    Cells_t cells;
    cells.resize(kCellsInLine);
    for (auto& col : cells)
        col.resize(kCellsInLine);

    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
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
    QLineEdit* cell = nullptr;
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

    // 3.Убираем доп. ячейки если в каких то зонах убрана всего одна
    for (quint8 row = 0; row < kLayoutsInLine; ++row) {
        for (quint8 col = 0; col < kLayoutsInLine; ++col) {
            auto layout = _getLayoutAtPosition(row, col);
            auto emptyCellsCount = 0;
            QList<QPoint> emptyCellsList;
            for (quint8 cellRow = 0; cellRow < kLayoutsInLine; ++cellRow) {
                for (quint8 cellCol = 0; cellCol < kLayoutsInLine; ++cellCol) {
                    auto widget = qobject_cast<QLineEdit*>(layout->itemAtPosition(cellRow, cellCol)->widget());
                    if (widget->text().isEmpty()) {
                        ++emptyCellsCount;
                        emptyCellsList.push_back({cellRow, cellCol});
                    }
                }
            }
            if (emptyCellsCount < 2) {
                quint8 randRow = 0, randCol = 0;
                do {
                    randRow = QRandomGenerator::global()->bounded(0, kLayoutsInLine);
                    randCol = QRandomGenerator::global()->bounded(0, kLayoutsInLine);
                } while (emptyCellsList.contains({randRow, randCol}));
                auto widget = qobject_cast<QLineEdit*>(layout->itemAtPosition(randRow, randCol)->widget());
                widget->clear();
                widget->setStyleSheet(kEmptyCellStyleSheet);
            }
        }
    }

    // 4.Неспрятанные значения делаем read only
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
    bool isFirst = true;
    QLineEdit* oldWidget   = nullptr;
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            auto widget = _getCellAtPosition(row, col);
            if (widget->text().isEmpty()) {
                if (!isFirst) {
                    setTabOrder(oldWidget, widget);
                }
                oldWidget = widget;
                if (isFirst) {
                    widget->setFocus();
                    isFirst = false;
                }
            }
        }
    }
}

void MainWindow::_resetCellsState() {
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            auto widget = _getCellAtPosition(row, col);
            widget->setStyleSheet(kDefaultCellStyleSheet);
            if (widget->isReadOnly()) {
                widget->setReadOnly(false);
            }
        }
    }
}

} // namespace Sudoku


