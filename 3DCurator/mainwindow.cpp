#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

	leftRen = vtkSmartPointer<vtkRenderer>::New();

	setBackgroundColor(leftRen, .1, .2, .3); // fondo azul oscuro
	ui->leftWidget->GetRenderWindow()->Render(); // renderiza para ver la ventana del color asignado
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
	ui->leftWidget->GetRenderWindow()->AddRenderer(leftRen); // asigna el renderer donde se visualizará en 3D el volumen al widget izquierdo
}

void MainWindow::drawVolume() {
	leftRen->AddVolume(figura->getVolume()); // añade el volumen al renderer
	leftRen->ResetCamera(); // resetea la cámera
	ui->leftWidget->GetRenderWindow()->Render(); // renderiza
} 

void MainWindow::on_actionOpenDICOM_triggered() {
	QString dicomFolder = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta DICOM"), QDir::homePath(), QFileDialog::ShowDirsOnly);

	if (dicomFolder != NULL) { // la carpeta se ha leído bien
		figura->setDICOMFolder(dicomFolder.toUtf8().constData());  // carga los archivos DICOM de la carpeta a la figura
		ui->labelFolder->setText(dicomFolder); // actualiza el label con el path de la carpeta con los archivos DICOM
		drawVolume(); // dibuja
	}
}

void MainWindow::on_updateTF_pressed() {
	figura->removeTFPoints();

	double value;
	Color color;

	if (ui->habilitar_1->isChecked()) {
		value = ui->v_1->value();
		color = Color(ui->r_1->value(), ui->g_1->value(), ui->b_1->value(), ui->a_1->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_2->isChecked()) {
		value = ui->v_2->value();
		color = Color(ui->r_2->value(), ui->g_2->value(), ui->b_2->value(), ui->a_2->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_3->isChecked()) {
		value = ui->v_3->value();
		color = Color(ui->r_3->value(), ui->g_3->value(), ui->b_3->value(), ui->a_3->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_4->isChecked()) {
		value = ui->v_4->value();
		color = Color(ui->r_4->value(), ui->g_4->value(), ui->b_4->value(), ui->a_4->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_5->isChecked()) {
		value = ui->v_5->value();
		color = Color(ui->r_5->value(), ui->g_5->value(), ui->b_5->value(), ui->a_5->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_6->isChecked()) {
		value = ui->v_6->value();
		color = Color(ui->r_6->value(), ui->g_6->value(), ui->b_6->value(), ui->a_6->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_7->isChecked()) {
		value = ui->v_7->value();
		color = Color(ui->r_7->value(), ui->g_7->value(), ui->b_7->value(), ui->a_7->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_8->isChecked()) {
		value = ui->v_8->value();
		color = Color(ui->r_8->value(), ui->g_8->value(), ui->b_8->value(), ui->a_8->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_9->isChecked()) {
		value = ui->v_9->value();
		color = Color(ui->r_9->value(), ui->g_9->value(), ui->b_9->value(), ui->a_9->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_10->isChecked()) {
		value = ui->v_10->value();
		color = Color(ui->r_10->value(), ui->g_10->value(), ui->b_10->value(), ui->a_10->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_11->isChecked()) {
		value = ui->v_11->value();
		color = Color(ui->r_11->value(), ui->g_11->value(), ui->b_11->value(), ui->a_11->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_12->isChecked()) {
		value = ui->v_12->value();
		color = Color(ui->r_12->value(), ui->g_12->value(), ui->b_12->value(), ui->a_12->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_13->isChecked()) {
		value = ui->v_13->value();
		color = Color(ui->r_13->value(), ui->g_13->value(), ui->b_13->value(), ui->a_13->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_14->isChecked()) {
		value = ui->v_14->value();
		color = Color(ui->r_14->value(), ui->g_14->value(), ui->b_14->value(), ui->a_14->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_15->isChecked()) {
		value = ui->v_15->value();
		color = Color(ui->r_15->value(), ui->g_15->value(), ui->b_15->value(), ui->a_15->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_16->isChecked()) {
		value = ui->v_16->value();
		color = Color(ui->r_16->value(), ui->g_16->value(), ui->b_16->value(), ui->a_16->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_17->isChecked()) {
		value = ui->v_17->value();
		color = Color(ui->r_17->value(), ui->g_17->value(), ui->b_17->value(), ui->a_17->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_18->isChecked()) {
		value = ui->v_18->value();
		color = Color(ui->r_18->value(), ui->g_18->value(), ui->b_18->value(), ui->a_18->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_19->isChecked()) {
		value = ui->v_19->value();
		color = Color(ui->r_19->value(), ui->g_19->value(), ui->b_19->value(), ui->a_19->value());
		figura->addPoint(value, color);
	}

	if (ui->habilitar_20->isChecked()) {
		value = ui->v_20->value();
		color = Color(ui->r_20->value(), ui->g_20->value(), ui->b_20->value(), ui->a_20->value());
		figura->addPoint(value, color);
	}

	ui->leftWidget->GetRenderWindow()->Render(); // renderiza
}


void MainWindow::on_actionExit_triggered() {
    exit(0);
}
