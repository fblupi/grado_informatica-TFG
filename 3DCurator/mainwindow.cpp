#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader = dicomReader;
    colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
    property = vtkSmartPointer<vtkVolumeProperty>::New();
    //mapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
	leftRen = vtkSmartPointer<vtkRenderer>::New();
	viewer = vtkSmartPointer<vtkImageViewer2>::New();

	leftRen->SetBackground(.1, .2, .3); // set background blue
	ui->leftWidget->GetRenderWindow()->Render(); // render

    connectComponents();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectComponents() {
	ui->leftWidget->GetRenderWindow()->AddRenderer(leftRen); // attach render to widget
	//ui->rightWidget->SetRenderWindow(viewer->GetRenderWindow()); // attach viewer to widget

	//viewer->SetupInteractor(ui->rightWidget->GetInteractor()); // set viewer interactor
}

void MainWindow::setDICOMFolder(std::string s) {
    dicomReader->SetDirectoryName(s.c_str());
    dicomReader->Update(); // read dicom files

    //mapper->SetInputConnection(reader->GetOutputPort()); // connect volume mapper with dicom reader
	mapper->SetInputConnection(reader->GetOutputPort());
	//viewer->SetInputConnection(reader->GetOutputPort()); // connect viewer with dicom reader

	// update ui slider with number os slices
	//ui->slices->setMinimum(viewer->GetSliceMin());
	//ui->slices->setMaximum(viewer->GetSliceMax());
}

void MainWindow::setProperties() {
    property->SetIndependentComponents(true);
    property->SetColor(colorFun);
    property->SetScalarOpacity(opacityFun);
    property->SetInterpolationTypeToLinear();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.1);
}

void MainWindow::setTransferFunction() {
	double
		v0 = -1000,
		v1 = -700,
		v2 = v1 + 1,
		v3 = -350,
		v4 = v3 + 1,
		v5 = -200,
		v6 = v5 + 1,
		v7 = 500,
		v8 = v7 + 1;

	colorFun->AddRGBPoint(v0, 0, 0, 0);
	opacityFun->AddPoint(v0, 0);
	colorFun->AddRGBPoint(v1, 0, 0, 0);
	opacityFun->AddPoint(v1, 0);

	colorFun->AddRGBPoint(v2, .61, .45, .23);
	opacityFun->AddPoint(v2, 1);
	colorFun->AddRGBPoint(v3, .47, .29, .18);
	opacityFun->AddPoint(v3, 1);

    colorFun->AddRGBPoint(v4, 0, 0, 0);
	opacityFun->AddPoint(v4, 0);
	colorFun->AddRGBPoint(v5, 0, 0, 0);
	opacityFun->AddPoint(v5, 0);

	colorFun->AddRGBPoint(v6, .8, .8, .8);
	opacityFun->AddPoint(v6, .9);
	colorFun->AddRGBPoint(v6, .85, .85, .85);
	opacityFun->AddPoint(v6, .9);

	colorFun->AddRGBPoint(v7, 0, 0, 0);
	opacityFun->AddPoint(v7, 0);
	colorFun->AddRGBPoint(v8, 0, 0, 0);
	opacityFun->AddPoint(v8, 0);

}

void MainWindow::drawVolume() {
	leftRen->AddVolume(volume); // add generated volume
	
	leftRen->ResetCamera(); // reset camera

	// render
	viewer->Render();
	ui->leftWidget->GetRenderWindow()->Render();
}

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Open DICOM Folder"), QDir::currentPath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // folder successfully read
		setDICOMFolder(dicomFolder.toUtf8().constData());

		ui->labelFolder->setText(dicomFolder); // update ui label with dicom folder path

		setProperties();
		setTransferFunction();

		// attach properties and mapper to volume
		volume->SetProperty(property);
		volume->SetMapper(mapper);

		drawVolume();
	}
}

void MainWindow::on_slices_sliderMoved(int pos) {
	viewer->SetSlice(pos); // change slice showed
	viewer->Render();
}

void MainWindow::on_actionExit_triggered() {
    exit(0);
}
