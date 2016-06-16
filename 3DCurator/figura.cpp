#include "figura.h"

Figura::Figura() {
	tf = new TransferFunction();
	isoValue = WOOD_ISOVALUE;
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	imageData = vtkSmartPointer<vtkImageData>::New();
	meshActor = vtkSmartPointer<vtkActor>::New();
	//histogram = vtkSmartPointer<vtkImageAccumulate>::New();
	//histogram->SetComponentExtent(-750, 3000, 0, 3000, 0, 0);
    volume = vtkSmartPointer<vtkVolume>::New();
	setProperties(); // asigna propiedades del volumen (material, función de transferencia...)
	volume->SetMapper(mapper);
    volume->SetProperty(volumeProperty);
}

Figura::~Figura() {

}

TransferFunction * Figura::getTransferFunction() const {
	return tf;
}

vtkSmartPointer<vtkVolume> Figura::getVolume() const {
	return volume;
}

vtkSmartPointer<vtkImageData> Figura::getImageData() const {
	return imageData;
}

vtkSmartPointer<vtkImageAccumulate> Figura::getHistogram() const {
	return histogram;
}

vtkSmartPointer<vtkActor> Figura::getMesh() const {
	return meshActor;
}

double Figura::getIsoValue() const {
	return isoValue;
}

double Figura::getMinXBound() const {
	return volume->GetMinXBound();
}

double Figura::getMinYBound() const {
	return volume->GetMinYBound();
}

double Figura::getMinZBound() const {
	return volume->GetMinZBound();
}

double Figura::getMaxXBound() const {
	return volume->GetMaxXBound();
}

double Figura::getMaxYBound() const {
	return volume->GetMaxYBound();
}

double Figura::getMaxZBound() const {
	return volume->GetMaxZBound();
}

void Figura::setDICOMFolder(const std::string s) {
	// Lee los datos
	vtkSmartPointer<vtkDICOMImageReader> imageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    imageReader->SetDirectoryName(s.c_str()); // asigna la carpeta al image reader
    imageReader->Update(); // lee los archivos

	// Guarda los datos del volumen
	//imageData = vtkSmartPointer<vtkImageData>::New();
	imageData->ShallowCopy(imageReader->GetOutput());

	// Crea y asigna el mapper
	//mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	mapper->SetInputData(imageData); // conecta el mapper con el reader
	volume->SetMapper(mapper);

	//histogram->SetInputConnection(reader->GetOutputPort());
}

void Figura::createMesh() {
	vtkSmartPointer<vtkMarchingCubes> surface = vtkSmartPointer<vtkMarchingCubes>::New();
	surface->SetInputData(imageData);
	surface->UpdateInformation();
	surface->ComputeNormalsOn();
	surface->ComputeScalarsOn();
	surface->SetValue(0, isoValue);
	vtkSmartPointer<vtkPolyDataMapper> meshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	meshMapper->SetInputConnection(surface->GetOutputPort());
	meshMapper->ScalarVisibilityOff();
	meshActor->SetMapper(meshMapper);
}

void Figura::setProperties() {
	volumeProperty->SetInterpolationTypeToLinear(); // interpolación linear
	volumeProperty->SetGradientOpacity(tf->getGradientFun()); // función de opacidad gradiente
	volumeProperty->SetScalarOpacity(tf->getScalarFun()); // función de opacidad escalar
	volumeProperty->SetColor(tf->getColorFun()); // función de color
}

void Figura::enableShadow(const bool onOff) {
	if (onOff) {
		volumeProperty->ShadeOn();
	} else {
		volumeProperty->ShadeOff();
	}
}

void Figura::setMaterial(const double ambient, const double diffuse, const double specular, const double power) {
	volumeProperty->SetAmbient(ambient); // componente ambiental del material
	volumeProperty->SetDiffuse(diffuse); // componente difusa del material
	volumeProperty->SetSpecular(specular); // componente especular del material
	volumeProperty->SetSpecularPower(power); // componente de potencia especular del material
}

void Figura::setRenderMode(const int mode) {
	switch (mode) {
	case 0:
		mapper->SetRequestedRenderModeToGPU();
		break;
	case 1:
		mapper->SetRequestedRenderModeToRayCast();
		break;
	default:
		mapper->SetRequestedRenderModeToDefault();
	}
}

void Figura::setIsoValue(const double isoValue) {
	this->isoValue = isoValue;
}