#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "vtkAlgorithm.h"
#include "vtkBoxWidget.h"
#include "vtkColorTransferFunction.h"
#include "vtkDICOMImageReader.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkImageData.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkImageViewer2.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	// ui events
    void on_actionOpenDICOM_triggered();
    void on_actionExit_triggered();
	void on_slices_sliderMoved(int pos);

	// funcs
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
	vtkSmartPointer<vtkSmartVolumeMapper> smartMapper;
    vtkSmartPointer<vtkColorTransferFunction> colorFun;
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun;
    vtkSmartPointer<vtkVolumeProperty> property;
	vtkSmartPointer<vtkRenderer> leftRen;
	vtkSmartPointer<vtkImageViewer2> viewer;

};

#endif // MAINWINDOW_H
