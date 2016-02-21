#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	defaultTF();

	volumeRen = vtkSmartPointer<vtkRenderer>::New();
	slicesRen = vtkSmartPointer<vtkRenderer>::New();
	sliceViewer = vtkSmartPointer<vtkImageViewer2>::New();
	style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

	plano = new Plano(); // crea una instancia de Plano
	figura = new Figura(); // crea una instancia de Figura

	setBackgroundColor(volumeRen, .1, .2, .3); // fondo azul oscuro
    connectComponents(); // conecta los renderers con los widgets

	renderVolume();

	plano->enable(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b) {
	ren->SetBackground(r, g, b);
}

void MainWindow::connectComponents() {
	ui->volumeWidget->GetRenderWindow()->AddRenderer(volumeRen); // asigna el renderer donde se visualizar� en 3D el volumen al widget izquierdo
	ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
	plano->getPlane()->SetInteractor(ui->volumeWidget->GetRenderWindow()->GetInteractor());

	sliceViewer->SetInputData(plano->getPlane()->GetResliceOutput());
	ui->slicesWidget->SetRenderWindow(sliceViewer->GetRenderWindow());
	sliceViewer->SetupInteractor(ui->slicesWidget->GetInteractor());
}

void MainWindow::drawVolume() {
	volumeRen->AddVolume(figura->getVolume()); // a�ade el volumen al renderer
	volumeRen->ResetCamera(); // resetea la c�mera
	renderVolume();
}

void MainWindow::defaultTF() {
	QObjectList colorList = ui->colorTFContents->children(); // Guarda todos los hijos de colorTFContents (incluyenda las propiedades del layout)
	QObjectList opacityList = ui->opacityTFContents->children(); // Guarda todos los hijos de opacityTFContents (incluyenda las propiedades del layout)
	QObjectList gradientList = ui->gradientTFContents->children(); // Guarda todos los hijos de opacityTFContents (incluyenda las propiedades del layout)

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

	// Opacidades escalares
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

	// Opacidades gradientes
	Q_FOREACH(QObject* obj, gradientList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->setChecked(false);
			obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientValueP_" + id)).c_str()))->setValue(0);
			obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientAP_" + id)).c_str()))->setValue(0);
		}
	}
}

void MainWindow::updateTF() {
	figura->removeTFPoints(); // Borra TF

	QObjectList colorList = ui->colorTFContents->children(); // Guarda todos los hijos de colorTFContents (incluyenda las propiedades del layout)
	QObjectList opacityList = ui->opacityTFContents->children(); // Guarda todos los hijos de opacityTFContents (incluyenda las propiedades del layout)
	QObjectList gradientList = ui->gradientTFContents->children(); // Guarda todos los hijos de opacityTFContents (incluyenda las propiedades del layout)

	// Colores
	Q_FOREACH(QObject* obj, colorList) { // Recorre todos los colores
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			if (obj->findChild<QCheckBox *>(QString((std::string("colorEnableP_" + id)).c_str()))->isChecked()) { // Comprueba si est� activado y a�ade punto
				figura->addRGBPoint(
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorValueP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorRP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorGP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("colorBP_" + id)).c_str()))->value()
					);
			}
		}
	}

	// Opacidades escalares
	Q_FOREACH(QObject* obj, opacityList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			if (obj->findChild<QCheckBox *>(QString((std::string("opacityEnableP_" + id)).c_str()))->isChecked()) { // Comprueba si est� activado y a�ade punto
				figura->addOpacityPoint(
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityValueP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("opacityAP_" + id)).c_str()))->value()
					);
			}
		}
	}

	// Opacidades gradientes
	Q_FOREACH(QObject* obj, gradientList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			if (obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->isChecked()) { // Comprueba si est� activado y a�ade punto
				figura->addGradientPoint(
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientValueP_" + id)).c_str()))->value(),
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientAP_" + id)).c_str()))->value()
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

void MainWindow::defaultPlanePosition() {
	if (figura->getVolume() != NULL) {
		double xSize = figura->getMaxXBound() - figura->getMinXBound(), ySize = figura->getMaxYBound() - figura->getMinYBound(), zSize = figura->getMaxZBound() - figura->getMinZBound();
		plano->setOrigin(xSize / 2, ySize / 2, zSize / 2); // Coloca el centro del plano en el centro de la figura
		plano->placeWidget(figura->getMinXBound(), figura->getMaxXBound(), figura->getMinYBound(), figura->getMaxYBound(), figura->getMinZBound(), figura->getMaxZBound()); // Ajusta el tama�o del plano al de la figura
	}
}

void MainWindow::renderVolume() {
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
}

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha le�do bien
		figura->setDICOMFolder(dicomFolder.toUtf8().constData());  // carga los archivos DICOM de la carpeta a la figura
		plano->setInputConnection(figura->getReader());
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		updateTF(); // actualiza funci�n de transferencia con los valores de la GUI
		updateShadow(); // actualiza sombreado
		defaultPlanePosition(); // coloca el plano en una posici�n inicial
		drawVolume(); // dibuja
		sliceViewer->Render();
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