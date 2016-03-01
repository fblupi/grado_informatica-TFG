#ifndef PLANO_H
#define PLANO_H

#include "imagePlaneWidget.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkAlgorithm.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageViewer2.h"

class Plano {
public:
	// constructor/destructor
	Plano();
	~Plano();

	// getters
	vtkSmartPointer<ImagePlaneWidget> getPlane() const;

	// funcs
	void setInputConnection(vtkSmartPointer<vtkAlgorithm> reader);
	void setViewer(vtkSmartPointer<vtkImageViewer2> viewer);
	void enable(const bool onOff);
	void setOrigin(const double x, const double y, const double z);
	void setSagital();
	void setCoronal();
	void setAxial();

private:
	// atributos
	vtkSmartPointer<ImagePlaneWidget> plane;
	double centers[3]; // posiciones centrales para los planos sagital, coronal y axial
};

#endif // PLANO_H