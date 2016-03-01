#ifndef PLANO_H
#define PLANO_H

#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkAlgorithm.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageViewer2.h"

class ImagePlaneWidget : public vtkImagePlaneWidget {
public:
	static ImagePlaneWidget* New();
	vtkTypeMacro(vtkImagePlaneWidget, ImagePlaneWidget);

	void SetViewer(vtkSmartPointer<vtkImageViewer2> viewer);

	virtual void OnMouseMove();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();

private:
	bool moving = false;
	vtkSmartPointer<vtkImageViewer2> viewer;
};

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