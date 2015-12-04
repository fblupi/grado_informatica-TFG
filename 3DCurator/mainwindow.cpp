#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
    property = vtkSmartPointer<vtkVolumeProperty>::New();
    mapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    ren = vtkSmartPointer<vtkRenderer>::New();

    setProperties();
    setTransferFunction();
    volume->SetProperty(property);

    ui->widget->GetRenderWindow()->AddRenderer(ren);
    ren->SetBackground(.1, .2, .3);
    ui->widget->GetRenderWindow()->Render();

    ren->AddVolume(volume);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setDICOMFolder(std::string s) {
    dicomReader->SetDirectoryName(s.c_str());
    dicomReader->Update();

    reader = dicomReader;
    mapper->SetInputConnection(reader->GetOutputPort());
    volume->SetMapper(mapper);
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
    colorFun->AddRGBPoint(-3024, 0, 0, 0, .5, .0);
    colorFun->AddRGBPoint(-750, .73, .25, .30, .49, .613);
    colorFun->AddRGBPoint(-500, .90, .82, .56, .5, 0.0);
    colorFun->AddRGBPoint(3071, 1, 1, 1, .5, .0);

    opacityFun->AddPoint(-3024, 0, .5, .0);
    opacityFun->AddPoint(-750, 0, .49, .61);
    opacityFun->AddPoint(-500, .72, .5, .0);
    opacityFun->AddPoint(3071, .71, .5, .0);
}

void MainWindow::drawVolume() {
    ren->ResetCamera();
    ui->widget->GetRenderWindow()->Render();

    //ui->widget->GetRenderWindow()->GetInteractor()->Start();
}

void MainWindow::on_actionOpenDICOM_triggered() {
    QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Open DICOM Folder"), QDir::currentPath(), QFileDialog::ShowDirsOnly);
    setDICOMFolder(dicomFolder.toUtf8().constData());

    ui->labelFolder->setText(dicomFolder);

    drawVolume();
}

void MainWindow::on_actionExit_triggered() {
    exit(0);
}
