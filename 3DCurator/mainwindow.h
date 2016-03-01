#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "utils.h"
#include "figura.h"
#include "plano.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkImageViewer2.h"
#include "vtkSmartPointer.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkLookupTable.h"

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
	void on_sagitalPlane_pressed();
	void on_coronalPlane_pressed();
	void on_axialPlane_pressed();
	void on_enablePlane_stateChanged();

	// funcs
	void renderVolume();
    void setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b);
    void connectComponents();
    void drawVolume();
	void defaultTF();
	void updateTF();
	void updateShadow();
	void updateMaterial();
	void defaultPlanePosition();

private:
	// atributos
    Ui::MainWindow *ui;
    Figura *figura;
	Plano *plano;
	vtkSmartPointer<vtkRenderer> volumeRen;
	vtkSmartPointer<vtkImageViewer2> sliceViewer;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
};

#endif // MAINWINDOW_H
