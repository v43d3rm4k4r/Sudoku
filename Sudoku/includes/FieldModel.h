#pragma once

#include <QObject>
#include "Types.h"

namespace Sudoku {

class FieldModel final : public QObject {
    Q_OBJECT
public:
    explicit FieldModel(QObject* parent = nullptr);
    Field_t  field() const { return _field; }

public slots:
    void onCellChanged(const Sudoku::Cell& cell);
    void onCellsHidden();

signals:
    void sudokuCompleted();

private:
    void _init();
    void _shuffle();

    void _transpose();
    void _swapRowsSmall();    // TODO: add another version
    void _swapColumnsSmall(); // TODO: add another version
    void _swapRowsArea();     // TODO: add another version
    void _swapColumnsArea();  // TODO: add another version

    bool _isSudokuCompleted();

    void debugShowField(const QString& functionName);
    void debugMessage(const QString& message);

private:
    Field_t _field;
    bool    _initialized = false;
};

} // namespace Sudoku
