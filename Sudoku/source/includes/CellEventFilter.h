#pragma once

#include <QObject>
#include <QPropertyAnimation>

namespace Sudoku {

class CellEventFilter final : public QObject {
    Q_OBJECT
public:
    explicit CellEventFilter(QObject* targetObject);

protected:
    bool eventFilter(QObject* object, QEvent* event) override;

private:
    QObject* _targetObject = nullptr;
    QPropertyAnimation* _propertyAnimation = nullptr;
};

} // namespace Sudoku
