#include "qmessagebox.h"
#include <MainWindow.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[]) {
    //QT_REQUIRE_VERSION(argc, argv, "5.15.2")
    QApplication app(argc, argv);
    app.setApplicationVersion(QString::number(SUDOKU_APP_VERSION));
    app.setApplicationDisplayName(QObject::tr("Sudoku") + " v" + QString::number(SUDOKU_APP_VERSION));
    //app.setWindowIcon(QIcon(":/icons/window_icon@256.ico"));
    //app.setWindowIcon(QIcon(":/icons/window_icon@256.ico"));
    app.setWindowIcon(QIcon(":/icons/window_icon@256.ico"));

    // TODO: make mainStylesheet.css
//    app.setStyleSheet("QLineEdit { background-color: yellow }");
    app.setStyleSheet("QLineEdit {"
                      "border: 2px solid gray;"
                      "border-radius: 10px;"
                      "padding: 0 8px;"
                      "background: mediumslateblue;"
                      "selection-background-color: darkgray;"
                  "}");

    QTranslator translator;
    const auto uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const auto baseName = "Sudoku_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    Sudoku::MainWindow window;
    window.show();
    return app.exec();
}
