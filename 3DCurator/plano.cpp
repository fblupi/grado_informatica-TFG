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

	// Obtiene los puntos y el origen del plano sagital
	double p1[3] = {
		plane->GetPoint1()[0],
		plane->GetPoint1()[1],
		plane->GetPoint1()[2]
	};
	double p2[3] = {
		plane->GetPoint2()[0],
		plane->GetPoint2()[1],
		plane->GetPoint2()[2]
	};
	double o[3] = {
		plane->GetOrigin()[0],
		plane->GetOrigin()[1],
		plane->GetOrigin()[2]
	};

	// Hace un giro de 180º sobre el eje X
	plane->SetPoint1(p1[0], p1[2], p2[2]);
	plane->SetPoint2(p2[0], p1[1], p2[1]);
	plane->SetOrigin(o[0],  p1[1], p2[2]);

	plane->UpdatePlacement();
}

void Plano::setCoronal() {
	plane->SetPlaneOrientationToXAxes();
	plane->SetSlicePosition(centers[0]);

	// Obtiene los puntos y el origen del plano sagital
	double p1[3] = {
		plane->GetPoint1()[0],
		plane->GetPoint1()[1],
		plane->GetPoint1()[2]
	};
	double p2[3] = {
		plane->GetPoint2()[0],
		plane->GetPoint2()[1],
		plane->GetPoint2()[2]
	};
	double o[3] = {
		plane->GetOrigin()[0],
		plane->GetOrigin()[1],
		plane->GetOrigin()[2]
	};

	// Hace un giro de 90º sobre el eje Z
	plane->SetPoint1(p1[1], p1[0], p2[2]);
	plane->SetPoint2(p1[2], p2[0], p2[1]);
	plane->SetOrigin(o[2],  o[0],  p2[2]);

	plane->UpdatePlacement();
}

void Plano::setAxial() {
	plane->SetPlaneOrientationToZAxes();
	plane->SetSlicePosition(centers[2]);
}