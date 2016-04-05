#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <vector>
#include <string>
#include <iostream>

#include "boost/property_tree/xml_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/foreach.hpp"

#include "vtkSmartPointer.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"

struct ColorPoint {
	double x;
	double r;
	double g;
	double b;
	double m;
	double s;
};

struct OpacityPoint {
	double x;
	double y;
	double m;
	double s;
};

typedef std::vector<ColorPoint> ColorTF;
typedef std::vector<OpacityPoint> ScalarTF;
typedef std::vector<OpacityPoint> GradientTF;

struct TF {
	ColorTF colorTF;
	ScalarTF scalarTF;
	GradientTF gradientTF;
	std::string name;
	std::string description;
};

class TransferFunction {

public:
	// constructor/destructor
	TransferFunction();

	// getter
	vtkSmartPointer<vtkColorTransferFunction> getColorFun() const;
	vtkSmartPointer<vtkPiecewiseFunction> getScalarFun() const;
	vtkSmartPointer<vtkPiecewiseFunction> getGradientFun() const;

	// funcs
	void read(std::string &filename);
	void write(std::string &filename);
	void update();
	void clear();
	void addColorPoint(const double x, const double r, const double g, const double b);
	void addColorPoint(const double x, const double r, const double g, const double b, const double m, const double s);
	void addScalarPoint(const double x, const double y);
	void addScalarPoint(const double x, const double y, const double m, const double s);
	void addGradientPoint(const double x, const double y);
	void addGradientPoint(const double x, const double y, const double m, const double s);

private:
	// atributos
	struct TF tf;
	vtkSmartPointer<vtkColorTransferFunction> colorFun;
	vtkSmartPointer<vtkPiecewiseFunction> scalarFun;
	vtkSmartPointer<vtkPiecewiseFunction> gradientFun;
};

#endif // TRANSFERFUNCTION_H
