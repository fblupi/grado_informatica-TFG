#ifndef OPACITYTFCHART_H
#define OPACITYTFCHART_H

#include "vtkContextView.h"
#include "vtkTable.h"
#include "vtkPlot.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPiecewiseFunctionItem.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"

#include "piecewiseControlPointsItem.h"
#include "chartXY.h"

class OpacityTFChart {

public:
	// constructor/destructor
	OpacityTFChart(vtkSmartPointer<vtkRenderWindow> figureRenWin, vtkSmartPointer<vtkRenderWindow> chartRenWin, vtkSmartPointer<vtkPiecewiseFunction> tf, const std::string xLabel, const std::string yLabel, const double minBound, const double maxBound);
	~OpacityTFChart();

	// funcs
	void defaultRange();
	void setRange(const double min, const double max);

private:
	vtkSmartPointer<ChartXY> chart;
	vtkSmartPointer<vtkContextView> context;
	vtkSmartPointer<vtkPiecewiseFunctionItem> function;
	vtkSmartPointer<PiecewiseControlPointsItem> controlPoints; 
	vtkSmartPointer<vtkPiecewiseFunction> tf;
};

#endif