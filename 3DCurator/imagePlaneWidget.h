#ifndef IMAGEPLANEWIDGET_H
#define IMAGEPLANEWIDGET_H

#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
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

#endif // IMAGEPLANEWIDGET_H