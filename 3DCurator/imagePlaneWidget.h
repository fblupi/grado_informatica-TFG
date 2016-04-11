#ifndef IMAGEPLANEWIDGET_H
#define IMAGEPLANEWIDGET_H

#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageViewer2.h"

/**
 * @class ImagePlaneWidget
 * Clase que hereda de la clase vtkImagePlaneWidget y redefine los m�todos de eventos producidos por el rat�n para que al moverlo, si previamente
 * se ha pulsado el bot�n derecho se actualice un ImageViewer2. Adem�s, desactiva los eventos producidos al hacer click en el b�t�n izquierdo y central
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
	bool moving = false;  /**< Variable booleana para saber si se est� moviendo o no el plano */
	vtkSmartPointer<vtkImageViewer2> viewer; /**< ImageViewer2 que se actualizar� cuando se mueva el plano */
};

#endif // IMAGEPLANEWIDGET_H