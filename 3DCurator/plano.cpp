#include "plano.h"

Plano::Plano() {
	plane = vtkSmartPointer<vtkImagePlaneWidget>::New();
	centers[0] = centers[1] = centers[2] = 0.0;
}

vtkSmartPointer<vtkImagePlaneWidget> Plano::getPlane() const {
	return plane;
}

void Plano::enable(const bool onOff) {
	if (onOff) {
		plane->On();
	} else {
		plane->Off();
	}
}

void Plano::setOrigin(const double x, const double y, const double z) {
	plane->SetOrigin(x, y, z);
	centers[0] = x;
	centers[1] = y;
	centers[2] = z;
}

void Plano::setInputConnection(vtkSmartPointer<vtkAlgorithm> reader) {
	plane->SetInputConnection(reader->GetOutputPort());
}

void Plano::setSagital() {
	plane->SetPlaneOrientationToXAxes();
	plane->SetSlicePosition(centers[0]);
}

void Plano::setCoronal() {
	plane->SetPlaneOrientationToYAxes();
	plane->SetSlicePosition(centers[1]);
}

void Plano::setAxial() {
	plane->SetPlaneOrientationToZAxes();
	plane->SetSlicePosition(centers[2]);
}