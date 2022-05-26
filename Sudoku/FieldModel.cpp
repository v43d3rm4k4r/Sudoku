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
    // TODO: add check if sudoku completed? or do it by button?
}

void FieldModel::_init() {
    _field.resize(9);
    for (auto& col : _field)
        col.resize(9);

//    // Compile time - our everything :)
//    _field = {{1,2,3,4,5,6,7,8,9},
//              {4,5,6,7,8,9,1,2,3},
//              {7,8,9,1,2,3,4,5,6},
//              {2,3,4,5,6,7,8,9,1},
//              {5,6,7,8,9,1,2,3,4},
//              {8,9,1,2,3,4,5,6,7},
//              {3,4,5,6,7,8,9,1,2},
//              {6,7,8,9,1,2,3,4,5},
//              {9,1,2,3,4,5,6,7,8}};

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

    // TODO: randomly call shuffle methods
#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_transpose() {


#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_swapRowsSmall() {


#ifdef DEBUG
    debugShowField(FUNC);
#endif
}

void FieldModel::_swapColumnsSmall() {


#ifdef DEBUG
    debugShowField(FUNC);
#endif
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
#endif

} // namespace Sudoku
