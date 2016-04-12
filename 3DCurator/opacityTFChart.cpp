#include "opacityTFChart.h"

OpacityTFChart::OpacityTFChart(vtkSmartPointer<vtkRenderWindow> figureRenWin, vtkSmartPointer<vtkRenderWindow> chartRenWin, 
	vtkSmartPointer<vtkPiecewiseFunction> tf, const std::string xLabel, const std::string yLabel, const double minBound, const double maxBound) {
	this->tf = tf;
	
	chart = vtkSmartPointer<ChartXY>::New();
	chart->GetAxis(0)->SetTitle(yLabel);
	chart->GetAxis(1)->SetTitle(xLabel);
	// no permite que los plot cambien el rango de los ejes
	chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
	chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);

	function = vtkSmartPointer<vtkPiecewiseFunctionItem>::New();
	function->SetPiecewiseFunction(tf);
	chart->AddPlot(function);

	controlPoints = vtkSmartPointer<PiecewiseControlPointsItem>::New();
	controlPoints->SetRenderWindow(figureRenWin); // renderWindow que actualizar� al cambiar la funci�n de transferencia
	controlPoints->SetPiecewiseFunction(tf);
	controlPoints->SetUserBounds(minBound, maxBound, 0, 1); // l�mite hasta donde se pueden colocar puntos (xmin, xmax, ymin, ymax)
	chart->AddPlot(controlPoints);

	context = vtkSmartPointer<vtkContextView>::New();
	context->SetRenderWindow(chartRenWin); // renderWindow donde se ver�n los gr�ficos
	context->GetScene()->AddItem(chart);

	defaultRange(); // actualiza rangos
}

OpacityTFChart::~OpacityTFChart() {

}

void OpacityTFChart::defaultRange() {
	chart->GetAxis(0)->SetRange(0, 1); // rango del eje Y
	chart->GetAxis(1)->SetRange(tf->GetRange()[0], tf->GetRange()[1]); // rango del eje X extra�do de la funci�n de transferencia
}

void OpacityTFChart::setRange(const double min, const double max) {
	chart->GetAxis(1)->SetRange(min, max);
	context->Render();
}
