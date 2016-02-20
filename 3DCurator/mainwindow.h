#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "utils.h"
#include "figura.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkImagePlaneWidget.h"
#include "vtkSmartPointer.h"
#include "vtkXYPlotActor.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
	// constructor/destructor
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	// eventos GUI
    void on_actionOpenDICOM_triggered();
    void on_actionExit_triggered();
	void on_updateTF_pressed();
	void on_restoreTF_pressed();
	void on_updateProperties_pressed();

	// funcs
	void renderVolume();
    void setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b);
    void connectComponents();
    void drawVolume();
	void defaultTF();
	void updateTF();
	void updateShadow();
	void defaultPlanePosition();

private:
	// atributos
    Ui::MainWindow *ui;
    Figura *figura;
	vtkSmartPointer<vtkRenderer> volumeRen;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
	vtkSmartPointer<vtkImagePlaneWidget> plane;

	vtkSmartPointer<vtkRenderer> histogramRen;
	vtkSmartPointer<vtkXYPlotActor> plot;

};

#endif // MAINWINDOW_H
