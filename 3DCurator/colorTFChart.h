#ifndef COLORTFCHART_H
#define COLORTFCHART_H

#include "vtkContextView.h"
#include "vtkTable.h"
#include "vtkPlot.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkChartXY.h"
#include "vtkColorTransferFunction.h"
#include "vtkColorTransferFunctionItem.h"
#include "vtkColorTransferControlPointsItem.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"

class ColorTFChart {

public:
	// constructor/destructor
	ColorTFChart(vtkSmartPointer<vtkColorTransferFunction> tf, vtkSmartPointer<vtkRenderWindow> renWin, const std::string xLabel, const std::string yLabel, const double minBound, const double maxBound);
	~ColorTFChart();

	// funcs
	void defaultRange();

private:
	vtkSmartPointer<vtkChartXY> chart;
	vtkSmartPointer<vtkContextView> context;
	vtkSmartPointer<vtkColorTransferFunctionItem> function;
	vtkSmartPointer<vtkColorTransferControlPointsItem> controlPoints;
	vtkSmartPointer<vtkColorTransferFunction> tf;
};

#endif