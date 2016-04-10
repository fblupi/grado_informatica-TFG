#ifndef PIECEWISECONTROLPOINTSITEM_H
#define PIECEWISECONTROLPOINTSITEM_H

#include "vtkPiecewiseControlPointsItem.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkObjectFactory.h"
#include "vtkContextMouseEvent.h"
#include "vtkContextKeyEvent.h"
#include "vtkRenderWindowInteractor.h"

class PiecewiseControlPointsItem : public vtkPiecewiseControlPointsItem {

public:
	static PiecewiseControlPointsItem* New();
	vtkTypeMacro(vtkPiecewiseControlPointsItem, PiecewiseControlPointsItem);

	void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin);

	virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse);
	virtual bool KeyReleaseEvent(const vtkContextKeyEvent &key);

private:
	vtkSmartPointer<vtkRenderWindow> renWin;
};

#endif