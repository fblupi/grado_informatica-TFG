#ifndef PLANO_H
#define PLANO_H

#include "vtkSmartPointer.h"
#include "vtkAlgorithm.h"
#include "vtkImagePlaneWidget.h"

class Plano {
public:
	// constructor/destructor
	Plano();
	~Plano();

	// getters
	vtkSmartPointer<vtkImagePlaneWidget> getPlane() const;

	// funcs
	void enable(const bool onOff);
	void setOrigin(const double x, const double y, const double z);
	void placeWidget(const double xMin, const double xMax, const double yMin, const double yMax, const double zMin, const double zMax);
	void setInputConnection(vtkSmartPointer<vtkAlgorithm> reader);
	void setSagital();
	void setCoronal();
	void setAxial();

private:
	// atributos
	vtkSmartPointer<vtkImagePlaneWidget> plane;
};

#endif // PLANO_H