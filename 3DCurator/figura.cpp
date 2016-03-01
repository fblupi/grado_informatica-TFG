#include "figura.h"

Figura::Figura() {
	imageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader = imageReader;
    colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
	opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
	gradientFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    setProperties();
    volume->SetProperty(volumeProperty);
	volume->SetMapper(mapper);
}

Figura::~Figura() {

}

vtkSmartPointer<vtkVolume> Figura::getVolume() const {
	return volume;
}

vtkSmartPointer<vtkAlgorithm> Figura::getReader() const {
	return reader;
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
	mapper->SetInputConnection(reader->GetOutputPort()); // conecta el mapper con el reader
}

void Figura::setProperties() {
    volumeProperty->SetIndependentComponents(true);
    volumeProperty->SetColor(colorFun); // función de color
    volumeProperty->SetScalarOpacity(opacityFun); // función de opacidad escalar
	volumeProperty->SetGradientOpacity(gradientFun); // función de opacidad gradiente
	volumeProperty->SetInterpolationTypeToLinear();
	/*
	volumeProperty->ShadeOn();
	volumeProperty->SetAmbient(0.1); // componente ambiental del material
	volumeProperty->SetDiffuse(0.9); // componente difusa del material
	volumeProperty->SetSpecular(0.2); // componente especular del material
	volumeProperty->SetSpecularPower(10.0); // componente de potencia especular del material
	*/
}

void Figura::removeTFPoints() {
	colorFun->RemoveAllPoints();
	opacityFun->RemoveAllPoints();
	gradientFun->RemoveAllPoints();
}

void Figura::addOpacityPoint(const double value, const double alpha) {
	opacityFun->AddPoint(value, alpha);
}

void Figura::addGradientPoint(const double value, const double alpha) {
	gradientFun->AddPoint(value, alpha);
}

void Figura::addRGBPoint(const double value, const double c1, const double c2, const double c3) {
	colorFun->AddRGBPoint(value, c1, c2, c3);
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