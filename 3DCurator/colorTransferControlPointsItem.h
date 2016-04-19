#ifndef COLORTRANSFERCONTROLPOINTSITEM_H
#define COLORTRANSFERCONTROLPOINTSITEM_H

#include <vtkColorTransferFunction.h>
#include <vtkColorTransferControlPointsItem.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkObjectFactory.h>
#include <vtkContextMouseEvent.h>
#include <vtkContextKeyEvent.h>
#include <vtkRenderWindowInteractor.h>

#include <QColorDialog>

/**
 * @class ColorTransferControlPointsItem
 * Clase que hereda de la clase vtkColorTransferControlPointsItem y redefine los métodos de eventos producidos por el ratón
 * para que al realizar cualquier cambio sobre la función de transferencia se actualice un RenderWindow. Además se añade una 
 * funcionalidad al doble click para que aparezca un selector de color para cambiar el color de ese punto
 */
class ColorTransferControlPointsItem : public vtkColorTransferControlPointsItem {
public:
	static ColorTransferControlPointsItem* New();
	vtkTypeMacro(vtkColorTransferControlPointsItem, ColorTransferControlPointsItem);

	void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin); /**< Establece el RenderWindow */

	virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseDoubleClickEvent(const vtkContextMouseEvent &mouse);
	virtual bool KeyReleaseEvent(const vtkContextKeyEvent &key);

private:
	vtkSmartPointer<vtkRenderWindow> renWin; /**< RenderWindow que se actualizará cuando se produzca un cambio en la función de transferencia */
};

#endif