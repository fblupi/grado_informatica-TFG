#ifndef FIGURA_H
#define FIGURA_H

#include "color.h"

#include "vtkAlgorithm.h"
#include "vtkColorTransferFunction.h"
#include "vtkDICOMImageReader.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkImageData.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSmartPointer.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

class Figura {

public:
	// constructor/destructor
    Figura();
    ~Figura();

	// getters
    vtkSmartPointer<vtkDICOMImageReader> getImageReader() const;
    vtkSmartPointer<vtkAlgorithm> getReader() const;
    vtkSmartPointer<vtkVolume> getVolume() const;
    vtkSmartPointer<vtkSmartVolumeMapper> getMapper() const;
    vtkSmartPointer<vtkColorTransferFunction> getColorFun() const;
    vtkSmartPointer<vtkPiecewiseFunction> getOpacityFun() const;
    vtkSmartPointer<vtkVolumeProperty> getVolumeProperty() const;

	// setters
    void setImageReader(const vtkSmartPointer<vtkDICOMImageReader> imageReader);
    void setReader(const vtkSmartPointer<vtkAlgorithm> reader);
    void setVolume(const vtkSmartPointer<vtkVolume> volume);
    void setMapper(const vtkSmartPointer<vtkSmartVolumeMapper> mapper);
    void setColorFun(const vtkSmartPointer<vtkColorTransferFunction> colorFun);
    void setOpacityFun(const vtkSmartPointer<vtkPiecewiseFunction> opacityFun);
    void setVolumeProperty(const vtkSmartPointer<vtkVolumeProperty> volumeProperty);
	void removeTFPoints();
	void addPoint(const double value, const Color color);

	// funcs
	void setDICOMFolder(const std::string s);

private:
	// atributos
    vtkSmartPointer<vtkDICOMImageReader> imageReader;
    vtkSmartPointer<vtkAlgorithm> reader;
    vtkSmartPointer<vtkVolume> volume;
	vtkSmartPointer<vtkSmartVolumeMapper> mapper;
    vtkSmartPointer<vtkColorTransferFunction> colorFun;
    vtkSmartPointer<vtkPiecewiseFunction> opacityFun;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;

	// funcs
    void setProperties();
    void setTransferFunction();
};

#endif // FIGURA_H
