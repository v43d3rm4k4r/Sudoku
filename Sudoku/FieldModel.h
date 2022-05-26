#pragma once

#include <QObject>
#include <Types.h>

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
    void _swapRowsSmall();
    void _swapColumnsSmall();
    void _swapRowsArea();
    void _swapColumnsArea();

    bool _isSudokuCompleted();

#ifdef DEBUG
    void debugShowField(const QString& functionName);
    void debugMessage(const QString& message);
#endif

private:
    Field_t _field; // QVector2D?
    bool _initialized = false;
};

} // namespace Sudoku
