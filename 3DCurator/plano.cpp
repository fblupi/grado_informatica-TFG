#include "plano.h"

Plano::Plano() {
	plane = vtkSmartPointer<vtkImagePlaneWidget>::New();
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
}

void Plano::setInputConnection(vtkSmartPointer<vtkAlgorithm> reader) {
	plane->SetInputConnection(reader->GetOutputPort());
}

void Plano::placeWidget(const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax) {
	plane->PlaceWidget(xMin, xMax, yMin, yMax, zMin, zMax);
}

void Plano::setSagital() {
	plane->SetPlaneOrientationToXAxes();
}

void Plano::setCoronal() {
	plane->SetPlaneOrientationToYAxes();
}

void Plano::setAxial() {
	plane->SetPlaneOrientationToZAxes();
}