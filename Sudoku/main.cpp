#include <MainWindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // TODO: make mainStylesheet.css
//    app.setStyleSheet("QLineEdit { background-color: yellow }");
    app.setStyleSheet("QLineEdit {"
                      "border: 2px solid gray;"
                      "border-radius: 10px;"
                      "padding: 0 8px;"
                      "background: blue;"
                      "selection-background-color: darkgray;"
                  "})");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Sudoku_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    Sudoku::MainWindow w;
    w.show();
    return app.exec();
}
