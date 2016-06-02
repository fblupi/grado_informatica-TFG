#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVolumePicker.h>
#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkMath.h>

#include <stack>
#include <set>
#include <vector>
#include <time.h>

#define MIN_AIR -850
#define AIR_HU -1000
#define TOLERANCE 50

#define ISLANDS

void deleteIslands(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y, const int MIN_Z, const int MAX_Z) {
	double v;
	int i, j, k, iters = 0;
	std::vector<int> xyz(3), xyzNew(3);
	xyz[0] = ijk[0];
	xyz[1] = ijk[1];
	xyz[2] = ijk[2];
	std::stack<std::vector<int> > stack;
	stack.push(xyz);
	while (!stack.empty()) {
		xyz = stack.top();
		stack.pop();
		//std::cout << "Checking (" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << ")" << std::endl;
		if (xyz[0] < MAX_X && xyz[1] < MAX_Y && xyz[2] < MAX_Z && xyz[0] >= MIN_X && xyz[1] >= MIN_Y && xyz[2] >= MIN_Z) {
			v = imageData->GetScalarComponentAsDouble(xyz[0], xyz[1], xyz[2], 0);
			//std::cout << "Deleting (" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << ") = " << v << std::endl;
			if (v >= MIN_AIR) {
				imageData->SetScalarComponentFromDouble(xyz[0], xyz[1], xyz[2], 0, AIR_HU);
				for (i = -1; i < 2; i++) {
					for (j = -1; j < 2; j++) {
						for (k = -1; k < 2; k++) {
							if (!(i == 0 && j == 0 && k == 0)) {
								xyzNew[0] = xyz[0] + i;
								xyzNew[1] = xyz[1] + j;
								xyzNew[2] = xyz[2] + k;
								//std::cout << "Adding (" << xyzNew[0] << ", " << xyzNew[1] << ", " << xyzNew[2] << ")" << std::endl;
								stack.push(xyzNew);
							}
						}
					}
				}
			}
		}
		iters++;
	}
	std::cout << "Voxels deleted: " << iters << std::endl;
}

void deleteVoxelsIter(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const double value, const double tolerance, const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y, const int MIN_Z, const int MAX_Z) {
	double v;
	int i, j, k, iters = 0;
	std::vector<int> xyz(3), xyzNew(3);
	xyz[0] = ijk[0];
	xyz[1] = ijk[1];
	xyz[2] = ijk[2];
	std::stack<std::vector<int> > stack;
	stack.push(xyz);
	while (!stack.empty()) {
		xyz = stack.top();
		stack.pop();
		//std::cout << "Checking (" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << ")" << std::endl;
		if (xyz[0] < MAX_X && xyz[1] < MAX_Y && xyz[2] < MAX_Z && xyz[0] >= MIN_X && xyz[1] >= MIN_Y && xyz[2] >= MIN_Z) {
			v = imageData->GetScalarComponentAsDouble(xyz[0], xyz[1], xyz[2], 0);
			//std::cout << "Deleting (" << xyz[0] << ", " << xyz[1] << ", " << xyz[2] << ") = " << v << std::endl;
			if (v >= value - tolerance && v <= value + tolerance && v > AIR_HU) {
				imageData->SetScalarComponentFromDouble(xyz[0], xyz[1], xyz[2], 0, AIR_HU);
				for (i = -1; i < 2; i++) {
					for (j = -1; j < 2; j++) {
						for (k = -1; k < 2; k++) {
							if (!(i == 0 && j == 0 && k == 0)) {
								xyzNew[0] = xyz[0] + i;
								xyzNew[1] = xyz[1] + j;
								xyzNew[2] = xyz[2] + k;
								//std::cout << "Adding (" << xyzNew[0] << ", " << xyzNew[1] << ", " << xyzNew[2] << ")" << std::endl;
								stack.push(xyzNew);
							}
						}
					}
				}
			}
		}
		iters++;
	}
	std::cout << "Voxels deleted: " << iters << std::endl;
}

void deleteVoxelsRecur(vtkSmartPointer<vtkImageData> imageData, const int ijk[3], const double value, const double tolerance, const int MIN_X, const int MAX_X, const int MIN_Y, const int MAX_Y, const int MIN_Z, const int MAX_Z) {
	//std::cout << value << std::endl << imageData->GetScalarComponentAsDouble(x, y, z, 0) << std::endl << std::endl;
	int i, j, k, xyz[3];
	double v;
	for (i = ijk[0] - 1; i < ijk[0] + 2; i++) {
		for (j = ijk[1] - 1; j < ijk[1] + 2; j++) {
			for (k = ijk[2] - 1; k < ijk[2] + 2; k++) {
				if (i < MAX_X && j < MAX_Y && k < MAX_Z && i >= MIN_X && j >= MIN_Y && k >= MIN_Z) {
					v = imageData->GetScalarComponentAsDouble(i, j, k, 0);
					if (v >= value - tolerance && v <= value + tolerance) {
						//std::cout << "Deleting (" << x << ", " << y << ", " << z << ")" << std::endl;
						imageData->SetScalarComponentFromDouble(ijk[0], ijk[1], ijk[2], 0, AIR_HU);
						xyz[0] = i; 
						xyz[1] = j; 
						xyz[2] = k;
						deleteVoxelsRecur(imageData, xyz, value, tolerance, MIN_X, MAX_X, MIN_Y, MAX_Y, MIN_Z, MAX_Z);
					}
				}
			}
		}
	}
}

class MouseIteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	static MouseIteractorStyle* New();
	vtkTypeMacro(vtkInteractorStyleTrackballCamera, MouseIteractorStyle);

	void SetDefaultRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin) {
		this->renWin = renWin;
	}

	void SetImageData(vtkSmartPointer<vtkImageData> imageData) {
		this->imageData = imageData;
	}

	void SetVolume(vtkSmartPointer<vtkVolume> volume) {
		this->volume = volume;
	}

	virtual void OnLeftButtonDown() {
		if (removing) {
			vtkSmartPointer<vtkVolumePicker> picker = vtkSmartPointer<vtkVolumePicker>::New();
			picker->SetUseVolumeGradientOpacity(true);
			picker->SetVolumeOpacityIsovalue(0.1);

			int *pos = this->GetInteractor()->GetEventPosition();
			picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

			int *ijk = picker->GetPointIJK();
			std::cout << "Pick position is: " << ijk[0] << " " << ijk[1] << " " << ijk[2] << std::endl;

			if (picker->GetPointId() != -1) {
				double value = imageData->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0);
				int *dimensions = imageData->GetDimensions();
				if (value > -1000) {
					std::cout << "Voxel value (before) is: " << value << std::endl;
					std::cout << "Working..." << std::endl;
					clock_t t = clock();
#ifdef ISLANDS
					deleteIslands(imageData, ijk, 0, dimensions[0], 0, dimensions[1], 0, dimensions[2]);
#else
					deleteVoxelsIter(imageData, ijk, value, TOLERANCE, 0, dimensions[0], 0, dimensions[1], 0, dimensions[2]);
#endif
					t = clock() - t;
					std::cout << "It took " << ((float) t) / CLOCKS_PER_SEC << " secconds" << std::endl;
					value = imageData->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0);
					std::cout << "Voxel value (after) is: " << value << std::endl;

					vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
					volumeMapper->SetBlendModeToComposite();
					volumeMapper->SetRequestedRenderModeToGPU();
					volumeMapper->SetInputData(imageData);
					volume->SetMapper(volumeMapper);
				}
			}
		}

		vtkInteractorStyleTrackballCamera::OnLeftButtonDown(); // Forward events
	}

	virtual void OnKeyPress() {
		if (this->Interactor->GetKeySym() == std::string("Delete")) {
			if (removing) {
				removing = false;
				this->GetDefaultRenderer()->SetBackground(0.1, 0.2, 0.3);
				renWin->Render();
			} else {
				removing = true;
				this->GetDefaultRenderer()->SetBackground(0.2, 0.3, 0.1);
				renWin->Render();
			}
		}
	}
private:
	bool removing = false;
	vtkSmartPointer<vtkRenderWindow> renWin;
	vtkSmartPointer<vtkImageData> imageData;
	vtkSmartPointer<vtkVolume> volume;
};

vtkStandardNewMacro(MouseIteractorStyle);

int main(int argc, char *argv[]) {

	vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<MouseIteractorStyle> style = vtkSmartPointer<MouseIteractorStyle>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
	vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();

	reader->SetDirectoryName("C:\\Users\\FranciscoJavier\\Dropbox\\Facultad\\Grado Informatica\\TFG\\DICOM\\Inmaculada Concepcion\\SE000000");
	reader->Update();
	imageData->ShallowCopy(reader->GetOutput());
  
	ren1->SetBackground(0.1, 0.2, 0.3);
  
	renWin->AddRenderer(ren1);
	renWin->SetSize(600, 600);
  
	style->SetDefaultRenderer(ren1);
	style->SetDefaultRenderWindow(renWin);
	style->SetImageData(imageData);
	style->SetVolume(volume);
	iren->SetInteractorStyle(style);
	iren->SetRenderWindow(renWin);
  
	volumeMapper->SetBlendModeToComposite();
	volumeMapper->SetRequestedRenderModeToGPU();
	//volumeMapper->SetInputConnection(reader->GetOutputPort());
	volumeMapper->SetInputData(imageData);

	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationTypeToLinear();
	volumeProperty->SetAmbient(0.1);
	volumeProperty->SetDiffuse(0.9);
	volumeProperty->SetSpecular(0.2);
	volumeProperty->SetSpecularPower(10.0);

	gradientOpacity->AddPoint(0.0, 0.0);
	gradientOpacity->AddPoint(2000.0, 1.0);
	volumeProperty->SetGradientOpacity(gradientOpacity);
  
	scalarOpacity->AddPoint(-800.0, 0.0);
	scalarOpacity->AddPoint(-750.0, 1.0);
	scalarOpacity->AddPoint(-350.0, 1.0);
	scalarOpacity->AddPoint(-300.0, 0.0);
	scalarOpacity->AddPoint(-200.0, 0.0);
	scalarOpacity->AddPoint(-100.0, 1.0);
	scalarOpacity->AddPoint(1000.0, 0.0);
	scalarOpacity->AddPoint(2750.0, 0.0);
	scalarOpacity->AddPoint(2976.0, 1.0);
	scalarOpacity->AddPoint(3000.0, 0.0);
	volumeProperty->SetScalarOpacity(scalarOpacity);
  
	color->AddRGBPoint(-750.0, 0.08, 0.05, 0.03);
	color->AddRGBPoint(-350.0, 0.39, 0.25, 0.16);
	color->AddRGBPoint(-200.0, 0.80, 0.80, 0.80);
	color->AddRGBPoint(2750.0, 0.70, 0.70, 0.70);
	color->AddRGBPoint(3000.0, 0.35, 0.35, 0.35);
	volumeProperty->SetColor(color);
    
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	ren1->AddVolume(volume);
	ren1->ResetCamera();
  
	renWin->Render();
	iren->Start();
  
	return EXIT_SUCCESS;

}