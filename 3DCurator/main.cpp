#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    
	QApplication a(argc, argv);

	a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
