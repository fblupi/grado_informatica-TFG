#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "figura.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"

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

	// funcs
    void setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b);
    void connectComponents();
    void drawVolume();

private:
	// atributos
    Ui::MainWindow *ui;
    Figura *figura;
	vtkSmartPointer<vtkRenderer> leftRen;

};

#endif // MAINWINDOW_H
