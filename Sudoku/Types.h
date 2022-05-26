#pragma once

#include <QVector>
#include <QPoint>
#include <QLineEdit>

namespace Sudoku {

static constexpr quint8 kLayoutsInLine  = 3;
static constexpr quint8 kCellsInLine    = 9;
static constexpr quint8 kCellsSumInLine = 45;

struct Cell {
    QPoint coord;
    quint8 value;
};

using Field_t = QVector<QVector<Cell>>;
using Cells_t = QVector<QVector<QLineEdit*>>;

} // namespace Sudoku
