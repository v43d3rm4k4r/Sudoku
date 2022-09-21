#include <CellEventFilter.h>
#include <QEvent>
#include <QPropertyAnimation>
#include <QRectF>
#include <QLineEdit>

namespace Sudoku {

CellEventFilter::CellEventFilter(QObject* targetObject) : _targetObject{targetObject} {
    _propertyAnimation = new QPropertyAnimation(targetObject, "geometry");
    _propertyAnimation->setDuration(75);
    _propertyAnimation->setEasingCurve(QEasingCurve::Linear);
}

bool CellEventFilter::eventFilter(QObject* object, QEvent* event) {
    if (object->objectName().contains("lineEdit") && (event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverLeave)) {
        auto widget = qobject_cast<QLineEdit*>(object);
        widget->raise();
        auto firstPosition = widget->geometry();
        auto endPosition   = widget->geometry();
        endPosition.setWidth(endPosition.width()+10);
        endPosition.setHeight(endPosition.height()+10);
        if (event->type() == QEvent::HoverEnter) {
            _propertyAnimation->setStartValue(QRect(firstPosition.left(), firstPosition.top(), firstPosition.width(), firstPosition.height()));
            _propertyAnimation->setEndValue(endPosition);
        } else if (event->type() == QEvent::HoverLeave) {
            _propertyAnimation->setStartValue(endPosition);
            _propertyAnimation->setEndValue(QRect(firstPosition.left(), firstPosition.top(), 56, 53));
        }
        _propertyAnimation->start();
        return true;
    }
    return QObject::eventFilter(object, event);
}

} // namespace Sudoku
