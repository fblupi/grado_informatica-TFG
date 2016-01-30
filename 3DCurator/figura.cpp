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
	histogram = vtkSmartPointer<vtkExtractHistogram2D>::New();
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

vtkSmartPointer<vtkExtractHistogram2D> Figura::getHistogram() const {
	return histogram;
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
	
	//histogram->SetInputConnection(reader->GetOutputPort());
	//histogram->Update();
/*
	vtkSmartPointer<vtkImageData> output = vtkSmartPointer<vtkImageData>::New();
	output->ShallowCopy(imageReader->GetOutput());

	vtkSmartPointer<vtkDataArray> scalars = output->GetPointData()->GetScalars();

	int ijk[3] = {31, -244, 429};
	int pointId = output->ComputePointId(ijk);

	cout << scalars->GetTuple1(pointId) << endl;
*/
}

void Figura::setProperties() {
    volumeProperty->SetIndependentComponents(true);
    volumeProperty->SetColor(colorFun); // función de color
    volumeProperty->SetScalarOpacity(opacityFun); // función de opacidad
    volumeProperty->SetInterpolationTypeToLinear();
    //volumeProperty->ShadeOn(); // sombras activas
    volumeProperty->SetAmbient(0.1); // componente ambiental del material
    volumeProperty->SetDiffuse(0.9); // componente difusa del material
    volumeProperty->SetSpecular(0.1); // componente especular del material
}

void Figura::removeTFPoints() {
	colorFun->RemoveAllPoints();
	opacityFun->RemoveAllPoints();
}

void Figura::addOpacityPoint(const double value, const double alpha) {
	opacityFun->AddPoint(value, alpha);
}

void Figura::addRGBPoint(const double value, const double c1, const double c2, const double c3) {
	colorFun->AddRGBPoint(value, c1, c2, c3);
}

void Figura::setTransferFunction() {
	// valores de intensidad para los materiales
	double
		woodValueMin = -750,
		woodValueMax = -350,
		stuccoValueMin = -200,
		stuccoValueMax = 800;

	// colores y transparencia para cada material
	Color 
		woodColorMin = Color(0.078, 0.047, 0.031, 1.0), 
		woodColorMax = Color(0.392, 0.251, 0.157, 1.0), 
		stuccoColorMin = Color(0.8, 0.8, 0.8, 0.9), 
		stuccoColorMax = Color(0.8, 0.8, 0.8, 0.9), 
		transparentColor = Color(0.0);

	colorFun->AddRGBPoint(woodValueMin - .1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(woodValueMin - .1, transparentColor.a());

	colorFun->AddRGBPoint(woodValueMin, woodColorMin.r(), woodColorMin.g(), woodColorMin.b());
	opacityFun->AddPoint(woodValueMin, woodColorMin.a());

	colorFun->AddRGBPoint(woodValueMax, woodColorMax.r(), woodColorMax.g(), woodColorMax.b());
	opacityFun->AddPoint(woodValueMax, woodColorMax.a());

	colorFun->AddRGBPoint(woodValueMax + 1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(woodValueMax + 1, transparentColor.a());

	colorFun->AddRGBPoint(stuccoValueMin - 1, transparentColor.r(), transparentColor.g(), transparentColor.b());
	opacityFun->AddPoint(stuccoValueMin - 1, transparentColor.a());

	colorFun->AddRGBPoint(stuccoValueMin, stuccoColorMin.r(), stuccoColorMin.g(), stuccoColorMin.b());
	opacityFun->AddPoint(stuccoValueMin, stuccoColorMin.a());

	colorFun->AddRGBPoint(stuccoValueMax, stuccoColorMax.r(), stuccoColorMax.g(), stuccoColorMax.b());
	opacityFun->AddPoint(stuccoValueMax, stuccoColorMax.a());

}