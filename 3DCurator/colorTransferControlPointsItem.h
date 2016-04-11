#ifndef COLORTRANSFERCONTROLPOINTSITEM_H
#define COLORTRANSFERCONTROLPOINTSITEM_H

#include "vtkColorTransferFunction.h"
#include "vtkColorTransferControlPointsItem.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkObjectFactory.h"
#include "vtkContextMouseEvent.h"
#include "vtkContextKeyEvent.h"
#include "vtkRenderWindowInteractor.h"

#include <QColorDialog>

class ColorTransferControlPointsItem : public vtkColorTransferControlPointsItem {

public:
	static ColorTransferControlPointsItem* New();
	vtkTypeMacro(vtkColorTransferControlPointsItem, ColorTransferControlPointsItem);

	void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin);

	virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseDoubleClickEvent(const vtkContextMouseEvent &mouse);
	virtual bool KeyReleaseEvent(const vtkContextKeyEvent &key);

private:
	vtkSmartPointer<vtkRenderWindow> renWin;
};

#endif