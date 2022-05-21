#pragma once

#include <QVector>
#include <QPoint>
#include <QLineEdit>

namespace Sudoku {

struct Cell {
    QPoint coord;
    quint8 value;
};

using Field_t = QVector<QVector<Cell>>;
using Cells_t = QVector<QVector<QLineEdit*>>;

} // namespace Sudoku
