#ifndef OPACITYTFCHART_H
#define OPACITYTFCHART_H

#include "vtkContextView.h"
#include "vtkTable.h"
#include "vtkPlot.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkChartXY.h"
#include "vtkPiecewiseFunction.h"
#include "vtkPiecewiseFunctionItem.h"
#include "vtkPiecewiseControlPointsItem.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"

class OpacityTFChart {

public:
	// constructor/destructor
	OpacityTFChart(vtkSmartPointer<vtkPiecewiseFunction> tf, vtkSmartPointer<vtkRenderWindow> renWin, const std::string xLabel, const std::string yLabel);
	~OpacityTFChart();

private:
	vtkSmartPointer<vtkChartXY> chart;
	vtkSmartPointer<vtkContextView> context;
	vtkSmartPointer<vtkPiecewiseFunctionItem> function;
	vtkSmartPointer<vtkPiecewiseControlPointsItem> controlPoints;
};

#endif