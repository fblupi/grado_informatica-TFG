#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->isoValueSlider->setTracking(false);

	deleting = false;
	showPlane = true;
	ruleCounter = 0;

	volumeRen = vtkSmartPointer<vtkRenderer>::New();
	meshRen = vtkSmartPointer<vtkRenderer>::New();
	sliceViewer = vtkSmartPointer<vtkImageViewer2>::New();
	volumeStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	sliceStyle = vtkSmartPointer<InteractorStyleImage>::New();
	deleterStyle = vtkSmartPointer<InteractorStyleDeleter>::New();
	distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	plano = new Plano(); // crea una instancia de Plano
	figura = new Figura(); // crea una instancia de Figura
	defaultTF(); // define la función de transferencia, necesaria para definir las gráficas y el visor de cortes
	defaultMaterial(); // asigna un material por defecto a la figura

	colorTFChart = new ColorTFChart(ui->volumeWidget->GetRenderWindow(), ui->colorTFWidget->GetRenderWindow(), figura->getTransferFunction()->getColorFun(), "Densidad", "", MIN_INTENSITY, MAX_INTENSITY);
	scalarTFChart = new OpacityTFChart(ui->volumeWidget->GetRenderWindow(), ui->scalarTFWidget->GetRenderWindow(), figura->getTransferFunction()->getScalarFun(), "Densidad", "Opacidad", MIN_INTENSITY, MAX_INTENSITY);
	gradientTFChart = new OpacityTFChart(ui->volumeWidget->GetRenderWindow(), ui->gradientTFWidget->GetRenderWindow(), figura->getTransferFunction()->getGradientFun(), "Gradiente", "Opacidad", 0, MAX_INTENSITY);
	updateSliders(); // actualiza valores de los sliders

	sliceViewer->GetWindowLevel()->SetLookupTable(figura->getTransferFunction()->getColorFun()); // usa los mismo colores en el slice viewer que los usados en la TF
	sliceViewer->SetColorLevel(-600); 
	sliceViewer->SetColorWindow(400);

	setBackgroundColor(volumeRen, .1, .2, .3); // fondo azul oscuro
	setBackgroundColor(meshRen, .1, .2, .3); // fondo azul oscuro

    connectComponents(); // conecta los renderers con los widgets

	renderVolume();
	renderMesh();

	plano->show(false); // No muestra el corte del plano
	enablePlane(); // Muestra el plano
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b) {
	ren->SetBackground(r, g, b);
}

void MainWindow::connectComponents() {
	ui->volumeWidget->GetRenderWindow()->AddRenderer(volumeRen); // conecta el volume widget con el renderer
	ui->meshWidget->GetRenderWindow()->AddRenderer(meshRen); // conecta el mesh widget con el renderer

	ui->slicesWidget->SetRenderWindow(sliceViewer->GetRenderWindow()); // conecta el slice widget con el viewer
	sliceViewer->SetInputData(plano->getPlane()->GetResliceOutput()); // asigna el flujo de salida de los cortes del plano al slice viewer

	ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(volumeStyle); // asigna el estilo al interactor del volume widget
	plano->getPlane()->SetInteractor(ui->volumeWidget->GetRenderWindow()->GetInteractor()); // asigna el interactor al plano para poder verlo y moverlo

	sliceViewer->SetupInteractor(ui->slicesWidget->GetInteractor()); // asigna el interactor al viewer
	sliceStyle->SetPlano(plano);
	sliceStyle->SetDefaultRenderer(sliceViewer->GetRenderer());
	sliceStyle->SetLabel(ui->coordsAndValueLabel);
	ui->slicesWidget->GetInteractor()->SetInteractorStyle(sliceStyle); // asigna el estilo al interactor del slice widget

	plano->setViewer(sliceViewer); // asigna el slice viewer al plano para que pueda renderizar cuando se de el evento de mover el plano

	distanceWidget->SetInteractor(ui->slicesWidget->GetInteractor()); // conecta la regla para medir con el interactor de los cortes
	distanceWidget->CreateDefaultRepresentation(); // usa la representación por defecto
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm"); // cambia el formato de la etiqueta

	deleterStyle->SetFigura(figura); // asigna la figura al estilo para borrar partes
	deleterStyle->SetPlano(plano); // asigna el plano al estilo para borrar partes
	deleterStyle->SetDefaultRenderer(volumeRen); // asigna el renderer al estilo para borrar partes
	deleterStyle->SetViewer(sliceViewer); // asigna la ventana de cortes
	deleterStyle->SetDefaultRenderWindow(ui->volumeWidget->GetRenderWindow()); // asigna la ventana de renderizado al estilo para borrar partes
}

void MainWindow::drawVolume() {
	volumeRen->AddViewProp(figura->getVolume()); // añade el volumen al renderer
	volumeRen->ResetCamera(); // resetea la cámera
	renderVolume();
}

void MainWindow::drawMesh() {
	meshRen->AddActor(figura->getMesh()); // añade la malla al renderer
	meshRen->ResetCamera(); // resetea la cámara
	renderMesh();
}

void MainWindow::removeVolume() {
	volumeRen->RemoveAllViewProps();
	volumeRen->ResetCamera();
	renderVolume();
}

void MainWindow::removeMesh() {
	meshRen->RemoveAllViewProps();
	meshRen->ResetCamera();
	renderMesh();
}

void MainWindow::renderVolume() {
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
}
void MainWindow::renderMesh() {
	ui->meshWidget->GetRenderWindow()->Render(); // renderiza
}

void MainWindow::renderSlice() {
	sliceViewer->Render(); // renderiza
}

void MainWindow::loadDefaultPreset(QFile *file) {
	if (file->open(QIODevice::ReadOnly | QIODevice::Text)) { // se lee el archivo
		std::istringstream ss;
		ss.str(QString(file->readAll()).toStdString());
		figura->getTransferFunction()->read(ss);
		file->close(); // se cierra el archivo
		ui->tfName->setText(QString::fromUtf8(figura->getTransferFunction()->getName().c_str()));
		ui->tfDescription->setText(QString::fromUtf8(figura->getTransferFunction()->getDescription().c_str()));
	} else {
		cerr << "Error abriendo archivo por defecto de función de transferencia" << endl;
		exit(-1); // si no lo puede leer se sale
	}
}

void MainWindow::defaultTF() {
	QFile file(":/presets/ct-woodsculpture.xml");
	loadDefaultPreset(&file);
}

void MainWindow::defaultMaterial() {
	ui->ambientValue->setValue(0.1);
	ui->diffuseValue->setValue(0.9);
	ui->specularValue->setValue(0.2);
	ui->powerValue->setValue(10.0);
}

void MainWindow::defaultPlanePosition() {
	if (figura->getVolume() != NULL) {
		double xSize = figura->getMaxXBound() - figura->getMinXBound(), ySize = figura->getMaxYBound() - figura->getMinYBound(), zSize = figura->getMaxZBound() - figura->getMinZBound();
		plano->setOrigin(xSize / 2, ySize / 2, zSize / 2); // Coloca el centro del plano en el centro de la figura
		plano->setAxial();
	}
}

void MainWindow::updateShadow() {
	if (ui->enableShadow->isChecked()) {
		figura->enableShadow(true);
	} else {
		figura->enableShadow(false);
	}
}

void MainWindow::updateMaterial() {
	figura->setMaterial(
		ui->ambientValue->value(),
		ui->diffuseValue->value(),
		ui->specularValue->value(),
		ui->powerValue->value()
		);
}

void MainWindow::updateRenderMode() {
	figura->setRenderMode(ui->renderMode->currentIndex());
}

void MainWindow::updateSliders() {
	// Color
	ui->colorTFMinSlider->setMinimum((int)MIN_INTENSITY);
	ui->colorTFMinSlider->setValue((int)figura->getTransferFunction()->getColorFun()->GetRange()[0]);
	ui->colorTFMinSlider->setMaximum((int)figura->getTransferFunction()->getColorFun()->GetRange()[1]);

	ui->colorTFMaxSlider->setMinimum((int)figura->getTransferFunction()->getColorFun()->GetRange()[0]);
	ui->colorTFMaxSlider->setValue((int)figura->getTransferFunction()->getColorFun()->GetRange()[1]);
	ui->colorTFMaxSlider->setMaximum((int)MAX_INTENSITY);

	// Scalar
	ui->scalarTFMinSlider->setMinimum((int)MIN_INTENSITY);
	ui->scalarTFMinSlider->setValue((int)figura->getTransferFunction()->getScalarFun()->GetRange()[0]);
	ui->scalarTFMinSlider->setMaximum((int)figura->getTransferFunction()->getScalarFun()->GetRange()[1]);

	ui->scalarTFMaxSlider->setMinimum((int)figura->getTransferFunction()->getScalarFun()->GetRange()[0]);
	ui->scalarTFMaxSlider->setValue((int)figura->getTransferFunction()->getScalarFun()->GetRange()[1]);
	ui->scalarTFMaxSlider->setMaximum((int)MAX_INTENSITY);

	// Gradient
	ui->gradientTFMinSlider->setMinimum(0);
	ui->gradientTFMinSlider->setValue((int)figura->getTransferFunction()->getGradientFun()->GetRange()[0]);
	ui->gradientTFMinSlider->setMaximum((int)figura->getTransferFunction()->getGradientFun()->GetRange()[1]);

	ui->gradientTFMaxSlider->setMinimum((int)figura->getTransferFunction()->getGradientFun()->GetRange()[0]);
	ui->gradientTFMaxSlider->setValue((int)figura->getTransferFunction()->getGradientFun()->GetRange()[1]);
	ui->gradientTFMaxSlider->setMaximum((int)MAX_INTENSITY);
}

void MainWindow::importDICOM() {
	QString dicomFolder = QFileDialog::getExistingDirectory(
		this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha leído bien
		QPointer<QProgressBar> bar = new QProgressBar(0);
		QPointer<QProgressDialog> progressDialog = new QProgressDialog(0);
		progressDialog->setWindowTitle(QString("Cargando..."));
		progressDialog->setLabelText(QString::fromLatin1("Cargando los datos DICOM especificados"));
		progressDialog->setWindowIcon(QIcon(":/icons/3DCurator.ico"));
		progressDialog->setWindowFlags(progressDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
		progressDialog->setCancelButton(0);
		progressDialog->setBar(bar);
		progressDialog->show();
		bar->close();
		QApplication::processEvents();

		removeVolume();
		removeMesh();

		plano->show(false);
		disablePlane();
		figura->setDICOMFolder(dicomFolder.toUtf8().constData()); // carga los archivos DICOM de la carpeta a la figura
		plano->setInputData(figura->getImageData()); // conecta el plano con los datos del volumen
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		updateShadow(); // actualiza sombreado
		defaultPlanePosition(); // coloca el plano en una posición inicial
		plano->show(true);
		enablePlane();

		drawVolume(); // dibuja volumen
		drawMesh(); // dibuja la malla
		renderSlice(); // dibuja el corte

		progressDialog->close();
	}
}

void MainWindow::importPreset() {
	QString presetFile = QFileDialog::getOpenFileName(this, tr("Importar preset"), QDir::homePath());

	if (presetFile != NULL) {
		std::string s = presetFile.toUtf8().constData();
		figura->getTransferFunction()->read(s);

		ui->tfName->setText(QString::fromUtf8(figura->getTransferFunction()->getName().c_str()));
		ui->tfDescription->setText(QString::fromUtf8(figura->getTransferFunction()->getDescription().c_str()));

		colorTFChart->defaultRange();
		scalarTFChart->defaultRange();
		gradientTFChart->defaultRange();
		updateSliders();
	}
}

void MainWindow::exportImageFromRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin, const QString filename) {
	if (filename != NULL) { // el archivo se ha leído bien
		vtkSmartPointer<vtkWindowToImageFilter> filter = vtkSmartPointer<vtkWindowToImageFilter>::New(); // crea el filter
		filter->SetInput(renWin); // conecta el filter al widget
		filter->Update(); // actualiza el filter
		vtkSmartPointer<vtkImageWriter> writer;
		if (toUpper(getFileExtension(filename.toUtf8().constData())) == "PNG") { // comprueba si es PNG
			writer = vtkSmartPointer<vtkPNGWriter>::New(); // crea el writer de PNG
		} else { // si no, es JPG
			writer = vtkSmartPointer<vtkJPEGWriter>::New(); // crea el writer de JPG
		}
		writer->SetFileName(filename.toUtf8().constData()); // le pone el nombre que se había indicado
		writer->SetInputConnection(filter->GetOutputPort()); // conecta el writer con el filter
		writer->Write(); // crea la imagen de salida
	}
}

void MainWindow::exportMeshToFile(const QString filename) {
	if (filename != NULL) { // el archivo se ha leído bien
		QPointer<QProgressBar> bar = new QProgressBar(0);
		QPointer<QProgressDialog> progressDialog = new QProgressDialog(0);
		progressDialog->setWindowTitle(QString("Extrayendo..."));
		progressDialog->setLabelText(QString::fromLatin1("Extrayendo la malla del modelo"));
		progressDialog->setWindowIcon(QIcon(":/icons/3DCurator.ico"));
		progressDialog->setWindowFlags(progressDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
		progressDialog->setCancelButton(0);
		progressDialog->setBar(bar);
		progressDialog->show();
		bar->close();
		QApplication::processEvents();

		vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
		surface->SetInputData(figura->getImageData());
		surface->UpdateInformation();
		surface->ComputeNormalsOn();
		surface->ComputeScalarsOn();
		surface->SetValue(0, figura->getIsoValue());
		vtkSmartPointer<vtkPolyData> marched = vtkSmartPointer<vtkPolyData>::New();
		surface->Update();
		marched->DeepCopy(surface->GetOutput());
		vtkSmartPointer<vtkDecimatePro> decimator = vtkSmartPointer<vtkDecimatePro>::New();
		decimator->SetInputData(marched);
		decimator->SetTargetReduction(0.5);
		decimator->SetPreserveTopology(true);
		decimator->Update();
		vtkSmartPointer<vtkSmoothPolyDataFilter> smoother = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
		smoother->SetInputData(decimator->GetOutput());
		smoother->SetNumberOfIterations(5);
		smoother->SetFeatureAngle(60);
		smoother->SetRelaxationFactor(0.05);
		smoother->FeatureEdgeSmoothingOff();
		smoother->Update();
		vtkSmartPointer<vtkPolyData> mesh = vtkSmartPointer<vtkPolyData>::New();
		mesh->ShallowCopy(smoother->GetOutput());
		vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
		stlWriter->SetFileName(filename.toUtf8().constData());
		stlWriter->SetInputData(mesh);
		stlWriter->Write();

		progressDialog->close();
	}
}

void MainWindow::exportPreset(const QString filename) {
	if (filename != NULL) { // el nombre del archivo es correcto
		figura->getTransferFunction()->setName(ui->tfName->text().toUtf8().constData());
		figura->getTransferFunction()->setDescription(ui->tfDescription->text().toUtf8().constData());

		std::string s = filename.toUtf8().constData();
		figura->getTransferFunction()->write(s);
	}
}

QString MainWindow::getExportPresetFilename(const QString defaultFilename) {
	return QFileDialog::getSaveFileName(
		this, tr("Exportar preset"), QDir(QDir::homePath()).filePath(defaultFilename), "XML (*.xml)");
}

QString MainWindow::getExportImageFilename(const QString defaultFilename) {
	return QFileDialog::getSaveFileName(
		this, tr("Exportar imagen"), QDir(QDir::homePath()).filePath(defaultFilename), "PNG (*.png);;JPG (*.jpg)");
}

QString MainWindow::getExportMeshFilename(const QString defaultFilename) {
	return QFileDialog::getSaveFileName(
		this, tr("Exportar malla"), QDir(QDir::homePath()).filePath(defaultFilename), "STL (*.stl)");
}

void MainWindow::enablePlane() {
	plano->enable(true);
	QIcon icon(":/icons/eye-slash.png");
	ui->enableDisablePlane->setIcon(icon);
	showPlane = true;
}

void MainWindow::disablePlane() {
	plano->enable(false);
	QIcon icon(":/icons/eye.png");
	ui->enableDisablePlane->setIcon(icon);
	showPlane = false;
}

void MainWindow::exportMesh() {
	if (figura->getLoaded()) {
		exportMeshToFile(getExportMeshFilename("Mesh"));
	} else {
		launchWarningNoVolume();
	}
}

void MainWindow::updateMesh() {
	if (figura->getLoaded()) {
		QPointer<QProgressBar> bar = new QProgressBar(0);
		QPointer<QProgressDialog> progressDialog = new QProgressDialog(0);
		progressDialog->setWindowTitle(QString("Actualizando..."));
		progressDialog->setLabelText(QString::fromLatin1("Generando el modelo con la isosuperficie especificada"));
		progressDialog->setWindowIcon(QIcon(":/icons/3DCurator.ico"));
		progressDialog->setWindowFlags(progressDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
		progressDialog->setCancelButton(0);
		progressDialog->setBar(bar);
		progressDialog->show();
		bar->close();
		QApplication::processEvents();

		figura->createMesh();
		meshRen->Render();

		progressDialog->close();

	} else {
		launchWarningNoVolume();
	}
}

void MainWindow::enableDisablePlane() {
	if (showPlane) {
		disablePlane();
	} else {
		enablePlane();
	}
	renderVolume();
}

void MainWindow::axialPlane() {
	if (figura->getLoaded()) {
		plano->setAxial();
		renderVolume();
		renderSlice();
	} else {
		launchWarningNoVolume();
	}
}

void MainWindow::coronalPlane() {
	if (figura->getLoaded()) {
		plano->setCoronal();
		renderVolume();
		renderSlice();
	} else {
		launchWarningNoVolume();
	}
}

void MainWindow::sagitalPlane() {
	if (figura->getLoaded()) {
		plano->setSagital();
		renderVolume();
		renderSlice();
	} else {
		launchWarningNoVolume();
	}
}

void MainWindow::deleteVolumeParts() {
	if (deleting) {
		deleting = false;
		setBackgroundColor(volumeRen, .1, .2, .3);
		ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(volumeStyle);
	} else {
		deleting = true;
		setBackgroundColor(volumeRen, .2, .3, .1);
		ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(deleterStyle);
	}
	plano->getPlane()->UpdatePlacement();
	renderVolume();
	renderSlice();
}

void MainWindow::importCompletePreset() {
	QFile file(":/presets/ct-woodsculpture.xml");
	loadDefaultPreset(&file);
	renderVolume();
}

void MainWindow::importWoodPreset() {
	QFile file(":/presets/ct-onlywood.xml");
	loadDefaultPreset(&file);
	renderVolume();
}

void MainWindow::importStuccoPreset() {
	QFile file(":/presets/ct-onlystucco.xml");
	loadDefaultPreset(&file);
	renderVolume();
}

void MainWindow::importMetalPreset() {
	QFile file(":/presets/ct-onlymetal.xml");
	loadDefaultPreset(&file);
	renderVolume();
}

void MainWindow::launchWarningNoVolume() {
	QPointer<QMessageBox> confirmBox = new QMessageBox(0);
	confirmBox->setWindowTitle(QString::fromLatin1("Advertencia"));
	confirmBox->setWindowIcon(QIcon(":/icons/3DCurator.ico"));
	confirmBox->setIcon(QMessageBox::Information);
	confirmBox->setText(QString::fromLatin1("Hace falta cargar un modelo antes"));
	confirmBox->setStandardButtons(QMessageBox::Ok);
	confirmBox->exec();
}

void MainWindow::addRule() {
	ruleCounter++;
	QListWidgetItem *item = new QListWidgetItem(0);
	item->setText(("Regla " + std::to_string(ruleCounter)).c_str());
	ui->rulesList->addItem(item);
	ui->rulesList->setCurrentItem(item);
}

void MainWindow::deleteRule() {
	 delete ui->rulesList->currentItem();
	 if (ui->rulesList->count() == 0) {
		 ruleCounter = 0;
	 }
}

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - MENÚ
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_actionOpenDICOM_triggered() {
	importDICOM();
}

void MainWindow::on_actionExportVolumeImage_triggered() {
	exportImageFromRenderWindow(
		ui->volumeWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_actionExportSliceImage_triggered() {
	exportImageFromRenderWindow(
		ui->slicesWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_actionExit_triggered() {
	exit(0);
}

void MainWindow::on_actionEnableDisablePlane_triggered() {
	enableDisablePlane();
}

void MainWindow::on_actionSagitalPlane_triggered() {
	sagitalPlane();
}

void MainWindow::on_actionAxialPlane_triggered() {
	axialPlane();
}

void MainWindow::on_actionCoronalPlane_triggered() {
	coronalPlane();
}

void MainWindow::on_actionImportPreset_triggered() {
	importPreset();
}

void MainWindow::on_actionCompletePreset_triggered() {
	importCompletePreset();
}

void MainWindow::on_actionWoodPreset_triggered() {
	importWoodPreset();
}

void MainWindow::on_actionStuccoPreset_triggered() {
	importStuccoPreset();
}

void MainWindow::on_actionMetalPreset_triggered() {
	importMetalPreset();
}

void MainWindow::on_actionExportPreset_triggered() {
	exportPreset(getExportPresetFilename(ui->tfName->text()));
}

void MainWindow::on_actionDeleteVolumeParts_triggered() {
	deleteVolumeParts();
}

void MainWindow::on_actionWoodMesh_triggered() {
	ui->isoValueSlider->setValue(WOOD_ISOVALUE);
}

void MainWindow::on_actionStuccoMesh_triggered() {
	ui->isoValueSlider->setValue(STUCCO_ISOVALUE);
}

void MainWindow::on_actionMetalMesh_triggered() {
	ui->isoValueSlider->setValue(METAL_ISOVALUE);
}

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - BOTONES
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_openDICOM_pressed() {
	importDICOM();
}

void MainWindow::on_axialPlane_pressed() {
	axialPlane();
}

void MainWindow::on_coronalPlane_pressed() {
	coronalPlane();
}

void MainWindow::on_sagitalPlane_pressed() {
	sagitalPlane();
}

void MainWindow::on_exportSliceImage_pressed() {
	exportImageFromRenderWindow(
		ui->slicesWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_exportVolumeImage_pressed() {
	exportImageFromRenderWindow(
		ui->volumeWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_importPreset_pressed() {
	importPreset();
}

void MainWindow::on_exportPreset_pressed() {
	exportPreset(getExportPresetFilename(ui->tfName->text()));
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

void MainWindow::on_completePreset_pressed() {
	importCompletePreset();
}

void MainWindow::on_woodPreset_pressed() {
	importWoodPreset();
}

void MainWindow::on_stuccoPreset_pressed() {
	importStuccoPreset();
}

void MainWindow::on_metalPreset_pressed() {
	importMetalPreset();
}

void MainWindow::on_extractMesh_pressed() {
	exportMesh();
}

void MainWindow::on_extractMeshWood_pressed() {
	ui->isoValueSlider->setValue(WOOD_ISOVALUE);
}

void MainWindow::on_extractMeshStucco_pressed() {
	ui->isoValueSlider->setValue(STUCCO_ISOVALUE);
}

void MainWindow::on_extractMeshMetal_pressed() {
	ui->isoValueSlider->setValue(METAL_ISOVALUE);
}

void MainWindow::on_enableDisablePlane_pressed() {
	enableDisablePlane();
}

void MainWindow::on_deleteVolumeParts_pressed() {
	deleteVolumeParts();
}

void MainWindow::on_addRule_pressed() {
	addRule();
}

void MainWindow::on_deleteRule_pressed() {
	deleteRule();
}

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - CHECKBOX
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_enableRule_stateChanged() {
	if (ui->enableRule->isChecked()) {
		distanceWidget->On();
	} else {
		distanceWidget->Off();
		//distanceWidget->SetWidgetStateToStart(); // Borra los puntos actuales
	}
}

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - SLIDERS
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_colorTFMaxSlider_valueChanged() {
	colorTFChart->setRange((double)ui->colorTFMinSlider->value(), (double)ui->colorTFMaxSlider->value());
	ui->colorTFMinSlider->setMaximum(ui->colorTFMaxSlider->value() - 1);
}

void MainWindow::on_colorTFMinSlider_valueChanged() {
	colorTFChart->setRange((double)ui->colorTFMinSlider->value(), (double)ui->colorTFMaxSlider->value());
	ui->colorTFMaxSlider->setMinimum(ui->colorTFMinSlider->value() + 1);
}

void MainWindow::on_gradientTFMaxSlider_valueChanged() {
	gradientTFChart->setRange((double)ui->gradientTFMinSlider->value(), (double)ui->gradientTFMaxSlider->value());
	ui->gradientTFMinSlider->setMaximum(ui->gradientTFMaxSlider->value() - 1);
}

void MainWindow::on_gradientTFMinSlider_valueChanged() {
	gradientTFChart->setRange((double)ui->gradientTFMinSlider->value(), (double)ui->gradientTFMaxSlider->value());
	ui->gradientTFMaxSlider->setMinimum(ui->gradientTFMinSlider->value() + 1);
}

void MainWindow::on_scalarTFMaxSlider_valueChanged() {
	scalarTFChart->setRange((double)ui->scalarTFMinSlider->value(), (double)ui->scalarTFMaxSlider->value());
	ui->scalarTFMinSlider->setMaximum(ui->scalarTFMaxSlider->value() - 1);
}

void MainWindow::on_scalarTFMinSlider_valueChanged() {
	scalarTFChart->setRange((double)ui->scalarTFMinSlider->value(), (double)ui->scalarTFMaxSlider->value());
	ui->scalarTFMaxSlider->setMinimum(ui->scalarTFMinSlider->value() + 1);
}

void MainWindow::on_isoValueSlider_valueChanged() {
	figura->setIsoValue(ui->isoValueSlider->value());
	updateMesh();
}