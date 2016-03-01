#include "imagePlaneWidget.h"

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