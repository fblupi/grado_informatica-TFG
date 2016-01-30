#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

	plot = vtkSmartPointer<vtkXYPlotActor>::New();

	volumeRen = vtkSmartPointer<vtkRenderer>::New();
	histogramRen = vtkSmartPointer<vtkRenderer>::New();

	setBackgroundColor(volumeRen, .1, .2, .3); // fondo azul oscuro
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza para ver la ventana del color asignado
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
	ui->volumeWidget->GetRenderWindow()->AddRenderer(volumeRen); // asigna el renderer donde se visualizará en 3D el volumen al widget izquierdo
	//ui->histogramWidget->GetRenderWindow()->AddRenderer(histogramRen);
}

void MainWindow::drawVolume() {
	volumeRen->AddVolume(figura->getVolume()); // añade el volumen al renderer
	volumeRen->ResetCamera(); // resetea la cámera
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
} 

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha leído bien
		figura->setDICOMFolder(dicomFolder.toUtf8().constData());  // carga los archivos DICOM de la carpeta a la figura
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		
		//plot->AddDataSetInputConnection(figura->getHistogram()->GetOutputPort());

		//histogramRen->AddActor(plot);
		
		drawVolume(); // dibuja
	}
}

void MainWindow::on_updateTF_pressed() {
	figura->removeTFPoints(); // Borra TF

	QObjectList colorList = ui->colorTFContents->children(); // Guarda todos los hijos de colorTFContents (incluyenda las propiedades del layout)
	QObjectList opacityList = ui->opacityTFContents->children(); // Guarda todos los hijos de opacityTFContents (incluyenda las propiedades del layout)

	// Colores
	Q_FOREACH(QObject* obj, colorList) { // Recorre todos los colores
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			if (obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->isChecked()) { // Comprueba si está activado y añade punto
				figura->addRGBPoint(
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->value()
				);
			}
		}
	}

	// Opacidades
	Q_FOREACH(QObject* obj, opacityList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			if (obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->isChecked()) { // Comprueba si está activado y añade punto
				figura->addOpacityPoint(
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->value()
				);
			}
		}
	}

	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
}


void MainWindow::on_actionExit_triggered() {
    exit(0);
}
