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

#define AIR_HU -1000
#define TOLERANCE 20
#define MIN_X 0
#define MIN_Y 0
#define MIN_Z 0
#define MAX_X 512
#define MAX_Y 512
#define MAX_Z 460

void deleteVoxels(vtkSmartPointer<vtkImageData> imageData, const int x, const int y, const int z, const double value, const double tolerance) {
	//std::cout << value << std::endl << imageData->GetScalarComponentAsDouble(x, y, z, 0) << std::endl << std::endl;
	if (x < MAX_X && y < MAX_Y && z < MAX_Z && x >= MIN_X && y >= MIN_Y && z >= MIN_Z)
		if (imageData->GetScalarComponentAsDouble(x, y, z, 0) >= value - tolerance && imageData->GetScalarComponentAsDouble(x, y, z, 0) <= value + tolerance) {
			std::cout << "Deleting (" << x << ", " << y << ", " << z << ")" << std::endl;

			imageData->SetScalarComponentFromDouble(x, y, z, 0, AIR_HU);

			deleteVoxels(imageData, x, y, z - 1, value, tolerance);
			deleteVoxels(imageData, x, y, z + 1, value, tolerance);

			deleteVoxels(imageData, x, y - 1, z, value, tolerance);
			deleteVoxels(imageData, x, y - 1, z - 1, value, tolerance);
			deleteVoxels(imageData, x, y - 1, z + 1, value, tolerance);

			deleteVoxels(imageData, x, y + 1, z, value, tolerance);
			deleteVoxels(imageData, x, y + 1, z - 1, value, tolerance);
			deleteVoxels(imageData, x, y + 1, z + 1, value, tolerance);


			deleteVoxels(imageData, x - 1, y, z, value, tolerance);
			deleteVoxels(imageData, x - 1, y, z - 1, value, tolerance);
			deleteVoxels(imageData, x - 1, y, z + 1, value, tolerance);

			deleteVoxels(imageData, x - 1, y - 1, z, value, tolerance);
			deleteVoxels(imageData, x - 1, y - 1, z - 1, value, tolerance);
			deleteVoxels(imageData, x - 1, y - 1, z + 1, value, tolerance);

			deleteVoxels(imageData, x - 1, y + 1, z, value, tolerance);
			deleteVoxels(imageData, x - 1, y + 1, z - 1, value, tolerance);
			deleteVoxels(imageData, x - 1, y + 1, z + 1, value, tolerance);


			deleteVoxels(imageData, x + 1, y, z, value, tolerance);
			deleteVoxels(imageData, x + 1, y, z - 1, value, tolerance);
			deleteVoxels(imageData, x + 1, y, z + 1, value, tolerance);

			deleteVoxels(imageData, x + 1, y - 1, z, value, tolerance);
			deleteVoxels(imageData, x + 1, y - 1, z - 1, value, tolerance);
			deleteVoxels(imageData, x + 1, y - 1, z + 1, value, tolerance);

			deleteVoxels(imageData, x + 1, y + 1, z, value, tolerance);
			deleteVoxels(imageData, x + 1, y + 1, z - 1, value, tolerance);
			deleteVoxels(imageData, x + 1, y + 1, z + 1, value, tolerance);
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
				std::cout << "Voxel value (before) is: " << value << std::endl;
				deleteVoxels(imageData, ijk[0], ijk[1], ijk[2], value, TOLERANCE);
				value = imageData->GetScalarComponentAsDouble(ijk[0], ijk[1], ijk[2], 0);
				std::cout << "Voxel value (after) is: " << value << std::endl;

				//for (int i = 0; i < 512; i++)
				//	for (int j = 0; j < 512; j++)
				//		for (int k = 0; k < 220; k++)
				//			imageData->SetScalarComponentFromDouble(i, j, k, 0, AIR_HU);

				std::cout << "finish" << std::endl;

				vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
				volumeMapper->SetBlendModeToComposite();
				volumeMapper->SetRequestedRenderModeToGPU();
				volumeMapper->SetInputData(imageData);
				volume->SetMapper(volumeMapper);
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
				this->GetDefaultRenderer()->SetBackground(0.3, 0.2, 0.1);
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
	renWin->SetSize(800, 800);
  
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