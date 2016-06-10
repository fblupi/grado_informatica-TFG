#include "plano.h"

Plano::Plano() {
	plane = vtkSmartPointer<ImagePlaneWidget>::New();
	centers[0] = centers[1] = centers[2] = 0.0;
}

vtkSmartPointer<ImagePlaneWidget> Plano::getPlane() const {
	return plane;
}

void Plano::setInputData(vtkSmartPointer<vtkImageData> imageData) {
	//plane->SetInputConnection(reader->GetOutputPort());
	plane->SetInputData(imageData);
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
	// coloca el plano sobre los ejes YZ (pero hay que darle la vuelta)
	plane->SetPlaneOrientationToXAxes();
	plane->SetSlicePosition(centers[0]);

	// obtiene los puntos del plano
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

	// hace un giro de 180º sobre el eje X
	plane->SetPoint1(p1[0], p2[1], p2[2]);
	plane->SetPoint2(p1[0], p1[1], p1[2]);
	plane->SetOrigin(p1[0], p1[1], p2[2]);

	plane->UpdatePlacement();
}

void Plano::setCoronal() {
	// coloca el plano sobre los ejes YZ (pero hay que darle la vuelta y ponerlo sobre el XZ)
	plane->SetPlaneOrientationToXAxes();
	plane->SetSlicePosition(centers[0]);

	// obtiene los puntos del plano sagital
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

	// hace un giro de 180º sobre el eje X y otro de 90º sobre el eje Z
	plane->SetPoint1(p2[1], p1[0], p2[2]);
	plane->SetPoint2(p1[1], p1[0], p1[2]);
	plane->SetOrigin(p1[1], p1[0], p2[2]);

	plane->UpdatePlacement();
}

void Plano::setAxial() {
	// coloca el plano sobre los ejes XY
	plane->SetPlaneOrientationToZAxes();
	plane->SetSlicePosition(centers[2]);
}