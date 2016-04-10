#include "opacityTFChart.h"

OpacityTFChart::OpacityTFChart(vtkSmartPointer<vtkRenderWindow> figureRenWin, vtkSmartPointer<vtkRenderWindow> chartRenWin, vtkSmartPointer<vtkPiecewiseFunction> tf, const std::string xLabel, const std::string yLabel, const double minBound, const double maxBound) {
	this->tf = tf;
	
	chart = vtkSmartPointer<ChartXY>::New();
	chart->GetAxis(0)->SetTitle(yLabel);
	chart->GetAxis(1)->SetTitle(xLabel);
	chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
	chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);
	chart->SetZoomWithMouseWheel(false);
	defaultRange();

	function = vtkSmartPointer<vtkPiecewiseFunctionItem>::New();
	function->SetPiecewiseFunction(tf);
	chart->AddPlot(function);

	controlPoints = vtkSmartPointer<PiecewiseControlPointsItem>::New();
	controlPoints->SetRenderWindow(figureRenWin);
	controlPoints->SetPiecewiseFunction(tf);
	controlPoints->SetUserBounds(minBound, maxBound, 0, 1);
	chart->AddPlot(controlPoints);

	context = vtkSmartPointer<vtkContextView>::New();
	context->SetRenderWindow(chartRenWin);
	context->GetScene()->AddItem(chart);
}


OpacityTFChart::~OpacityTFChart() {

}

void OpacityTFChart::defaultRange() {
	chart->GetAxis(0)->SetRange(0, 1);
	chart->GetAxis(1)->SetRange(tf->GetRange()[0], tf->GetRange()[1]);
}
