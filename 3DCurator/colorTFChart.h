#ifndef COLORTFCHART_H
#define COLORTFCHART_H

#include "vtkContextView.h"
#include "vtkTable.h"
#include "vtkPlot.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkColorTransferFunction.h"
#include "vtkColorTransferFunctionItem.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"

#include "colorTransferControlPointsItem.h"
#include "chartXY.h"

class ColorTFChart {

public:
	// constructor/destructor
	ColorTFChart(vtkSmartPointer<vtkRenderWindow> figureRenWin, vtkSmartPointer<vtkRenderWindow> chartRenWin, vtkSmartPointer<vtkColorTransferFunction> tf, const std::string xLabel, const std::string yLabel, const double minBound, const double maxBound);
	~ColorTFChart();

	// funcs
	void defaultRange();

private:
	vtkSmartPointer<ChartXY> chart;
	vtkSmartPointer<vtkContextView> context;
	vtkSmartPointer<vtkColorTransferFunctionItem> function;
	vtkSmartPointer<ColorTransferControlPointsItem> controlPoints;
	vtkSmartPointer<vtkColorTransferFunction> tf;
};

#endif