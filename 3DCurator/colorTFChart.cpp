#include "colorTFChart.h"

ColorTFChart::ColorTFChart(vtkSmartPointer<vtkColorTransferFunction> tf, vtkSmartPointer<vtkRenderWindow> renWin, const std::string xLabel, const std::string yLabel) {
	chart = vtkSmartPointer<vtkChartXY>::New();
	chart->GetAxis(0)->SetTitle(yLabel);
	chart->GetAxis(0)->SetRange(0, 1);
	chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
	chart->GetAxis(1)->SetTitle(xLabel);
	chart->GetAxis(1)->SetRange(tf->GetRange()[0], tf->GetRange()[1]);
	chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);

	function = vtkSmartPointer<vtkColorTransferFunctionItem>::New();
	function->SetColorTransferFunction(tf);
	chart->AddPlot(function);

	controlPoints = vtkSmartPointer<vtkColorTransferControlPointsItem>::New();
	controlPoints->SetColorTransferFunction(tf);
	controlPoints->SetUserBounds(0, 10976, 0, 1);
	chart->AddPlot(controlPoints);

	context = vtkSmartPointer<vtkContextView>::New();
	context->SetRenderWindow(renWin);
	context->GetScene()->AddItem(chart);
}


ColorTFChart::~ColorTFChart() {

}
