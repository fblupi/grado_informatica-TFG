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
    volumeProperty->SetColor(colorFun); // funci�n de color
    volumeProperty->SetScalarOpacity(opacityFun); // funci�n de opacidad
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn(); // sombras activas
    volumeProperty->SetAmbient(0.1); // componente ambiental del material
    volumeProperty->SetDiffuse(0.9); // componente difusa del material
    volumeProperty->SetSpecular(0.1); // componente especular del material
}

void Figura::removeTFPoints() {
	colorFun->RemoveAllPoints();
	opacityFun->RemoveAllPoints();
}

void Figura::addPoint(double value, Color color) {
	colorFun->AddRGBPoint(value, color.r(), color.g(), color.b());
	opacityFun->AddPoint(value, color.a());
}

void Figura::setTransferFunction() {
	// valores de intensidad para los materiales
	double
		woodValueMin = -620,
		woodValueMax = -350,
		stuccoValueMin = -200,
		stuccoValueMax = 0,
		valueMin = -10000,
		valueMax = 10000;

	// colores y transparencia para cada material
	Color 
		woodColorMin = Color(0.07, 0.63, 0.5, 1.0), 
		woodColorMax = Color(0.07, 0.63, 0.2, 1.0), 
		stuccoColorMin = Color(0.42, 0.01, 0.59, 0.9), 
		stuccoColorMax = Color(0.42, 0.01, 0.59, 0.9), 
		transparentColor = Color(0.0);

	colorFun->AddHSVPoint(valueMin, transparentColor.h(), transparentColor.s(), transparentColor.v());
	opacityFun->AddPoint(valueMin, transparentColor.a());
	colorFun->AddHSVPoint(woodValueMin - .1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(woodValueMin - .1, transparentColor.a());

	colorFun->AddHSVPoint(woodValueMin, woodColorMin.r(), woodColorMin.g(), woodColorMin.b());
	opacityFun->AddPoint(woodValueMin, woodColorMin.a());
	colorFun->AddHSVPoint(woodValueMax, woodColorMax.r(), woodColorMax.g(), woodColorMax.b());
	opacityFun->AddPoint(woodValueMax, woodColorMax.a());

	colorFun->AddHSVPoint(woodValueMax + 1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(woodValueMax + 1, transparentColor.a());
	colorFun->AddHSVPoint(stuccoValueMin - 1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(stuccoValueMin - 1, transparentColor.a());

	colorFun->AddHSVPoint(stuccoValueMin, stuccoColorMin.r(), stuccoColorMin.g(), stuccoColorMin.b());
	opacityFun->AddPoint(stuccoValueMin, stuccoColorMin.a());
	colorFun->AddHSVPoint(stuccoValueMax, stuccoColorMax.r(), stuccoColorMax.g(), stuccoColorMax.b());
	opacityFun->AddPoint(stuccoValueMax, stuccoColorMax.a());

	colorFun->AddHSVPoint(stuccoValueMax + 1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(stuccoValueMax + 1, transparentColor.a());
	colorFun->AddHSVPoint(valueMax, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(valueMax, transparentColor.a());

}