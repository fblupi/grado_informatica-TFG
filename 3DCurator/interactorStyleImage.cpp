#include "interactorStyleImage.h"

vtkStandardNewMacro(InteractorStyleImage);

void InteractorStyleImage::SetPlano(Plano* plano) {
	this->plano = plano;
}

void InteractorStyleImage::SetLabel(QLabel* label) {
	this->label = label;
}

void InteractorStyleImage::OnLeftButtonDown() {
	// idle
}

void InteractorStyleImage::OnMouseMove() {
	if (plano != NULL && label != NULL && this->GetDefaultRenderer() != NULL) {
		vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
		picker->SetTolerance(0.0005);

		int* pos = this->GetInteractor()->GetEventPosition();
		picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

		int* ijk = picker->GetPointIJK();

		if (picker->GetPointId() != -1) {
			float value = plano->getPlane()->GetResliceOutput()->GetScalarComponentAsFloat(ijk[0], ijk[1], ijk[2], 0);
			label->setText(QString::fromStdString("HU: " + std::to_string((int) value)));
		} else {
			label->setText(QString::fromStdString("HU: Fuera de rango"));
		}
	}
	vtkInteractorStyleImage::OnMouseMove(); // Forward events
}