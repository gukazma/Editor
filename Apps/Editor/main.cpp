#include <iostream>
#include <thread>
#include <QApplication>
#include <QSplashScreen>
#include <QFile>
#include <Widgets/MainWindow.h>

int main(int argc, char* argv[])
{
    QApplication qapp(argc, argv);

    QPixmap       pixmap(QString::fromLocal8Bit(":/Images/splashScreen.png"));
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QString::fromLocal8Bit("程序正在加载...."));

     // initialize style
    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

    MainWindow* mainwindow = new MainWindow();
    mainwindow->showMaximized();

    splash.finish(mainwindow);
    return qapp.exec();
}
