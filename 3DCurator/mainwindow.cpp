#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	volumeRen = vtkSmartPointer<vtkRenderer>::New();
	sliceViewer = vtkSmartPointer<vtkImageViewer2>::New();
	volumeStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	sliceStyle = vtkSmartPointer<InteractorStyleImage>::New();
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
    connectComponents(); // conecta los renderers con los widgets

	renderVolume();

	plano->show(false); // No muestra el corte del plano
	plano->enable(true); // Muestra el plano
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b) {
	ren->SetBackground(r, g, b);
}

void MainWindow::connectComponents() {
	ui->volumeWidget->GetRenderWindow()->AddRenderer(volumeRen); // conecta el volume widget con el renderer

	ui->slicesWidget->SetRenderWindow(sliceViewer->GetRenderWindow()); // conecta el slice widget con el viewer
	sliceViewer->SetInputData(plano->getPlane()->GetResliceOutput()); // asigna el flujo de salida de los cortes del plano al slice viewer

	ui->volumeWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(volumeStyle); // asigna el estilo al interactor del volume widget
	plano->getPlane()->SetInteractor(ui->volumeWidget->GetRenderWindow()->GetInteractor()); // asigna el interactor al plano para poder verlo y moverlo

	sliceViewer->SetupInteractor(ui->slicesWidget->GetInteractor()); // asigna el interactor al viewer
	ui->slicesWidget->GetInteractor()->SetInteractorStyle(sliceStyle); // asigna el estilo al interactor del slice widget

	plano->setViewer(sliceViewer); // asigna el slice viewer al plano para que pueda renderizar cuando se de el evento de mover el plano

	distanceWidget->SetInteractor(ui->slicesWidget->GetInteractor()); // conecta la regla para medir con el interactor de los cortes
	distanceWidget->CreateDefaultRepresentation(); // usa la representación por defecto
	static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.3g mm"); // cambia el formato de la etiqueta
}

void MainWindow::drawVolume() {
	volumeRen->AddViewProp(figura->getVolume()); // añade el volumen al renderer
	volumeRen->ResetCamera(); // resetea la cámera
	renderVolume();
}

void MainWindow::renderVolume() {
	ui->volumeWidget->GetRenderWindow()->Render(); // renderiza
}

void MainWindow::renderSlice() {
	sliceViewer->Render(); // renderiza
}

void MainWindow::defaultTF() {
	QFile file(":/presets/ct-woodsculpture.xml");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // se lee el archivo
		std::istringstream ss;
		ss.str(QString(file.readAll()).toStdString());
		figura->getTransferFunction()->read(ss);
		file.close(); // se cierra el archivo
		ui->tfName->setText(QString::fromUtf8(figura->getTransferFunction()->getName().c_str()));
		ui->tfDescription->setText(QString::fromUtf8(figura->getTransferFunction()->getDescription().c_str()));
	} else {
		cerr << "Error abriendo archivo por defecto de función de transferencia" << endl;
		exit(-1); // si no lo puede leer se sale
	}
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
		QProgressDialog progressDialog(this);
		progressDialog.setWindowTitle(QString("Cargando..."));
		progressDialog.setWindowFlags(progressDialog.windowFlags() & ~Qt::WindowCloseButtonHint);
		progressDialog.setCancelButton(0);
		progressDialog.show();

		plano->show(false);
		figura->setDICOMFolder(dicomFolder.toUtf8().constData()); // carga los archivos DICOM de la carpeta a la figura
		plano->setInputConnection(figura->getReader()); // conecta el plano con los datos del volumen
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		updateShadow(); // actualiza sombreado
		defaultPlanePosition(); // coloca el plano en una posición inicial
		plano->show(true);
		drawVolume(); // dibuja volumen
		renderSlice(); // dibuja el corte

		/* Dibuja el histograma con valores */
		//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		//vtkSmartPointer<vtkXYPlotActor> plot = vtkSmartPointer<vtkXYPlotActor>::New();
		//plot->AddDataSetInputConnection(figura->getHistogram()->GetOutputPort());
		//plot->SetXRange(-750, 3000);
		//renderer->AddActor(plot);
		//ui->histogramWidget->GetRenderWindow()->AddRenderer(renderer);
		/* ------------------------------- */

		progressDialog.close();
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
		filter = vtkSmartPointer<vtkWindowToImageFilter>::New(); // crea el filter
		filter->SetInput(renWin); // conecta el filter al widget
		filter->Update(); // actualiza el filter
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

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - MENÚ
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_actionExit_triggered() {
	exit(0);
}

void MainWindow::on_actionExportSliceImage_triggered() {
	exportImageFromRenderWindow(
		ui->slicesWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_actionExportVolumeImage_triggered() {
	exportImageFromRenderWindow(
		ui->volumeWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_actionExportPreset_triggered() {
	exportPreset(getExportPresetFilename(ui->tfName->text()));
}

void MainWindow::on_actionImportPreset_triggered() {
	importPreset();
}

void MainWindow::on_actionOpenDICOM_triggered() {
	importDICOM();
}

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - BOTONES
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_axialPlane_pressed() {
	plano->setAxial();
	renderVolume();
	renderSlice();
}

void MainWindow::on_coronalPlane_pressed() {
	plano->setCoronal();
	renderVolume();
	renderSlice();
}

void MainWindow::on_sagitalPlane_pressed() {
	plano->setSagital();
	renderVolume();
	renderSlice();
}

void MainWindow::on_exportSliceImage_pressed() {
	exportImageFromRenderWindow(
		ui->slicesWidget->GetRenderWindow(), QDir(QDir::homePath()).filePath(QString::fromStdString(getCurrentDate() + ".png")));
}

void MainWindow::on_exportSliceImageAs_pressed() {
	exportImageFromRenderWindow(
		ui->slicesWidget->GetRenderWindow(), getExportImageFilename(QString::fromStdString(getCurrentDate())));
}

void MainWindow::on_importPreset_pressed() {
	importPreset();
}

void MainWindow::on_exportPreset_pressed() {
	exportPreset(QDir(QDir::homePath()).filePath(ui->tfName->text() + ".xml"));
}

void MainWindow::on_exportPresetAs_pressed() {
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

//---------------------------------------------------------------------------------------------------------------------------------
// Eventos GUI - CHECKBOX
//---------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_enablePlane_stateChanged() {
	if (ui->enablePlane->isChecked()) {
		plano->enable(true);
	} else {
		plano->enable(false);
	}
	renderVolume();
}

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
	ui->colorTFMinSlider->setMaximum(ui->colorTFMaxSlider->value());
}

void MainWindow::on_colorTFMinSlider_valueChanged() {
	colorTFChart->setRange((double)ui->colorTFMinSlider->value(), (double)ui->colorTFMaxSlider->value());
	ui->colorTFMaxSlider->setMinimum(ui->colorTFMinSlider->value());
}

void MainWindow::on_gradientTFMaxSlider_valueChanged() {
	gradientTFChart->setRange((double)ui->gradientTFMinSlider->value(), (double)ui->gradientTFMaxSlider->value());
	ui->gradientTFMinSlider->setMaximum(ui->gradientTFMaxSlider->value());
}

void MainWindow::on_gradientTFMinSlider_valueChanged() {
	gradientTFChart->setRange((double)ui->gradientTFMinSlider->value(), (double)ui->gradientTFMaxSlider->value());
	ui->gradientTFMaxSlider->setMinimum(ui->gradientTFMinSlider->value());
}

void MainWindow::on_scalarTFMaxSlider_valueChanged() {
	scalarTFChart->setRange((double)ui->scalarTFMinSlider->value(), (double)ui->scalarTFMaxSlider->value());
	ui->scalarTFMinSlider->setMaximum(ui->scalarTFMaxSlider->value());
}

void MainWindow::on_scalarTFMinSlider_valueChanged() {
	scalarTFChart->setRange((double)ui->scalarTFMinSlider->value(), (double)ui->scalarTFMaxSlider->value());
	ui->scalarTFMaxSlider->setMinimum(ui->scalarTFMinSlider->value());
}