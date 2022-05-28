#include <FieldModel.h>
#ifdef DEBUG
#   include <iostream>
#endif

#if defined (__linux__) && defined (__GNUC__)
#define FUNC __PRETTY_FUNCTION__
#elif defined _WIN32
#define FUNC __func__
#endif

namespace Sudoku {

FieldModel::FieldModel(QObject* parent/*=nullptr*/) : QObject(parent) {
    _init();
    _shuffle();
}

void FieldModel::onCellChanged(const Cell &cell) {
    _field[cell.coord.x()][cell.coord.y()].value = cell.value;

    debugShowField(FUNC);

    if (_initialized && _isSudokuCompleted()) {
        debugMessage("Sudoku completed!");
        emit sudokuCompleted();
    }
}

void FieldModel::onCellsHidden() {
    _initialized = true;
}

void FieldModel::_init() {
    _field.resize(9);
    for (auto& col : _field)
        col.resize(9);

    quint8 globalGridNum = 1;
    for (quint8 colStart = 0; colStart < 9; colStart += 3) { // global 3x3
        for (quint8 rowStart = 0; rowStart < 9; rowStart += 3) {
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

#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_shuffle() {
    _transpose();

    // TODO: randomly call shuffle methods
#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_transpose() {
    Field_t transposed;
    transposed.resize(9);
    for (auto& col : transposed)
        col.resize(9);

    for (quint8 col = 0; col < kCellsInLine; ++col) {
        for (quint8 row = 0; row < kCellsInLine; ++row) {
            transposed[col][row] = _field[row][col];
        }
    }
    _field = transposed;

#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_swapRowsSmall() {
    Field_t transposed;
    transposed.resize(9);
    for (auto& col : transposed)
        col.resize(9);

    // TODO

#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_swapColumnsSmall() {


#ifdef DEBUG
    debugShowField(FUNC);
#endif
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

#ifdef DEBUG
void FieldModel::debugShowField(const QString& functionName) {
    std::cout << "Field after " << functionName.toStdString() << ':' << std::endl;
    for (quint8 row = 0; row < 9; ++row) {
        for (quint8 col = 0; col < 9; ++col) {
            std::cout << static_cast<int>(_field[row][col].value) << ' ';
            if (col == 2 || col == 5) {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
        if (row == 2 || row == 5) {
            for (int i = 0; i < 9; ++i) {
                std::cout << ' ';
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void FieldModel::debugMessage(const QString &message) {
    std::cout << message.toStdString() << std::endl;
}
#endif

} // namespace Sudoku
