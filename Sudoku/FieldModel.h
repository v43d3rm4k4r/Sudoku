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

private:
    void _init();
    void _shuffle();

    void _transpose();
    void _swapRowsSmall();
    void _swapColumnsSmall();
    void _swapRowsArea();
    void _swapColumnsArea();

#ifdef DEBUG
    void debugShowField(const QString& functionName);
#endif

private:
    Field_t _field; // QVector2D?
};

} // namespace Sudoku
