#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader = dicomReader;
    colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
    property = vtkSmartPointer<vtkVolumeProperty>::New();
    mapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
	leftRen = vtkSmartPointer<vtkRenderer>::New();
	viewer = vtkSmartPointer<vtkImageViewer2>::New();

	leftRen->SetBackground(.1, .2, .3); // set background blue

    connectComponents();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connectComponents() {
	ui->leftWidget->GetRenderWindow()->AddRenderer(leftRen); // attach render to widget
	ui->rightWidget->SetRenderWindow(viewer->GetRenderWindow()); // attach viewer to widget

	viewer->SetupInteractor(ui->rightWidget->GetInteractor()); // set viewer interactor
}

void MainWindow::setDICOMFolder(std::string s) {
    dicomReader->SetDirectoryName(s.c_str());
    dicomReader->Update(); // read dicom files

    mapper->SetInputConnection(reader->GetOutputPort()); // connect volume mapper with dicom reader
	viewer->SetInputConnection(reader->GetOutputPort()); // connect viewer with dicom reader

	// update ui slider with number os slices
	ui->slices->setMinimum(viewer->GetSliceMin());
	ui->slices->setMaximum(viewer->GetSliceMax());
}

void MainWindow::setProperties() {
    property->SetIndependentComponents(true);
    property->SetColor(colorFun);
    property->SetScalarOpacity(opacityFun);
    property->SetInterpolationTypeToLinear();
    property->ShadeOn();
    property->SetAmbient(0.1);
    property->SetDiffuse(0.9);
    property->SetSpecular(0.2);
    property->SetSpecularPower(10.0);
    property->SetScalarOpacityUnitDistance(0.8919);
}

void MainWindow::setTransferFunction() {
    colorFun->AddRGBPoint(-3024, 0, 0, 0, .5, 0);
    colorFun->AddRGBPoint(-750, .73, .25, .30, .49, .613);
    colorFun->AddRGBPoint(-500, .90, .82, .56, .5, 0);
    colorFun->AddRGBPoint(3071, 1, 1, 1, .5, 0);

    opacityFun->AddPoint(-3024, 0, .5, 0);
    opacityFun->AddPoint(-750, 0, .49, .61);
    opacityFun->AddPoint(-500, .72, .5, 0);
    opacityFun->AddPoint(3071, .71, .5, 0);
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
