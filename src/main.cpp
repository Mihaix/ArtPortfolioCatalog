#include "ui/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    qputenv("QT_LEAK_CHECK", "1");
    qputenv("QT_ENABLE_LEAK_CHECK", "1");
    qputenv("QML_DISABLE_DISK_CACHE", "1");
    qputenv("QSG_RENDER_LOOP", "basic");

    QApplication a(argc, argv);

    QApplication::setApplicationName("Art Portfolio Catalog");

    QString appPath = QCoreApplication::applicationFilePath();
    QFileInfo appInfo(appPath);
    QString testPath = appInfo.absolutePath() + "/../tests/debug/tests.exe";

    qDebug() << "Running tests from:" << testPath;
    QProcess testProcess;
    testProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    testProcess.start(testPath);
    testProcess.waitForFinished(-1);

    int testResult = testProcess.exitCode();
    if (testResult != 0) {
        qCritical() << "Tests failed with code:" << testResult;
        return testResult;
    } else {
        qDebug() << "All tests passed!";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
