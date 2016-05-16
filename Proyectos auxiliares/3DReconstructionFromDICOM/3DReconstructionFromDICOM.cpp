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

class MouseIteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	static MouseIteractorStyle* New();
	vtkTypeMacro(vtkInteractorStyleTrackballCamera, MouseIteractorStyle);

	void SetDefaultRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin) {
		this->renWin = renWin;
	}

	virtual void OnLeftButtonDown() {
		if (removing) {
			int *pos = this->GetInteractor()->GetEventPosition();

			vtkSmartPointer<vtkVolumePicker> picker = vtkSmartPointer<vtkVolumePicker>::New();
			picker->SetUseVolumeGradientOpacity(true);
			picker->SetVolumeOpacityIsovalue(0.2);

			picker->Pick(pos[0], pos[1], pos[2], this->GetDefaultRenderer());

			double *worldPosition = picker->GetPickPosition();
			std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1] << " " << worldPosition[2] << std::endl;
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
};

vtkStandardNewMacro(MouseIteractorStyle);

int main(int argc, char *argv[]) {

  vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
  reader->SetDirectoryName("C:\\Users\\FranciscoJavier\\Dropbox\\Facultad\\Grado Informatica\\TFG\\DICOM\\Inmaculada Concepcion\\SE000000");
  reader->Update();
  
  vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderer> ren1 = vtkSmartPointer<vtkRenderer>::New();
  ren1->SetBackground(0.1, 0.2, 0.3);
  
  renWin->AddRenderer(ren1);
  renWin->SetSize(800, 800);
  
  vtkSmartPointer<MouseIteractorStyle> style = vtkSmartPointer<MouseIteractorStyle>::New();
  style->SetDefaultRenderer(ren1);
  style->SetDefaultRenderWindow(renWin);
  vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetInteractorStyle(style);
  iren->SetRenderWindow(renWin);
  
  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper->SetBlendModeToComposite();
  volumeMapper->SetRequestedRenderModeToGPU();
  volumeMapper->SetInputConnection(reader->GetOutputPort());

  vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->ShadeOn();
  volumeProperty->SetInterpolationTypeToLinear();
  volumeProperty->SetAmbient(0.1);
  volumeProperty->SetDiffuse(0.9);
  volumeProperty->SetSpecular(0.2);
  volumeProperty->SetSpecularPower(10.0);

  vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
  gradientOpacity->AddPoint(0.0, 0.0);
  gradientOpacity->AddPoint(2000.0, 1.0);
  volumeProperty->SetGradientOpacity(gradientOpacity);
  
  vtkSmartPointer<vtkPiecewiseFunction> scalarOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
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
  
  vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
  color->AddRGBPoint(-750.0, 0.08, 0.05, 0.03);
  color->AddRGBPoint(-350.0, 0.39, 0.25, 0.16);
  color->AddRGBPoint(-200.0, 0.80, 0.80, 0.80);
  color->AddRGBPoint(2750.0, 0.70, 0.70, 0.70);
  color->AddRGBPoint(3000.0, 0.35, 0.35, 0.35);
  volumeProperty->SetColor(color);
    
  vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);
  ren1->AddVolume(volume);
  ren1->ResetCamera();
  
  renWin->Render();
  iren->Start();
  
  return EXIT_SUCCESS;

}