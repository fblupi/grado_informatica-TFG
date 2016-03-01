#include "plano.h"

vtkStandardNewMacro(ImagePlaneWidget);

void ImagePlaneWidget::SetViewer(vtkSmartPointer<vtkImageViewer2> viewer) {
	this->viewer = viewer;
}

void ImagePlaneWidget::OnMouseMove() {
	if (moving && viewer != NULL) {
		viewer->Render();
	}
	vtkImagePlaneWidget::OnMouseMove(); // forward events
}

void ImagePlaneWidget::OnMiddleButtonDown() {
	moving = true;
	vtkImagePlaneWidget::OnMiddleButtonDown(); // forward events
}

void ImagePlaneWidget::OnMiddleButtonUp() {
	moving = false;
	vtkImagePlaneWidget::OnMiddleButtonUp(); // forward events
}

/*************************************************************************************/

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