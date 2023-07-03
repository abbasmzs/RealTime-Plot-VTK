#include <MainWindow.h>
#include "ui_MainWindow.h"
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkPlot.h>
#include <vtkContextActor.h>
#include <vtkContextScene.h>
#include <vtkBrush.h>
#include <vtkPlotPoints.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkTextActor.h>
#include <vtkAxis.h>
#include <vtkChartMatrix.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkFloatArray.h>
#include <vtkNew.h>
#include <vtkPen.h>
#include <vtkTable.h>
#include <vtkVersion.h>
#include <array>

#if VTK_VERSION_NUMBER >= 90220220630ULL
#define VTK_HAS_SETCOLORF 1
#endif

#if VTK_VERSION_NUMBER >= 89000000000ULL
#define VTK890 1
#endif

#if VTK_VERSION_NUMBER >= 90000000000ULL
#define VTK900 1
#endif

class Signal {
public:
	Signal(std::vector<double> data) : data_(data) {}
	std::vector<double> data() const { return data_; }
private:
	std::vector<double> data_;
};

void receiveSignal(std::vector<double>& signal_data, std::mutex& signal_mutex, std::condition_variable& signal_cv) {
	while (true) {
		// Simulate receiving the signal data from a UDP or TCP socket
		// Here, we just fill the vector with random values
		for (int i = 0; i < signal_data.size(); i++) {
			signal_data[i] = rand() % 100;
		}

		// Create a signal with the vector as data
		Signal signal(signal_data);

		// Lock the mutex and update the signal data
		{
			std::unique_lock<std::mutex> lock(signal_mutex);
			signal_data = signal.data();
		}

		// Notify the condition variable that new signal data is available
		signal_cv.notify_one();

		// Pause for 1ms
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);

	QObject::connect(this->ui->DrawPlot_button, &QPushButton::clicked, this, &MainWindow::onDrawSphereClick);
	QObject::connect(this->ui->DrawPlot_button2, &QPushButton::clicked, this, &MainWindow::onDrawSphereClick2);

}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::onDrawSphereClick()
{
	vtkNew<vtkNamedColors> colors;
	if (mChartVisible) {
		// If the chart is visible, remove it from the view
		mChartVisible = false;
		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->ui->openGLWidget->setRenderWindow(renderWindow);
		vtkNew<vtkContextView> view;
		view->SetRenderWindow(renderWindow);
		view->GetRenderer()->SetBackground(colors->GetColor3d("Black").GetData());
		//vtkContextView* view = vtkContextView::SafeDownCast(ui->openGLWidget->GetRenderWindow());
		if (view) {
			view->GetScene()->ClearItems();
			view->Render();
		}
	}
	else {
		mChartVisible = true;

		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->ui->openGLWidget->setRenderWindow(renderWindow);

		// Create a table with some points in it.
		vtkNew<vtkTable> table;

		vtkNew<vtkFloatArray> arrX;
		arrX->SetName("X Axis");
		table->AddColumn(arrX);

		vtkNew<vtkFloatArray> arrC;
		arrC->SetName("Cosine");
		table->AddColumn(arrC);

		vtkNew<vtkFloatArray> arrS;
		arrS->SetName("Sine");
		table->AddColumn(arrS);

		// Fill in the table with some example values.
		int numPoints = 69;
		float inc = 7.5 / (numPoints - 1);
		table->SetNumberOfRows(numPoints);
		for (int i = 0; i < numPoints; ++i)
		{
			table->SetValue(i, 0, i * inc);
			table->SetValue(i, 1, cos(i * inc));
			table->SetValue(i, 2, sin(i * inc));
		}

		// Set up the view
		vtkNew<vtkContextView> view;
		view->SetRenderWindow(renderWindow);
		view->GetRenderWindow()->SetWindowName("renderWindow");
		view->GetRenderer()->SetBackground(colors->GetColor3d("SlateGray").GetData());

		// Add multiple line plots, setting the colors etc.
		vtkNew<vtkChartXY> chart;
		view->GetScene()->AddItem(chart);
		vtkPlot* line = chart->AddPlot(vtkChart::LINE);
		line->SetInputData(table, 0, 1);
		line->SetColor(0, 255, 0, 255);
		line->SetWidth(1.0);
		line = chart->AddPlot(vtkChart::LINE);
		line->SetInputData(table, 0, 2);
		line->SetColor(255, 0, 0, 255);
		line->SetWidth(5.0);

		view->GetRenderWindow()->SetMultiSamples(0);
		view->Render();
	}
}

void MainWindow::onDrawSphereClick2()
{
	vtkNew<vtkNamedColors> colors;
	if (mChartVisible) {
		// If the chart is visible, remove it from the view
		mChartVisible = false;
		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->ui->openGLWidget->setRenderWindow(renderWindow);
		vtkNew<vtkContextView> view;
		view->SetRenderWindow(renderWindow);
		view->GetRenderer()->SetBackground(colors->GetColor3d("Black").GetData());
		//vtkContextView* view = vtkContextView::SafeDownCast(ui->openGLWidget->GetRenderWindow());
		if (view) {
			view->GetScene()->ClearItems();
			view->Render();
		}
	}
	else {
		mChartVisible = true;

		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->ui->openGLWidget->setRenderWindow(renderWindow);

		// Create a table with some points in it.
		vtkNew<vtkTable> table;

		vtkNew<vtkFloatArray> arrX;
		arrX->SetName("X Axis");
		table->AddColumn(arrX);

		vtkNew<vtkFloatArray> arrC;
		arrC->SetName("Cosine");
		table->AddColumn(arrC);

		vtkNew<vtkFloatArray> arrS;
		arrS->SetName("Sine");
		table->AddColumn(arrS);

		// Fill in the table with some example values.
		int numPoints = 69;
		float inc = 7.5 / (numPoints - 1);
		table->SetNumberOfRows(numPoints);
		for (int i = 0; i < numPoints; ++i)
		{
			table->SetValue(i, 0, i * inc);
			table->SetValue(i, 1, cos(i * inc));
			table->SetValue(i, 2, sin(i * inc));
		}

		// Set up the view
		vtkNew<vtkContextView> view;
		view->SetRenderWindow(renderWindow);
		view->GetRenderWindow()->SetWindowName("renderWindow");
		view->GetRenderer()->SetBackground(colors->GetColor3d("Black").GetData());

		// Add multiple line plots, setting the colors etc.
		vtkNew<vtkChartXY> chart;
		view->GetScene()->AddItem(chart);
		vtkPlot* line = chart->AddPlot(vtkChart::LINE);
		line->SetInputData(table, 0, 1);
		line->SetColor(0, 255, 0, 255);
		line->SetWidth(1.0);
		line = chart->AddPlot(vtkChart::LINE);
		line->SetInputData(table, 0, 2);
		line->SetColor(255, 0, 0, 255);
		line->SetWidth(5.0);

		view->GetRenderWindow()->SetMultiSamples(0);
		view->Render();
	}
}