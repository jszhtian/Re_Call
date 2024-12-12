#include "mainwindow.h"
#include <QApplication>
#include<QSplashScreen>
#include<QElapsedTimer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap scn(":/image/setup.bmp");
    QSplashScreen splash(scn);
    splash.show();
    splash.showMessage(QObject::tr("Initiating your program now..."), Qt::AlignLeft | Qt::AlignTop, Qt::black);
    qApp->processEvents();
    MainWindow w;
    QElapsedTimer s;
    s.start();
    while (s.elapsed()<1500) QCoreApplication::processEvents();
    w.show();
    splash.finish(&w);
    splash.raise();
    return a.exec();
}
