#include "figura.h"

Figura::Figura() {
	tf = new TransferFunction();
	imageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader = imageReader;
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	imageData = vtkSmartPointer<vtkImageData>::New();
	//histogram = vtkSmartPointer<vtkImageAccumulate>::New();
	//histogram->SetComponentExtent(-750, 3000, 0, 3000, 0, 0);
    volume = vtkSmartPointer<vtkVolume>::New();
	setProperties(); // asigna propiedades del volumen (material, funci�n de transferencia...)
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

vtkSmartPointer<vtkAlgorithm> Figura::getReader() const {
	return reader;
}

vtkSmartPointer<vtkImageData> Figura::getImageData() const {
	return imageData;
}

vtkSmartPointer<vtkImageAccumulate> Figura::getHistogram() const {
	return histogram;
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
    imageReader->SetDirectoryName(s.c_str()); // asigna la carpeta al image reader
    imageReader->Update(); // lee los archivos
	imageData->ShallowCopy(imageReader->GetOutput());
	mapper->SetInputData(imageData); // conecta el mapper con el reader
	//histogram->SetInputConnection(reader->GetOutputPort());
}

void Figura::setProperties() {
	mapper->SetBlendModeToComposite(); // renderiza con composici�n
	volumeProperty->SetInterpolationTypeToLinear(); // interpolaci�n linear
	volumeProperty->SetGradientOpacity(tf->getGradientFun()); // funci�n de opacidad gradiente
	volumeProperty->SetScalarOpacity(tf->getScalarFun()); // funci�n de opacidad escalar
	volumeProperty->SetColor(tf->getColorFun()); // funci�n de color
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