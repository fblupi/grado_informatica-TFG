#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	defaultTF();
	defaultMaterial();

	volumeRen = vtkSmartPointer<vtkRenderer>::New();
	sliceViewer = vtkSmartPointer<vtkImageViewer2>::New();
	style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	plano = new Plano(); // crea una instancia de Plano
	figura = new Figura(); // crea una instancia de Figura
/*
	sliceViewer->GetWindowLevel()->SetOutputFormatToRGB();
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetHueRange(0.66667, 0.0);
	lut->SetSaturationRange(1, 1);
	lut->SetValueRange(1, 1);
	lut->SetAlphaRange(1, 1);
	lut->SetNumberOfColors(256);
	lut->Build();
	sliceViewer->GetWindowLevel()->SetLookupTable(lut);
*/
	sliceViewer->SetColorLevel(-200);
	sliceViewer->SetColorWindow(1000);
	setBackgroundColor(volumeRen, .1, .2, .3); // fondo azul oscuro

    connectComponents(); // conecta los renderers con los widgets

	renderVolume();

	plano->show(false);
	plano->enable(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b) {
	ren->SetBackground(r, g, b);
}

void MainWindow::connectComponents() {
	ui->volumeWidget->GetRenderWindow()->AddRenderer(volumeRen); // asigna el renderer donde se visualizará en 3D el volumen al widget izquierdo
	ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
	plano->getPlane()->SetInteractor(ui->volumeWidget->GetRenderWindow()->GetInteractor());

	sliceViewer->SetInputData(plano->getPlane()->GetResliceOutput());
	ui->slicesWidget->SetRenderWindow(sliceViewer->GetRenderWindow());
	sliceViewer->SetupInteractor(ui->slicesWidget->GetInteractor());

	plano->setViewer(sliceViewer);
}

void MainWindow::drawVolume() {
	volumeRen->AddViewProp(figura->getVolume()); // añade el volumen al renderer
	volumeRen->ResetCamera(); // resetea la cámera
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
			}
		}
	}

	// Opacidades gradientes
	Q_FOREACH(QObject* obj, gradientList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			switch (atoi(id.c_str())) {
				case 1:
					obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientValueP_" + id)).c_str()))->setValue(-1000);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientAP_" + id)).c_str()))->setValue(0.0);
					break;
				case 2:
					obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientValueP_" + id)).c_str()))->setValue(90);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientAP_" + id)).c_str()))->setValue(0.2);
					break;
				case 3:
					obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->setChecked(true);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientValueP_" + id)).c_str()))->setValue(2000);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientAP_" + id)).c_str()))->setValue(1.0);
					break;
				default:
					obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->setChecked(false);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientValueP_" + id)).c_str()))->setValue(0);
					obj->findChild<QDoubleSpinBox *>(QString((std::string("gradientAP_" + id)).c_str()))->setValue(0);
			}
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

	// Opacidades escalares
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

	// Opacidades gradientes
	Q_FOREACH(QObject* obj, gradientList) { // Recorre todas las opacidades
		if (obj->inherits("QGroupBox")) { // Comprueba que no son las propiedades del layout
			std::string id = splitAndGetLast(obj->objectName().toUtf8().constData(), "_");
			if (obj->findChild<QCheckBox *>(QString((std::string("gradientEnableP_" + id)).c_str()))->isChecked()) { // Comprueba si está activado y añade punto
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
		figura->enableShadow(true);
	} else {
		figura->enableShadow(false);
	}
}

void MainWindow::updateRenderMode() {
	figura->setRenderMode(ui->renderMode->currentIndex());
}

void MainWindow::defaultMaterial() {
	ui->ambientValue->setValue(0.1);
	ui->diffuseValue->setValue(0.9);
	ui->specularValue->setValue(0.2);
	ui->powerValue->setValue(10.0);
}

void MainWindow::updateMaterial() {
	figura->setMaterial(
		ui->ambientValue->value(),
		ui->diffuseValue->value(),
		ui->specularValue->value(),
		ui->powerValue->value()
		);
}

void MainWindow::defaultPlanePosition() {
	if (figura->getVolume() != NULL) {
		double xSize = figura->getMaxXBound() - figura->getMinXBound(), ySize = figura->getMaxYBound() - figura->getMinYBound(), zSize = figura->getMaxZBound() - figura->getMinZBound();
		plano->setOrigin(xSize / 2, ySize / 2, zSize / 2); // Coloca el centro del plano en el centro de la figura
		plano->setAxial();
	}
}

void MainWindow::renderVolume() {
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
}

void MainWindow::renderSlice() {
	sliceViewer->Render();
}

void MainWindow::on_actionExit_triggered() {
	exit(0);
}

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha leído bien
		QProgressDialog progressDialog(this);
		progressDialog.setWindowTitle(QString("Cargando..."));
		progressDialog.setWindowFlags(progressDialog.windowFlags() & ~Qt::WindowCloseButtonHint);
		progressDialog.setCancelButton(0);
		progressDialog.show();

		plano->show(false);
		figura->setDICOMFolder(dicomFolder.toUtf8().constData()); // carga los archivos DICOM de la carpeta a la figura
		plano->setInputConnection(figura->getReader()); // conecta el plano con los datos del volumen
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		updateTF(); // actualiza función de transferencia con los valores de la GUI
		updateShadow(); // actualiza sombreado
		defaultPlanePosition(); // coloca el plano en una posición inicial
		plano->show(true);
		drawVolume(); // dibuja volumen
		renderSlice(); // dibuja el corte

		progressDialog.close();
	}
}

void MainWindow::on_actionExport_triggered() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Exportar imagen"), QDir::homePath(), "PNG (*.png);;JPG (*.jpg)");

	if (filename != NULL) {
		filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
		filter->SetInput(ui->volumeWidget->GetRenderWindow());
		filter->Update();
		if (getFileExtension(filename.toUtf8().constData()) == "png") {
			writer = vtkSmartPointer<vtkPNGWriter>::New();
			writer->SetFileName(filename.toUtf8().constData());
			writer->SetInputConnection(filter->GetOutputPort());
			writer->Write();
		} else {
			writer = vtkSmartPointer<vtkJPEGWriter>::New();
			writer->SetFileName(filename.toUtf8().constData());
			writer->SetInputConnection(filter->GetOutputPort());
			writer->Write();
		}
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

void MainWindow::on_restoreMaterial_pressed() {
	defaultMaterial();
}

void MainWindow::on_updateProperties_pressed() {
	updateShadow();
	updateMaterial();
	updateRenderMode();
	renderVolume();
}

void MainWindow::on_sagitalPlane_pressed() {
	plano->setSagital();
	renderVolume();
	renderSlice();
}

void MainWindow::on_coronalPlane_pressed() {
	plano->setCoronal();
	renderVolume();
	renderSlice();
}

void MainWindow::on_axialPlane_pressed() {
	plano->setAxial();
	renderVolume();
	renderSlice();
}

void MainWindow::on_enablePlane_stateChanged() {
	if (ui->enablePlane->isChecked()) {
		plano->enable(true);
	} else {
		plano->enable(false);
	}
	renderVolume();
}