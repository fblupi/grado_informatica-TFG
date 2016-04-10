#ifndef INTERACTORSTYLEIMAGE_H
#define INTERACTORSTYLEIMAGE_H

#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkInteractorStyleImage.h"

class InteractorStyleImage : public vtkInteractorStyleImage {

public:
	static InteractorStyleImage* New();
	vtkTypeMacro(vtkInteractorStyleImage, InteractorStyleImage);

	virtual void OnLeftButtonDown();
};

#endif // INTERACTORSTYLEIMAGE_H