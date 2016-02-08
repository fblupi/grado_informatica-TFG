#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	defaultTF();

	plot = vtkSmartPointer<vtkXYPlotActor>::New();

	volumeRen = vtkSmartPointer<vtkRenderer>::New();
	histogramRen = vtkSmartPointer<vtkRenderer>::New();

	setBackgroundColor(volumeRen, .1, .2, .3); // fondo azul oscuro
	renderVolume();
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
	renderVolume();
}

void MainWindow::defaultTF() {
	QObjectList colorList = ui->colorTFContents->children(); // Guarda todos los hijos de colorTFContents (incluyenda las propiedades del layout)
	QObjectList opacityList = ui->opacityTFContents->children(); // Guarda todos los hijos de opacityTFContents (incluyenda las propiedades del layout)

	// Colores
	Q_FOREACH(QObject* obj, colorList) { // Recorre todos los colores
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			switch (atoi(id.c_str())) {
				case 1:
					obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->setValue(-750);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->setValue(0.08);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->setValue(0.05);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->setValue(0.03);
					break;
				case 2:
					obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->setValue(-350);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->setValue(0.39);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->setValue(0.25);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->setValue(0.16);
					break;
				case 3:
					obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->setValue(-200);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->setValue(0.8);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->setValue(0.8);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->setValue(0.8);
					break;
				case 4:
					obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->setValue(3000);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->setValue(0.5);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->setValue(0.5);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->setValue(0.5);
					break;
				default:
					obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->setChecked(false);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->setValue(0);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->setValue(0);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->setValue(0);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->setValue(0);
					break;
			}
		}
	}

	// Opacidades
	Q_FOREACH(QObject* obj, opacityList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			switch (atoi(id.c_str())) {
				case 1:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(-800);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
				case 2:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(-750);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(1);
					break;
				case 3:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(-350);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(1);
					break;
				case 4:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(-300);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
				case 5:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(-200);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
				case 6:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(-100);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(1);
					break;
				case 7:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(1000);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
				case 8:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(2750);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
				case 9:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(2976);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(1);
					break;
				case 10:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(3000);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
				default:
					obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->setChecked(false);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->setValue(0);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->setValue(0);
					break;
			}
		}
	}
}

void MainWindow::updateTF() {
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
}

void MainWindow::updateShadow() {
	if (ui->enableShadow->isChecked()) {
		figura->enableShadow();
	} else {
		figura->disableShadow();
	}
}

void MainWindow::renderVolume() {
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
}

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha leído bien
		figura->setDICOMFolder(dicomFolder.toUtf8().constData());  // carga los archivos DICOM de la carpeta a la figura
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		updateTF(); // actualiza función de transferencia con los valores de la GUI
		updateShadow(); // actualiza sombreado
		
		//plot->AddDataSetInputConnection(figura->getHistogram()->GetOutputPort());

		//histogramRen->AddActor(plot);
		
		drawVolume(); // dibuja
	}
}

void MainWindow::on_updateTF_pressed() {
	updateTF();
	renderVolume();
}

void MainWindow::on_restoreTF_pressed() {
	defaultTF();
	updateTF();
	renderVolume();
}

void MainWindow::on_actionExit_triggered() {
    exit(0);
}

void MainWindow::on_updateProperties_pressed() {
	updateShadow();
	renderVolume();
}