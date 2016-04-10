#include "chartXY.h"

vtkStandardNewMacro(ChartXY);	

bool ChartXY::MouseEnterEvent(const vtkContextMouseEvent &mouse) {
	return true;
}

bool ChartXY::MouseMoveEvent(const vtkContextMouseEvent &mouse) {
	return true;
}

bool ChartXY::MouseLeaveEvent(const vtkContextMouseEvent &mouse) {
	return true;
}

bool ChartXY::MouseButtonPressEvent(const vtkContextMouseEvent &mouse) {
	return true;
}

bool ChartXY::MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse) {
	return true;
}

bool ChartXY::MouseWheelEvent(const vtkContextMouseEvent &mouse, int delta) {
	return true;
}
