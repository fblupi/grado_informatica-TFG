#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "vtkAlgorithm.h"
#include "vtkBoxWidget.h"
#include "vtkColorTransferFunction.h"
#include "vtkDICOMImageReader.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkImageData.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpenDICOM_triggered();
    void on_actionExit_triggered();

    void connectComponents();
    void setDICOMFolder(std::string s);
    void setProperties();
    void setTransferFunction();
    void drawVolume();

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkDICOMImageReader> dicomReader;
    vtkSmartPointer<vtkAlgorithm> reader;
    vtkSmartPointer<vtkVolume> volume;
    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> mapper;
    vtkSmartPointer<vtkColorTransferFunction> colorFun;
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun;
    vtkSmartPointer<vtkVolumeProperty> property;
    vtkSmartPointer<vtkRenderer> ren;
};

#endif // MAINWINDOW_H
