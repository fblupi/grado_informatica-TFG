#include "ReadDICOMQt.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ReadDICOMQt w;
    w.show();

    return a.exec();
}
