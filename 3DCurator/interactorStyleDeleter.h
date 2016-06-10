#ifndef INTERACTORSTYLEDELETER_H
#define INTERACTORSTYLEDELETER_H

#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkVolumePicker.h>

#include <stack>
#include "figura.h"

#define MIN_AIR -850
#define AIR_HU -1000

/**
 * @class InteractorStyleDeleter
 * Clase que hereda de la clase vtkInteractorStyleTrackballCamera y que al pulsar sobre un voxel borra todos los que forman
 * parte de esa parte de la figura rodeada de aire
 */
class InteractorStyleDeleter : public vtkInteractorStyleTrackballCamera {
public:
	static InteractorStyleDeleter* New();
	vtkTypeMacro(vtkInteractorStyleTrackballCamera, InteractorStyleDeleter);

	void SetDefaultRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin);
	void SetFigura(const Figura* figura);

	virtual void OnLeftButtonDown();

private:
	vtkSmartPointer<vtkRenderWindow> renWin;
	Figura* figura;

	std::pair<int, int> searchInitialVoxel(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y);
	void deleteImage(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y);
	void deleteByImages(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y, const int MIN_Z, const int MAX_Z);
};

#endif // INTERACTORSTYLEDELETER_H