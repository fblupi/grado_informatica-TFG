#ifndef READDICOMQT_H
#define READDICOMQT_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageViewer.h>

namespace Ui {
    class ReadDICOMQt;
}

class ReadDICOMQt : public QMainWindow {
    Q_OBJECT

public:
    explicit ReadDICOMQt(QWidget *parent = 0);
    ~ReadDICOMQt();

private slots:
    void openDICOMFile();
    void drawDICOMImage(std::string fileDICOM);
    void on_buttonOpenFile_clicked();

private:
    Ui::ReadDICOMQt *ui;
    vtkSmartPointer<vtkDICOMImageReader> reader;
    vtkSmartPointer<vtkImageViewer> viewer;
};

#endif // READDICOMQT_H
