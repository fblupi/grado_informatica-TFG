#ifndef PIECEWISECONTROLPOINTSITEM_H
#define PIECEWISECONTROLPOINTSITEM_H

#include <vtkPiecewiseControlPointsItem.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkObjectFactory.h>
#include <vtkContextMouseEvent.h>
#include <vtkContextKeyEvent.h>
#include <vtkRenderWindowInteractor.h>

/**
 * @class PiecewiseControlPointsItem
 * Clase que hereda de la clase vtkPiecewiseControlPointsItem y redefine los m�todos de eventos producidos por el rat�n
 * para que al realizar cualquier cambio sobre la funci�n de transferencia se actualice un RenderWindow.
 */
class PiecewiseControlPointsItem : public vtkPiecewiseControlPointsItem {
public:
	static PiecewiseControlPointsItem* New();
	vtkTypeMacro(vtkPiecewiseControlPointsItem, PiecewiseControlPointsItem);

	void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin);  /**< Establece el RenderWindow */

	virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse);
	virtual bool KeyReleaseEvent(const vtkContextKeyEvent &key);

private:
	vtkSmartPointer<vtkRenderWindow> renWin; /**< RenderWindow que se actualizar� cuando se produzca un cambio en la funci�n de transferencia */
};

#endif