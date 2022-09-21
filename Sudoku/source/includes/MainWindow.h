#pragma once

#include <QMainWindow>
#include <./ui_MainWindow.h>
#include <FieldModel.h>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Sudoku {

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

signals:
    void cellChanged(const Sudoku::Cell& cell);
    void cellsHidden();

private slots:
    void onCellChanged(const QString& text);
    void onSudokuCompleted();

private:
    void _initAnimations();
    void _displayField();
    Cells_t _getCells();
    QGridLayout* _getLayoutAtPosition(quint8 row, quint8 col);
    QLineEdit*   _getCellAtPosition(quint8 row, quint8 col);
    void _hideCells(); // TODO: make it more complicate
    void _setTabulation();
    void _resetCellsState();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<FieldModel>     _fieldModel;
    Cells_t                         _cells;
};

} // namespace Sudoku


