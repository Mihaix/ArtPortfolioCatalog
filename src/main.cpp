#include "ui/mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application information
    QApplication::setApplicationName("Art Portfolio Catalog");

    // Load an application style sheet (optional)
    // QFile styleFile(":/styles/style.qss");
    // if (styleFile.open(QFile::ReadOnly)) {
    //     QString style(styleFile.readAll());
    //     a.setStyleSheet(style);
    //     styleFile.close();
    // }

    MainWindow w;
    w.show();

    return a.exec();
}
