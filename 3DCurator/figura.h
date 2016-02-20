#ifndef FIGURA_H
#define FIGURA_H

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
#include "vtkExtractHistogram2D.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

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
	vtkSmartPointer<vtkExtractHistogram2D> getHistogram() const;
	double getMinXBound() const;
	double getMinYBound() const;
	double getMinZBound() const;
	double getMaxXBound() const;
	double getMaxYBound() const;
	double getMaxZBound() const;

	// setters
    void setImageReader(const vtkSmartPointer<vtkDICOMImageReader> imageReader);
    void setReader(const vtkSmartPointer<vtkAlgorithm> reader);
    void setVolume(const vtkSmartPointer<vtkVolume> volume);
    void setMapper(const vtkSmartPointer<vtkSmartVolumeMapper> mapper);
    void setColorFun(const vtkSmartPointer<vtkColorTransferFunction> colorFun);
    void setOpacityFun(const vtkSmartPointer<vtkPiecewiseFunction> opacityFun);
    void setVolumeProperty(const vtkSmartPointer<vtkVolumeProperty> volumeProperty);

	// funcs
	void setDICOMFolder(const std::string s);
	void removeTFPoints();
	void addOpacityPoint(const double value, const double alpha);
	void addGradientPoint(const double value, const double alpha);
	void addRGBPoint(const double value, const double c1, const double c2, const double c3);
	void enableShadow();
	void disableShadow();

private:
	// atributos
    vtkSmartPointer<vtkDICOMImageReader> imageReader;
    vtkSmartPointer<vtkAlgorithm> reader;
    vtkSmartPointer<vtkVolume> volume;
	vtkSmartPointer<vtkSmartVolumeMapper> mapper;
    vtkSmartPointer<vtkColorTransferFunction> colorFun;
	vtkSmartPointer<vtkPiecewiseFunction> opacityFun;
	vtkSmartPointer<vtkPiecewiseFunction> gradientFun;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
	vtkSmartPointer<vtkExtractHistogram2D> histogram;

	// funcs
    void setProperties();
};

#endif // FIGURA_H
