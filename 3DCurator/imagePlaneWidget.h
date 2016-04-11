#ifndef IMAGEPLANEWIDGET_H
#define IMAGEPLANEWIDGET_H

#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageViewer2.h"

/**
 * @class ImagePlaneWidget
 * Clase que hereda de la clase vtkImagePlaneWidget y redefine los métodos de eventos producidos por el ratón para que al moverlo, si previamente
 * se ha pulsado el botón derecho se actualice un ImageViewer2. Además, desactiva los eventos producidos al hacer click en el bótón izquierdo y central
 */
class ImagePlaneWidget : public vtkImagePlaneWidget {
public:
	static ImagePlaneWidget* New();
	vtkTypeMacro(vtkImagePlaneWidget, ImagePlaneWidget);

	void SetViewer(vtkSmartPointer<vtkImageViewer2> viewer); /**< Establece el ImageViewer2 */

	virtual void OnMouseMove();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();

private:
	bool moving = false;  /**< Variable booleana para saber si se está moviendo o no el plano */
	vtkSmartPointer<vtkImageViewer2> viewer; /**< ImageViewer2 que se actualizará cuando se mueva el plano */
};

#endif // IMAGEPLANEWIDGET_H