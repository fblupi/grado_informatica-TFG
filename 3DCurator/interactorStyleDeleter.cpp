#include "interactorStyleDeleter.h"

vtkStandardNewMacro(InteractorStyleDeleter);

void InteractorStyleDeleter::SetViewer(vtkSmartPointer<vtkImageViewer2> viewer) {
	this->viewer = viewer;
}

void InteractorStyleDeleter::SetDefaultRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin) {
	this->renWin = renWin;
}

void InteractorStyleDeleter::SetFigura(Figura* figura) {
	this->figura = figura;
}

void InteractorStyleDeleter::SetPlano(Plano* plano) {
	this->plano = plano;
}

void InteractorStyleDeleter::OnLeftButtonDown() {
	vtkSmartPointer<vtkVolumePicker> picker = vtkSmartPointer<vtkVolumePicker>::New();
	picker->SetUseVolumeGradientOpacity(true);
	picker->SetVolumeOpacityIsovalue(0.1);

	int* pos = this->GetInteractor()->GetEventPosition();
	picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

	int* ijk = picker->GetPointIJK();

	if (picker->GetPointId() != -1) {
		double value = figura->getImageData()->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0);
		int * dimensions = figura->getImageData()->GetDimensions();

		if (value > AIR_HU) {
			QProgressDialog progressDialog(NULL);
			progressDialog.setWindowTitle(QString("Borrando..."));
			progressDialog.setWindowFlags(progressDialog.windowFlags() & ~Qt::WindowCloseButtonHint);
			progressDialog.setCancelButton(0);
			progressDialog.show();

			deleteByImages(figura->getImageData(), ijk, 0, dimensions[0], 0, dimensions[1], 0, dimensions[2]);

			vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
			volumeMapper->SetInputData(figura->getImageData());
			figura->getVolume()->SetMapper(volumeMapper);

			plano->getPlane()->UpdatePlacement(); // Actualiza el plano para que se actualicen los cambios en el corte
			viewer->Render(); // Renderiza el corte

			progressDialog.close();
		}
	}

	vtkInteractorStyleTrackballCamera::OnLeftButtonDown(); // Forward events
}

std::pair<int, int> InteractorStyleDeleter::searchInitialVoxel(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y) {
	if (ijk[0] - 1 >= MIN_X 
		&& imageData->GetScalarComponentAsDouble(ijk[0] - 1, ijk[1], ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0] - 1, ijk[1]);
	}
	else if (ijk[0] + 1 < MAX_X 
		&& imageData->GetScalarComponentAsDouble(ijk[0] + 1, ijk[1], ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0] + 1, ijk[1]);
	}
	else if (ijk[1] - 1 >= MIN_Y 
		&& imageData->GetScalarComponentAsDouble(ijk[0], ijk[1] - 1, ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0], ijk[1] - 1);
	}
	else if (ijk[1] + 1 < MAX_Y 
		&& imageData->GetScalarComponentAsDouble(ijk[0], ijk[1] + 1, ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0], ijk[1] + 1);
	}
	else if (ijk[0] - 1 >= MIN_X && ijk[1] - 1 >= MIN_Y
		&& imageData->GetScalarComponentAsDouble(ijk[0] - 1, ijk[1] - 1, ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0] - 1, ijk[1] - 1);
	}
	else if (ijk[0] - 1 >= MIN_X && ijk[1] + 1 < MAX_Y
		&& imageData->GetScalarComponentAsDouble(ijk[0] - 1, ijk[1] + 1, ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0] - 1, ijk[1] + 1);
	}
	else if (ijk[0] + 1 < MAX_X && ijk[1] - 1 >= MIN_Y
		&& imageData->GetScalarComponentAsDouble(ijk[0] + 1, ijk[1] - 1, ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0] + 1, ijk[1] - 1);
	}
	else if (ijk[0] + 1 < MAX_X && ijk[1] + 1 < MAX_Y
		&& imageData->GetScalarComponentAsDouble(ijk[0] - 1, ijk[1] - 1, ijk[2], 0) < MIN_AIR) {
		return std::make_pair(ijk[0] + 1, ijk[1] + 1);
	}
	else {
		return std::make_pair(ijk[0], ijk[1]);
	}
}

void InteractorStyleDeleter::deleteImage(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y) {
	std::pair<int, int> ij;
	std::stack<std::pair<int, int>> stack;

	ij.first = ijk[0];
	ij.second = ijk[1];

	if (imageData->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0) < AIR_HU) {
		ij = searchInitialVoxel(imageData, ijk, MIN_X, MAX_X, MIN_Y, MAX_Y);
	}

	stack.push(ij);
	while (!stack.empty()) {
		ij = stack.top();
		stack.pop();
		if (ij.first < MAX_X && ij.first >= MIN_X && ij.second < MAX_Y && ij.second >= MIN_Y) {
			if (imageData->GetScalarComponentAsDouble(ij.first, ij.second, ijk[2], 0) >= MIN_AIR) {
				imageData->SetScalarComponentFromDouble(ij.first, ij.second, ijk[2], 0, AIR_HU);
				stack.push(std::make_pair(ij.first - 1, ij.second - 1));
				stack.push(std::make_pair(ij.first - 1, ij.second));
				stack.push(std::make_pair(ij.first - 1, ij.second + 1));
				stack.push(std::make_pair(ij.first, ij.second - 1));
				stack.push(std::make_pair(ij.first, ij.second + 1));
				stack.push(std::make_pair(ij.first + 1, ij.second - 1));
				stack.push(std::make_pair(ij.first + 1, ij.second));
				stack.push(std::make_pair(ij.first + 1, ij.second + 1));
			}
		}
	}
}

void InteractorStyleDeleter::deleteByImages(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y, const int MIN_Z, const int MAX_Z) {
	int xyz[3] = { ijk[0], ijk[1], ijk[2] };

	// Borrar actual
	deleteImage(imageData, xyz, MIN_X, MAX_X, MIN_Y, MAX_Y);

	// Borrar hacia arriba
	xyz[2] = ijk[2] + 1;
	while (xyz[2] < MAX_Z) {
		deleteImage(imageData, xyz, MIN_X, MAX_X, MIN_Y, MAX_Y);
		xyz[2] = xyz[2] + 1;
	}

	// Borrar hacia abajo
	xyz[2] = ijk[2] - 1;
	while (xyz[2] >= MIN_Z) {
		deleteImage(imageData, xyz, MIN_X, MAX_X, MIN_Y, MAX_Y);
		xyz[2] = xyz[2] - 1;
	}
}