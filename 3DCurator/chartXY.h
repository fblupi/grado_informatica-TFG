#ifndef CHARTXY_H
#define CHARTXY_H

#include "vtkChartXY.h"
#include "vtkObjectFactory.h"
#include "vtkContextMouseEvent.h"

class ChartXY : public vtkChartXY {

public:
	static ChartXY* New();
	vtkTypeMacro(vtkChartXY, ChartXY);

	virtual bool MouseEnterEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseMoveEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseLeaveEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseButtonPressEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse);
	virtual bool MouseWheelEvent(const vtkContextMouseEvent &mouse, int delta);
};

#endif