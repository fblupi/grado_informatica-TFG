#include "colorTransferControlPointsItem.h"

vtkStandardNewMacro(ColorTransferControlPointsItem);

void ColorTransferControlPointsItem::SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin) {
	this->renWin = renWin;
}

bool ColorTransferControlPointsItem::MouseButtonReleaseEvent(const vtkContextMouseEvent &mouse) {
	bool result = vtkColorTransferControlPointsItem::MouseButtonReleaseEvent(mouse);
	if (mouse.GetButton() == vtkContextMouseEvent::LEFT_BUTTON  || mouse.GetButton() == vtkContextMouseEvent::MIDDLE_BUTTON) {
		renWin->Render();
	}
	return result;
}

bool ColorTransferControlPointsItem::KeyReleaseEvent(const vtkContextKeyEvent &key) {
	bool result = vtkColorTransferControlPointsItem::KeyReleaseEvent(key);
	if (key.GetInteractor()->GetKeySym() == std::string("Delete") || key.GetInteractor()->GetKeySym() == std::string("BackSpace")) {
		renWin->Render();
	}
	return result;
}

bool ColorTransferControlPointsItem::MouseDoubleClickEvent(const vtkContextMouseEvent &mouse) {
	if (mouse.GetButton() == vtkContextMouseEvent::LEFT_BUTTON) {
		QColor color = QColorDialog::getColor();
		if (color.isValid()) {
			double values[6];
			ColorTransferFunction->GetNodeValue(this->GetCurrentPoint(), values);
			values[1] = color.redF();
			values[2] = color.greenF();
			values[3] = color.blueF();
			ColorTransferFunction->SetNodeValue(this->GetCurrentPoint(), values);
		}
	}
	return vtkColorTransferControlPointsItem::MouseDoubleClickEvent(mouse);
}