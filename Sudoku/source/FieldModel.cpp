#include <FieldModel.h>
#include <QRandomGenerator>

#ifdef DEBUG
#   include <iostream>
#endif

#if defined (__linux__) && defined (__GNUC__)
#   define FUNC __PRETTY_FUNCTION__
#elif defined _WIN32
#   define FUNC __func__
#endif

namespace Sudoku {

FieldModel::FieldModel(QObject* parent/*=nullptr*/) : QObject{parent} {
    _init();
    _shuffle();
}

void FieldModel::onCellChanged(const Cell &cell) {
    _field[cell.coord.x()][cell.coord.y()].value = cell.value;

    debugShowField(FUNC);

    if (_initialized && _isSudokuCompleted()) {
        debugMessage("Sudoku completed!");
        _initialized = false;
        _init();
        _shuffle();
        emit sudokuCompleted();
    }
}

void FieldModel::onCellsHidden() {
    _initialized = true;
}

void FieldModel::_init() {
    _field.resize(kCellsInLine);
    for (auto& col : _field)
        col.resize(kCellsInLine);

    quint8 globalGridNum = 1;
    for (quint8 colStart = 0; colStart < kCellsInLine; colStart += 3) { // global 3x3
        for (quint8 rowStart = 0; rowStart < kCellsInLine; rowStart += 3) {
            quint8 valueCounter = globalGridNum;
            for (quint8 row = rowStart; row < rowStart+3; ++row) { // local 3x3
                for (quint8 col = colStart; col < colStart+3; ++col) {
                    _field[row][col].value = valueCounter;
                    if (++valueCounter == 10) {
                        valueCounter = 1;
                    }
                }
            }
            ++globalGridNum;
        }
    }

    debugShowField(FUNC);
}

void FieldModel::_shuffle() {
    QVector<void(Sudoku::FieldModel::*)()> shuffleMethods;
    shuffleMethods.push_back(&Sudoku::FieldModel::_transpose);
    shuffleMethods.push_back(&Sudoku::FieldModel::_swapRowsSmall);
    shuffleMethods.push_back(&Sudoku::FieldModel::_swapColumnsSmall);
    shuffleMethods.push_back(&Sudoku::FieldModel::_swapRowsArea);
    shuffleMethods.push_back(&Sudoku::FieldModel::_swapColumnsArea);

    for ([[maybe_unused]] const auto& current : shuffleMethods) {
        auto methodNum = QRandomGenerator::global()->bounded(0, shuffleMethods.size());
        std::invoke(shuffleMethods[methodNum], this);
        shuffleMethods.remove(methodNum, 1);
    }

    debugShowField(FUNC);
}

void FieldModel::_transpose() {
    Field_t transposed;
    transposed.resize(kCellsInLine);
    for (auto& col : transposed)
        col.resize(kCellsInLine);

    for (quint8 col = 0; col < kCellsInLine; ++col) {
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            transposed[col][row] = _field[row][col];
        }
    }
    _field = transposed;

    debugShowField(FUNC);
}

void FieldModel::_swapRowsSmall() {
    Field_t swapped;
    swapped.resize(kCellsInLine);
    for (auto& col : swapped)
        col.resize(kCellsInLine);

    bool firstRowOfArea = false;
    bool thirdRowOfArea = false;
    for (quint8 fieldRow = 0, swappedRow = 0; fieldRow < kCellsInLine; ++fieldRow, ++swappedRow) {
        if (fieldRow % 3 == 0) {
            fieldRow += 2;
            firstRowOfArea = true;
        }
        if (!firstRowOfArea && (fieldRow == 2 || fieldRow == 5 || fieldRow == 8)) {
            fieldRow -= 2;
            thirdRowOfArea = true;
        }
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            swapped[swappedRow][col] = _field[fieldRow][col];
        }
        if (firstRowOfArea) {
            fieldRow -= 2;
            firstRowOfArea = false;
            continue;
        }
        if (thirdRowOfArea) {
            fieldRow += 2;
            thirdRowOfArea = false;
        }
    }
    _field = swapped;

    debugShowField(FUNC);
}

void FieldModel::_swapColumnsSmall() {
    Field_t swapped;
    swapped.resize(kCellsInLine);
    for (auto& col : swapped)
        col.resize(kCellsInLine);

    bool firstColOfArea = false;
    bool thirdColOfArea = false;
    for (quint8 fieldCol = 0, swappedCol = 0; fieldCol < kCellsInLine; ++fieldCol, ++swappedCol) {
        if (fieldCol % 3 == 0) {
            fieldCol += 2;
            firstColOfArea = true;
        }
        if (!firstColOfArea && (fieldCol == 2 || fieldCol == 5 || fieldCol == 8)) {
            fieldCol -= 2;
            thirdColOfArea = true;
        }
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            swapped[row][swappedCol] = _field[row][fieldCol];
        }
        if (firstColOfArea) {
            fieldCol -= 2;
            firstColOfArea = false;
            continue;
        }
        if (thirdColOfArea) {
            fieldCol += 2;
            thirdColOfArea = false;
        }
    }
    _field = swapped;

    debugShowField(FUNC);
}

void FieldModel::_swapRowsArea() {
    Field_t swapped;
    swapped.resize(kCellsInLine);
    for (auto& col : swapped)
        col.resize(kCellsInLine);

    for (quint8 fieldRow = 3, swappedRow = 0; fieldRow < 6; ++fieldRow, ++swappedRow) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            swapped[swappedRow][col] = _field[fieldRow][col];
        }
    }
    for (quint8 fieldRow = 0, swappedRow = 3; fieldRow < 4; ++fieldRow, ++swappedRow) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            swapped[swappedRow][col] = _field[fieldRow][col];
        }
    }
    for (quint8 row = 6; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            swapped[row][col] = _field[row][col];
        }
    }

    _field = swapped;

    debugShowField(FUNC);
}

void FieldModel::_swapColumnsArea() { /// !!!
    Field_t swapped;
    swapped.resize(kCellsInLine);
    for (auto& col : swapped)
        col.resize(kCellsInLine);

    for (quint8 fieldCol = 3, swappedCol = 0; fieldCol < 6; ++fieldCol, ++swappedCol) {
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            swapped[row][swappedCol] = _field[row][fieldCol];
        }
    }
    for (quint8 fieldCol = 0, swappedCol = 3; fieldCol < 4; ++fieldCol, ++swappedCol) {
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            swapped[row][swappedCol] = _field[row][fieldCol];
        }
    }
    for (quint8 col = 6; col < kCellsInLine; ++col) {
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            swapped[row][col] = _field[row][col];
        }
    }

    _field = swapped;

    debugShowField(FUNC);
}

bool FieldModel::_isSudokuCompleted() {
    quint8 sum = 0;
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            sum += _field[row][col].value;
        }
        if (sum != kCellsSumInLine) {
            return false;
        }
        sum = 0;
    }
    sum = 0;
    for (quint8 col = 0; col < kCellsInLine; ++col) {
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            sum += _field[row][col].value;
        }
        if (sum != kCellsSumInLine) {
            return false;
        }
        sum = 0;
    }
    return true;
}

void FieldModel::debugShowField(const QString& functionName) {
#ifdef DEBUG
    std::cout << "Field after " << functionName.toStdString() << ':' << std::endl;
    for (quint8 row = 0; row < kCellsInLine; ++row) {
        for (quint8 col = 0; col < kCellsInLine; ++col) {
            std::cout << static_cast<int>(_field[row][col].value) << ' ';
            if (col == 2 || col == 5) {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
        if (row == 2 || row == 5) {
            for (int i = 0; i < kCellsInLine; ++i) {
                std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
#endif
}

void FieldModel::debugMessage(const QString &message) {
#ifdef DEBUG
    std::cout << message.toStdString() << std::endl;
#endif
}

} // namespace Sudoku
