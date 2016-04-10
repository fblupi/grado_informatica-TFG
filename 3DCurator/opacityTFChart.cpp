#include "opacityTFChart.h"

OpacityTFChart::OpacityTFChart(vtkSmartPointer<vtkPiecewiseFunction> tf, vtkSmartPointer<vtkRenderWindow> renWin, const std::string xLabel, const std::string yLabel) {
	chart = vtkSmartPointer<vtkChartXY>::New();
	chart->GetAxis(0)->SetTitle(yLabel);
	chart->GetAxis(0)->SetRange(0, 1);
	chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
	chart->GetAxis(1)->SetTitle(xLabel);
	chart->GetAxis(1)->SetRange(tf->GetRange()[0], tf->GetRange()[1]);
	chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);

	function = vtkSmartPointer<vtkPiecewiseFunctionItem>::New();
	function->SetPiecewiseFunction(tf);
	chart->AddPlot(function);

	controlPoints = vtkSmartPointer<vtkPiecewiseControlPointsItem>::New();
	controlPoints->SetPiecewiseFunction(tf);
	controlPoints->SetUserBounds(0, 10976, 0, 1);
	chart->AddPlot(controlPoints);

	context = vtkSmartPointer<vtkContextView>::New();
	context->SetRenderWindow(renWin);
	context->GetScene()->AddItem(chart);
}


OpacityTFChart::~OpacityTFChart() {

}
