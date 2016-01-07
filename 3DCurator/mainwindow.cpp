#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

	leftRen = vtkSmartPointer<vtkRenderer>::New();

	setBackgroundColor(leftRen, .1, .2, .3); // fondo azul oscuro
	ui->leftWidget->GetRenderWindow()->Render(); // renderiza para ver la ventana del color asignado
    connectComponents(); // conecta los renderers con los widgets

    figura = new Figura(); // crea una instancia de Figura
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b) {
	ren->SetBackground(r, g, b);
}

void MainWindow::connectComponents() {
	ui->leftWidget->GetRenderWindow()->AddRenderer(leftRen); // asigna el renderer donde se visualizará en 3D el volumen al widget izquierdo
}

void MainWindow::drawVolume() {
	leftRen->AddVolume(figura->getVolume()); // añade el volumen al renderer
	leftRen->ResetCamera(); // resetea la cámera
	ui->leftWidget->GetRenderWindow()->Render(); // renderiza
} 

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha leído bien
		figura->setDICOMFolder(dicomFolder.toUtf8().constData());  // carga los archivos DICOM de la carpeta a la figura
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		drawVolume(); // dibuja
	}
}

void MainWindow::on_actionExit_triggered() {
    exit(0);
}
