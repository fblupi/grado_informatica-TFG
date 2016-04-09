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

class TransferFunction {

public:
	// constructor/destructor
	TransferFunction();
	~TransferFunction();

	// getter
	vtkSmartPointer<vtkColorTransferFunction> getColorFun() const;
	vtkSmartPointer<vtkPiecewiseFunction> getScalarFun() const;
	vtkSmartPointer<vtkPiecewiseFunction> getGradientFun() const;
	std::string getName() const;
	std::string getDescription() const;

	// setter
	void setName(const std::string name);
	void setDescription(const std::string description);

	// funcs
	void read(std::string &filename);
	void write(std::string &filename);
	void clear();
	void addColorPoint(const double x, const double r, const double g, const double b);
	void addScalarPoint(const double x, const double y);
	void addGradientPoint(const double x, const double y);

private:
	// atributos
	std::string name;
	std::string description;
	vtkSmartPointer<vtkColorTransferFunction> colorFun;
	vtkSmartPointer<vtkPiecewiseFunction> scalarFun;
	vtkSmartPointer<vtkPiecewiseFunction> gradientFun;
};

#endif // TRANSFERFUNCTION_H
