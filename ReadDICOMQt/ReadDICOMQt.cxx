#include "ReadDICOMQt.h"
#include "ui_ReadDICOMQt.h"

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)

#include <QFileDialog>

ReadDICOMQt::ReadDICOMQt(QWidget *parent) : QMainWindow(parent), ui(new Ui::ReadDICOMQt) {
    ui->setupUi(this);
    reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    viewer = vtkSmartPointer<vtkImageViewer>::New();
}

ReadDICOMQt::~ReadDICOMQt() {
    delete ui;
}

void ReadDICOMQt::openDICOMFile() {
    QString fileNameDICOM = QFileDialog::getOpenFileName(this, tr("Open DICOM File"), QDir::currentPath());
    drawDICOMImage(fileNameDICOM.toUtf8().constData());
}

void ReadDICOMQt::drawDICOMImage(std::string fileDICOM) {
  // Asigna carpeta de donde obtener las imágenes DICOM
  reader->SetFileName(fileDICOM.c_str());
  reader->Update();

  // Enlaza el input del viewer al output del reader
  viewer->SetInputConnection(reader->GetOutputPort());

  // Enlaza el widget con el viewer
  ui->qvtkWidget->SetRenderWindow(viewer->GetRenderWindow());

  // Añade interactor al viewer
  viewer->SetupInteractor(ui->qvtkWidget->GetInteractor());

  // Inicializa
  viewer->Render();

  // Actualiza labels de la GUI
  ui->labelFileName->setText(QString::fromStdString(fileDICOM));
}

void ReadDICOMQt::on_buttonOpenFile_clicked() {
      openDICOMFile();
}
