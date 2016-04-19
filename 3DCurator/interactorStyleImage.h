#ifndef INTERACTORSTYLEIMAGE_H
#define INTERACTORSTYLEIMAGE_H

#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleImage.h>

/**
 * @class ImagePlaneWidget
 * Clase que hereda de la clase vtkInteractorStyleImage y redefine el evento de pulsar el botón izquierdo del ratón para que no cambie el 
 * nivel de color y ventana del ImageViewer2
 */
class InteractorStyleImage : public vtkInteractorStyleImage {
public:
	static InteractorStyleImage* New();
	vtkTypeMacro(vtkInteractorStyleImage, InteractorStyleImage);

	virtual void OnLeftButtonDown();
};

#endif // INTERACTORSTYLEIMAGE_H