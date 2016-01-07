#include "figura.h"

Figura::Figura() {
	imageReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader = imageReader;
    colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
    opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    //mapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    setProperties();
    setTransferFunction();
    volume->SetProperty(volumeProperty);
	volume->SetMapper(mapper);
}

Figura::~Figura() {

}

vtkSmartPointer<vtkDICOMImageReader> Figura::getImageReader() const {
	return imageReader;
}

vtkSmartPointer<vtkAlgorithm> Figura::getReader() const {
	return reader;
}

vtkSmartPointer<vtkVolume> Figura::getVolume() const {
	return volume;
}

vtkSmartPointer<vtkSmartVolumeMapper> Figura::getMapper() const {
	return mapper;
}

vtkSmartPointer<vtkColorTransferFunction> Figura::getColorFun() const {
	return colorFun;
}

vtkSmartPointer<vtkPiecewiseFunction> Figura::getOpacityFun() const {
	return opacityFun;
}

vtkSmartPointer<vtkVolumeProperty> Figura::getVolumeProperty() const {
	return volumeProperty;
}

void Figura::setImageReader(const vtkSmartPointer<vtkDICOMImageReader> imageReader) {
	this->imageReader = imageReader;
}

void Figura::setReader(const vtkSmartPointer<vtkAlgorithm> reader) {
	this->reader = reader;
}

void Figura::setVolume(const vtkSmartPointer<vtkVolume> volume) {
	this->volume = volume;
}

void Figura::setMapper(const vtkSmartPointer<vtkSmartVolumeMapper> mapper) {
	this->mapper = mapper;
}

void Figura::setColorFun(const vtkSmartPointer<vtkColorTransferFunction> colorFun) {
	this->colorFun = colorFun;
}

void Figura::setOpacityFun(const vtkSmartPointer<vtkPiecewiseFunction> opacityFun) {
	this->opacityFun = opacityFun;
}

void Figura::setVolumeProperty(const vtkSmartPointer<vtkVolumeProperty> volumeProperty) {
	this->volumeProperty = volumeProperty;
}

void Figura::setDICOMFolder(const std::string s) {
    imageReader->SetDirectoryName(s.c_str()); // asigna la carpeta al image reader
    imageReader->Update(); // lee los archivos
	mapper->SetInputConnection(reader->GetOutputPort()); // conecta el mapper con el reader
}

void Figura::setProperties() {
    volumeProperty->SetIndependentComponents(true);
    volumeProperty->SetColor(colorFun); // función de color
    volumeProperty->SetScalarOpacity(opacityFun); // función de opacidad
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn(); // sombras activas
    volumeProperty->SetAmbient(0.1); // componente ambiental del material
    volumeProperty->SetDiffuse(0.9); // componente difusa del material
    volumeProperty->SetSpecular(0.1); // componente especular del material
}

void Figura::setTransferFunction() {
	double
		// nada
		v0 = -1000,
		v1 = -700,
		// madera
		v2 = v1 + 1,
		v3 = -350,
		// nada
		v4 = v3 + 1,
		v5 = -200,
		// estuco
		v6 = v5 + 1,
		v7 = 500,
		// nada
		v8 = v7 + 1,
		v9 = 3000;

	Color woodColor1 = Color(0.61, 0.45, 0.23, 1.0), 
		  woodColor2 = Color(0.47, 0.29, 0.18, 1.0), 
		  stuccoColor1 = Color(.8, .8, .8, .9), 
		  stuccoColor2 = Color(.7, .7, .7, .9), 
		  transparent = Color(.0);

	colorFun->AddRGBPoint(v0, transparent.r(), transparent.g(), transparent.b());
	opacityFun->AddPoint(v0, transparent.a());
	colorFun->AddRGBPoint(v1, transparent.r(), transparent.g(), transparent.b());
	opacityFun->AddPoint(v1, transparent.a());

	colorFun->AddRGBPoint(v2, woodColor1.r(), woodColor1.g(), woodColor1.b());
	opacityFun->AddPoint(v2, woodColor1.a());
	colorFun->AddRGBPoint(v3, woodColor2.r(), woodColor2.g(), woodColor2.b());
	opacityFun->AddPoint(v3, woodColor2.a());

	colorFun->AddRGBPoint(v4, transparent.r(), transparent.g(), transparent.b());
	opacityFun->AddPoint(v4, transparent.a());
	colorFun->AddRGBPoint(v5, transparent.r(), transparent.g(), transparent.b());
	opacityFun->AddPoint(v5, transparent.a());;

	colorFun->AddRGBPoint(v6, stuccoColor1.r(), stuccoColor1.g(), stuccoColor1.b());
	opacityFun->AddPoint(v6, stuccoColor1.a());
	colorFun->AddRGBPoint(v7, stuccoColor2.r(), stuccoColor2.g(), stuccoColor2.b());
	opacityFun->AddPoint(v7, stuccoColor2.a());

	colorFun->AddRGBPoint(v8, transparent.r(), transparent.g(), transparent.b());
	opacityFun->AddPoint(v8, transparent.a());
	colorFun->AddRGBPoint(v9, transparent.r(), transparent.g(), transparent.b());
	opacityFun->AddPoint(v9, transparent.a());

}