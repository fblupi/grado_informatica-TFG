#include "figura.h"

Figura::Figura() {
	tf = new TransferFunction();
	isoValue = WOOD_ISOVALUE;
	loaded = false;
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	imageData = vtkSmartPointer<vtkImageData>::New();
	meshActor = vtkSmartPointer<vtkActor>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
	meshMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	surface = vtkSmartPointer<vtkMarchingCubes>::New();
	setProperties(); // asigna propiedades del volumen (material, función de transferencia...) y la malla
	connectComponents(); // conecta los components del volumen y la malla
}

Figura::~Figura() {

}

void Figura::connectComponents() {
	// Volumen
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);

	// Malla
	meshMapper->SetInputConnection(surface->GetOutputPort());
	meshActor->SetMapper(meshMapper);
}

void Figura::setProperties() {
	// Volumen
	volumeProperty->SetInterpolationTypeToLinear(); // interpolación linear
	volumeProperty->SetGradientOpacity(tf->getGradientFun()); // función de opacidad gradiente
	volumeProperty->SetScalarOpacity(tf->getScalarFun()); // función de opacidad escalar
	volumeProperty->SetColor(tf->getColorFun()); // función de color
	volumeProperty->ShadeOn(); // habilita el sombreado

	// Malla
	surface->ComputeNormalsOn();
	surface->ComputeScalarsOn();
	meshMapper->ScalarVisibilityOff();
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

vtkSmartPointer<vtkActor> Figura::getMesh() const {
	return meshActor;
}

double Figura::getIsoValue() const {
	return isoValue;
}

bool Figura::getLoaded() const {
	return loaded;
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
	loaded = false;
	vtkSmartPointer<vtkDICOMImageReader> imageReader = vtkSmartPointer<vtkDICOMImageReader>::New();

	// Lee los datos
    imageReader->SetDirectoryName(s.c_str()); // asigna la carpeta al image reader
    imageReader->Update(); // lee los archivos

	// Guarda los datos del volumen
	imageData->DeepCopy(imageReader->GetOutput());

	// Actualiza los input data
	volumeMapper->SetInputData(imageData); // conecta el mapper con los datos

	surface->SetInputData(imageData); // conecta la malla con los datos
	meshMapper->Update(); // actualiza el mapper de la malla
	surface->SetValue(0, isoValue); // asigna el valor de isosuperficie

	loaded = true;
}

void Figura::createMesh() {
	surface->SetValue(0, isoValue);
}

void Figura::setMaterial(const double ambient, const double diffuse, const double specular, const double power) {
	volumeProperty->SetAmbient(ambient); // componente ambiental del material
	volumeProperty->SetDiffuse(diffuse); // componente difusa del material
	volumeProperty->SetSpecular(specular); // componente especular del material
	volumeProperty->SetSpecularPower(power); // componente de potencia especular del material
}

void Figura::setIsoValue(const double isoValue) {
	this->isoValue = isoValue;
}