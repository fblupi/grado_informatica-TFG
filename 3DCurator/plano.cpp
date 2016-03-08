#include "plano.h"

Plano::Plano() {
	plane = vtkSmartPointer<ImagePlaneWidget>::New();
	centers[0] = centers[1] = centers[2] = 0.0;
}

vtkSmartPointer<ImagePlaneWidget> Plano::getPlane() const {
	return plane;
}

void Plano::setInputConnection(vtkSmartPointer<vtkAlgorithm> reader) {
	plane->SetInputConnection(reader->GetOutputPort());
}

void Plano::setViewer(vtkSmartPointer<vtkImageViewer2> viewer) {
	plane->SetViewer(viewer);
}

void Plano::enable(const bool onOff) {
	plane->SetEnabled(onOff);
}

void Plano::show(const bool onOff) {
	plane->SetTextureVisibility(onOff); // muestra la imagen del corte
}

void Plano::setOrigin(const double x, const double y, const double z) {
	plane->SetOrigin(x, y, z);
	centers[0] = x;
	centers[1] = y;
	centers[2] = z;
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