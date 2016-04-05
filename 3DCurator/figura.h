#ifndef FIGURA_H
#define FIGURA_H

#include "vtkAlgorithm.h"
#include "vtkColorTransferFunction.h"
#include "vtkDICOMImageReader.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkImageData.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSmartPointer.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkExtractHistogram2D.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"

#include "transferFunction.h"

class Figura {

public:
	// constructor/destructor
    Figura();
    ~Figura();

	// getters
	TransferFunction *getTransferFunction() const;
    vtkSmartPointer<vtkVolume> getVolume() const;
	vtkSmartPointer<vtkAlgorithm> getReader() const;
	double getMinXBound() const;
	double getMinYBound() const;
	double getMinZBound() const;
	double getMaxXBound() const;
	double getMaxYBound() const;
	double getMaxZBound() const;

	// funcs
	void setDICOMFolder(const std::string s);
	void removeTFPoints();
	void addScalarPoint(const double value, const double alpha);
	void addGradientPoint(const double value, const double alpha);
	void addRGBPoint(const double value, const double c1, const double c2, const double c3);
	void enableShadow(const bool onOff);
	void setMaterial(const double ambient, const double diffuse, const double specular, const double power);
	void setRenderMode(const int mode);

private:
	// atributos
    vtkSmartPointer<vtkDICOMImageReader> imageReader;
    vtkSmartPointer<vtkAlgorithm> reader;
    vtkSmartPointer<vtkVolume> volume;
	vtkSmartPointer<vtkSmartVolumeMapper> mapper;
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;
	TransferFunction *tf;

	// funcs
    void setProperties();
};

#endif // FIGURA_H
