#include "piecewiseControlPointsItem.h"

vtkStandardNewMacro(PiecewiseControlPointsItem);

void PiecewiseControlPointsItem::SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin) {
	this->renWin = renWin;
}

bool PiecewiseControlPointsItem::MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse) {
	bool result = vtkPiecewiseControlPointsItem::MouseButtonReleaseEvent(mouse);
	if (mouse.GetButton() == vtkContextMouseEvent::LEFT_BUTTON || mouse.GetButton() == vtkContextMouseEvent::MIDDLE_BUTTON) {
		renWin->Render();
	}
	return result;
}

bool PiecewiseControlPointsItem::KeyReleaseEvent(const vtkContextKeyEvent &key) {
	bool result = vtkPiecewiseControlPointsItem::KeyReleaseEvent(key);
	if (key.GetInteractor()->GetKeySym() == std::string("Delete") || key.GetInteractor()->GetKeySym() == std::string("BackSpace")) {
		renWin->Render();
	}
	return result;
}