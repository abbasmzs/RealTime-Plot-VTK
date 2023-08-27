#include <MainWindow.h>
#include "ui_MainWindow.h"
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkBrush.h>
#include <vtkNew.h>
#include <array>
#include <chrono>
#include <fstream>
#include <vtkNamedColors.h>
#include <vtkPlotPoints.h>
#include <vtkFloatArray.h>
#include <vector>
#include <cstdint>
#include <vtkSmartPointer.h>
#include <vtkPlot.h>
#include <vtkContextActor.h>
#include <vtkContextScene.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleRubberBandZoom.h>
#include <vtkTextActor.h>
#include <vtkAxis.h>
#include <vtkChartMatrix.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkPen.h>
#include <vtkTable.h>
#include <vtkVersion.h>
#include <QTimer>
#include <thread>
#include <vtkTextProperty.h>
#include <vtkNamedColors.h>
#include <QThread>

struct PRFData {
	struct Header {
		uint16_t prf_angle;
		uint16_t prf_number;
		uint16_t video_length;
		uint16_t short_range_offset;
	} header;
	std::vector<double> i;
	std::vector<double> q;
};

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);

	//os.environ['QT_AUTO_SCREEN_SCALE_FACTOR'] = '1'

	QObject::connect(this->ui->DrawPlot_button, &QPushButton::clicked, this, &MainWindow::onDrawSphereClick);
	QObject::connect(this->ui->DrawPlot_button2, &QPushButton::clicked, this, &MainWindow::onDrawSphereClick2);

	// Create the PRF angle model and set it as the model for listView_3
	//prfAngleModel = new PRFAngleModel(this);
	//this->ui->listView_3->setModel(prfAngleModel);

}

MainWindow::~MainWindow() {
	delete ui;
}

//void MainWindow::onListViewClicked1(const QModelIndex& index)
//{
//	// Handle the click event of the QListView here
//	// You can use the index parameter to access the clicked item or perform any desired actions
//}
//
//void MainWindow::onListViewClicked2(const QModelIndex& index)
//{
//	// Handle the click event of the QListView here
//	// You can use the index parameter to access the clicked item or perform any desired actions
//}

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

		const int MAX_PRF_ANGLE = 8192; //4096
		const int size = 2600;
		std::ifstream file("E:/FAVA/Reading DAT file/IQ_06-07-2023__10(LOG1).dat", std::ios::in | std::ios::binary);

		std::vector<PRFData> prfData(MAX_PRF_ANGLE);

		std::vector<double> I;
		std::vector<double> Q;

		for (int k = 0; k < MAX_PRF_ANGLE; ++k) {
			auto& prf = prfData[k];

			// read headers
			uint16_t head;
			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
			prf.header.prf_angle = head;
			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
			prf.header.prf_number = head;
			prf.header.video_length = size;
			prf.header.short_range_offset = 100;

			// read video i
			std::vector<int16_t> data(size);
			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
			prf.i.resize(size);
			for (int i = 0; i < size; i++) {
				prf.i[i] = static_cast<double>(data[i]);
				I.push_back(prf.i[i]);
			}

			// read video q
			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
			prf.q.resize(size);
			for (int i = 0; i < size; i++) {
				prf.q[i] = static_cast<double>(data[i]);
				Q.push_back(prf.q[i]);
			}

			// skip other data
			file.seekg(sizeof(int16_t) * size, std::ios::cur);

		}

		file.close();

		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->ui->openGLWidget->setRenderWindow(renderWindow);

		vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
		vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
		view->GetRenderWindow()->SetWindowName("PRF Plot");
		view->GetRenderer()->SetBackground(colors->GetColor3d("black").GetData());

		vtkNew<vtkChartMatrix> matrix;
		view->GetScene()->AddItem(matrix);
		matrix->SetSize(vtkVector2i(2, 2));
		matrix->SetGutter(vtkVector2f(50.0, 50.0));
		view->SetRenderWindow(renderWindow);

		vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

		vtkSmartPointer<vtkFloatArray> arrX1 = vtkSmartPointer<vtkFloatArray>::New();
		arrX1->SetName("X Axis");
		table->AddColumn(arrX1);

		vtkSmartPointer<vtkFloatArray> arrC1 = vtkSmartPointer<vtkFloatArray>::New();
		arrC1->SetName("PRF");
		table->AddColumn(arrC1);

		//charts
		vtkSmartPointer<vtkChartXY> chart1 = vtkSmartPointer<vtkChartXY>::New();
		chart1->SetTitle("Signal 1");
		matrix->SetChart(vtkVector2i(0, 0), chart1);

		vtkSmartPointer<vtkChartXY> chart2 = vtkSmartPointer<vtkChartXY>::New();
		chart2->SetTitle("Signal 3");
		matrix->SetChart(vtkVector2i(0, 1), chart2);

		vtkSmartPointer<vtkChartXY> chart3 = vtkSmartPointer<vtkChartXY>::New();
		chart3->SetTitle("Signal 2");
		matrix->SetChart(vtkVector2i(1, 0), chart3);

		vtkSmartPointer<vtkChartXY> chart4 = vtkSmartPointer<vtkChartXY>::New();
		chart4->SetTitle("Signal 4");
		matrix->SetChart(vtkVector2i(1, 1), chart4);

		// Remove the grids
		chart1->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
		chart1->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
		chart2->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
		chart2->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
		chart3->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
		chart3->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
		chart4->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
		chart4->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);

		// color of titles
		//1
		vtkAxis* xAxis = chart1->GetAxis(vtkAxis::BOTTOM);
		xAxis->SetTitle("PRF1");
		xAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		xAxis->GetTitleProperties()->SetFontSize(10);
		xAxis->GetTitleProperties()->ItalicOn();
		xAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);

		vtkAxis* yAxis = chart1->GetAxis(vtkAxis::LEFT);
		yAxis->SetTitle("Val");
		yAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		yAxis->GetTitleProperties()->SetFontSize(10);
		yAxis->GetTitleProperties()->ItalicOn();
		yAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
		//2
		vtkAxis* xAxis2 = chart2->GetAxis(vtkAxis::BOTTOM);
		xAxis2->SetTitle("PRF2");
		xAxis2->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		xAxis2->GetTitleProperties()->SetFontSize(10);
		xAxis2->GetTitleProperties()->ItalicOn();
		xAxis2->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);

		vtkAxis* yAxis2 = chart2->GetAxis(vtkAxis::LEFT);
		yAxis2->SetTitle("Val");
		yAxis2->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		yAxis2->GetTitleProperties()->SetFontSize(10);
		yAxis2->GetTitleProperties()->ItalicOn();
		yAxis2->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
		//2
		vtkAxis* xAxis3 = chart3->GetAxis(vtkAxis::BOTTOM);
		xAxis3->SetTitle("PRF3");
		xAxis3->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		xAxis3->GetTitleProperties()->SetFontSize(10);
		xAxis3->GetTitleProperties()->ItalicOn();
		xAxis3->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);

		vtkAxis* yAxis3 = chart3->GetAxis(vtkAxis::LEFT);
		yAxis3->SetTitle("Val");
		yAxis3->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		yAxis3->GetTitleProperties()->SetFontSize(10);
		yAxis3->GetTitleProperties()->ItalicOn();
		yAxis3->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
		//2
		vtkAxis* xAxis4 = chart4->GetAxis(vtkAxis::BOTTOM);
		xAxis4->SetTitle("PRF4");
		xAxis4->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		xAxis4->GetTitleProperties()->SetFontSize(10);
		xAxis4->GetTitleProperties()->ItalicOn();
		xAxis4->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);

		vtkAxis* yAxis4 = chart4->GetAxis(vtkAxis::LEFT);
		yAxis4->SetTitle("Val");
		yAxis4->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		yAxis4->GetTitleProperties()->SetFontSize(10);
		yAxis4->GetTitleProperties()->ItalicOn();
		yAxis4->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);

		bool paused = false; // Flag to indicate if plotting is paused
		int start = 0;

		// Create a timer to trigger the chart update
		QTimer* timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, [=]() mutable {
			// Check if the plotting is paused
			if (!paused) {

				////High perfomrmancing
				chart1->RemovePlot(1);
				chart2->RemovePlot(1);
				chart3->RemovePlot(1);
				chart4->RemovePlot(1);
				chart1->RemovePlot(1);
				chart1->RemovePlot(2);
				chart1->RemovePlot(3);
				chart1->RemovePlot(4);
				view->RemoveAllObservers();
				view->RemoveAllRepresentations();
				table->RemoveAllObservers();

				// Extract data for the current step
				std::vector<double> data(size);
				std::vector<double> data2(size);

				for (int i = 0; i < size; ++i)
				{
					data[i] = I[start + i];
					data2[i] = Q[start + i];
				}

				int numPoints = data.size();
				table->SetNumberOfRows(numPoints);
				for (int i = 0; i < numPoints; ++i) {
					table->SetValue(i, 0, i);
					table->SetValue(i, 1, data[i]);
				}

				// Clear the chart before adding a new plot
				chart1->ClearPlots();

				//1
				vtkSmartPointer<vtkPlot> line1 = chart1->AddPlot(vtkChart::LINE);
				line1->SetInputData(table, 0, 1);
				line1->SetColor(221, 221, 0, 255);
				line1->SetWidth(.7);

				//2
				vtkSmartPointer<vtkPlot> line2 = chart2->AddPlot(vtkChart::LINE);
				line2->SetInputData(table, 0, 1);
				line2->SetColor(128, 0, 0, 255);
				line2->SetWidth(.7);

				//3
				vtkSmartPointer<vtkPlot> line3 = chart3->AddPlot(vtkChart::LINE);
				line3->SetInputData(table, 0, 1);
				line3->SetColor(0, 51, 102, 255);
				line3->SetWidth(.7);

				//4
				vtkSmartPointer<vtkPlot> line4 = chart4->AddPlot(vtkChart::LINE);
				line4->SetInputData(table, 0, 1);
				line4->SetColor(102, 204, 0, 255);
				line4->SetWidth(.7);

				// Update the plot with the new data
				chart1->Update();
				chart2->Update();
				chart3->Update();
				chart4->Update();

				view->GetRenderWindow()->Render();
				view->Render();

				// Update the start index for the next step
				start += size;

				// Check if all data has been plotted
				if (start + size > I.size()) {
					// Stop the timer if all data has been plotted
					timer->stop();
				}

				//// Update the PRF angles in the PRFAngleModel
				//prfAngleModel->setPRFAngles(prf.header.prf_angle);

				//// Trigger the update in listView_3
				//listView_3->update();

			}

		});

		// Start the timer to trigger the chart update
		timer->start(2); // Adjust the interval as needed

		// Pause/Resume button click event
		connect(this->ui->DrawPlot_button3, &QPushButton::clicked, this, [=]() mutable {
			paused = !paused;
			if (paused) {
				timer->stop(); // Pause the timer
			}
			else {
				timer->start(); // Resume the timer
			}
		});

		qApp->processEvents();
	}
}

void MainWindow::onDrawSphereClick2()
{

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

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
	else
	{
		mChartVisible = true;

		const int MAX_PRF_ANGLE = 8192; //4096
		const int size = 2600;
		std::ifstream file("E:/FAVA/Reading DAT file/IQ_06-07-2023__10(LOG1).dat", std::ios::in | std::ios::binary);

		std::vector<PRFData> prfData(MAX_PRF_ANGLE);

		std::vector<double> I;
		std::vector<double> Q;

		for (int k = 0; k < MAX_PRF_ANGLE; ++k) {
			auto& prf = prfData[k];

			// read headers
			uint16_t head;
			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
			prf.header.prf_angle = head;
			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
			prf.header.prf_number = head;
			prf.header.video_length = size;
			prf.header.short_range_offset = 100;

			// read video i
			std::vector<int16_t> data(size);
			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
			prf.i.resize(size);
			for (int i = 0; i < size; i++) {
				prf.i[i] = static_cast<double>(data[i]);
				I.push_back(prf.i[i]);
			}

			// read video q
			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
			prf.q.resize(size);
			for (int i = 0; i < size; i++) {
				prf.q[i] = static_cast<double>(data[i]);
				Q.push_back(prf.q[i]);
			}

			// skip other data
			file.seekg(sizeof(int16_t) * size, std::ios::cur);
		}

		file.close();

		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
		this->ui->openGLWidget->setRenderWindow(renderWindow);

		vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
		vtkSmartPointer<vtkContextView> view1 = vtkSmartPointer<vtkContextView>::New();
		view1->GetRenderer()->SetBackground(colors->GetColor3d("black").GetData());
		vtkSmartPointer<vtkTable> table1 = vtkSmartPointer<vtkTable>::New();
		vtkSmartPointer<vtkChartXY> chart1 = vtkSmartPointer<vtkChartXY>::New();

		vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
		arrX->SetName("X Axis");
		table1->AddColumn(arrX);

		vtkSmartPointer<vtkFloatArray> arrY1 = vtkSmartPointer<vtkFloatArray>::New();
		arrY1->SetName("Y1");
		table1->AddColumn(arrY1);

		vtkSmartPointer<vtkFloatArray> arrY2 = vtkSmartPointer<vtkFloatArray>::New();
		arrY2->SetName("Y2");
		table1->AddColumn(arrY2);

		vtkSmartPointer<vtkFloatArray> arrY3 = vtkSmartPointer<vtkFloatArray>::New();
		arrY3->SetName("Y3");
		table1->AddColumn(arrY3);

		vtkSmartPointer<vtkFloatArray> arrY4 = vtkSmartPointer<vtkFloatArray>::New();
		arrY4->SetName("Y4");
		table1->AddColumn(arrY4);

		view1->SetRenderWindow(renderWindow);
		view1->GetScene()->AddItem(chart1);

		// color of titles
		vtkAxis* xAxis = chart1->GetAxis(vtkAxis::BOTTOM);
		xAxis->SetTitle("PRF");
		xAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		xAxis->GetTitleProperties()->SetFontSize(16);
		xAxis->GetTitleProperties()->ItalicOn();
		xAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);

		vtkAxis* yAxis = chart1->GetAxis(vtkAxis::LEFT);
		yAxis->SetTitle("Val");
		yAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
		yAxis->GetTitleProperties()->SetFontSize(16);
		yAxis->GetTitleProperties()->ItalicOn();
		yAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);


		// Remove the grids
		chart1->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
		chart1->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);

		bool paused = false; // Flag to indicate if plotting is paused
		int start = 0;
		// Create a timer to trigger the chart update
		QTimer* timer = new QTimer(this);
		connect(timer, &QTimer::timeout, this, [=]() mutable {

			// Check if the plotting is paused
			if (!paused)
			{
				////High performancing
				chart1->RemovePlot(1);
				chart1->RemovePlot(2);
				view1->RemoveAllObservers();
				view1->RemoveAllRepresentations();
				table1->RemoveAllObservers();
				table1->FIELD_OPERATION_REMOVED;


				// extract data for the current step
				std::vector<double> data(size);
				std::vector<double> data2(size);
				for (int i = 0; i < size; ++i) {
					data[i] = I[start + i];
					data2[i] = Q[start + i];
				}

				// Update the table with the new signal data
				int numPoints = size;
				table1->SetNumberOfRows(numPoints);
				for (int i = 0; i < numPoints; ++i) {
					table1->SetValue(i, 0, i);
					table1->SetValue(i, 1, data[i]);
					table1->SetValue(i, 2, data[i] + 1000);
					table1->SetValue(i, 3, data[i] + 2000);
					table1->SetValue(i, 4, data[i] + 3000);
				}
				// clear the chart and add the new plot
				chart1->ClearPlots();

				vtkSmartPointer<vtkPlot> line1 = chart1->AddPlot(vtkChart::LINE);
				line1->SetInputData(table1, 0, 1);
				line1->SetColor(221, 221, 0, 255);
				line1->SetWidth(1);
				//line1->SetOpacity(0.2);

				vtkSmartPointer<vtkPlot> line2 = chart1->AddPlot(vtkChart::LINE);
				//line1 = chart1->AddPlot(vtkChart::LINE);
				line2->SetInputData(table1, 0, 2);
				line2->SetColor(128, 0, 0, 255);
				line2->SetWidth(1);
				/*line2->SetOpacity(0.2);*/

				line1 = chart1->AddPlot(vtkChart::LINE);
				line1->SetInputData(table1, 0, 3);
				line1->SetColor(0, 51, 102, 255);
				line1->SetWidth(1);

				line1 = chart1->AddPlot(vtkChart::LINE);
				line1->SetInputData(table1, 0, 4);
				line1->SetColor(102, 204, 0, 255);
				line1->SetWidth(1);


				// Wait for 2 ms and update the start index
				start += size;
				if (start + size > I.size()) {
					start = 0;
				}

				// Update the plot with the new data and render the view

				chart1->Update();
				view1->GetRenderWindow()->Render();
				view1->Render();
			}

		});

		// Start the timer to trigger the chart update
		timer->start(2); // Adjust the interval as needed

		// Pause/Resume button click event
		connect(this->ui->DrawPlot_button3, &QPushButton::clicked, this, [=]() mutable {
			paused = !paused;
			if (paused) {
				timer->stop(); // Pause the timer
			}
			else {
				timer->start(); // Resume the timer
			}
			});

		qApp->processEvents();
	}
}


///Old OK without pause

//#include <MainWindow.h>
//#include "ui_MainWindow.h"
//#include <vtkSphereSource.h>
//#include <vtkActor.h>
//#include <vtkPolyDataMapper.h>
//#include <iostream>
//#include <mutex>
//#include <condition_variable>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkBrush.h>
//#include <vtkNew.h>
//#include <array>
//#include <chrono>
//#include <fstream>
//#include <vtkNamedColors.h>
//#include <vtkPlotPoints.h>
//#include <vtkFloatArray.h>
//#include <vector>
//#include <cstdint>
//#include <vtkSmartPointer.h>
//#include <vtkPlot.h>
//#include <vtkContextActor.h>
//#include <vtkContextScene.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkInteractorStyleRubberBandZoom.h>
//#include <vtkTextActor.h>
//#include <vtkAxis.h>
//#include <vtkChartMatrix.h>
//#include <vtkChartXY.h>
//#include <vtkContextView.h>
//#include <vtkPen.h>
//#include <vtkTable.h>
//#include <vtkVersion.h>
//#include <QTimer>
//#include <thread>
//#include <vtkTextProperty.h>
//#include <vtkNamedColors.h>
//#include <QThread>
//
//struct PRFData {
//	struct Header {
//		uint16_t prf_angle;
//		uint16_t prf_number;
//		uint16_t video_length;
//		uint16_t short_range_offset;
//	} header;
//	std::vector<double> i;
//	std::vector<double> q;
//};
//
//MainWindow::MainWindow(QWidget* parent) :
//	QMainWindow(parent),
//	ui(new Ui::MainWindow)
//{
//	this->ui->setupUi(this);
//
//	//os.environ['QT_AUTO_SCREEN_SCALE_FACTOR'] = '1'
//
//	QObject::connect(this->ui->DrawPlot_button, &QPushButton::clicked, this, &MainWindow::onDrawSphereClick);
//	QObject::connect(this->ui->DrawPlot_button2, &QPushButton::clicked, this, &MainWindow::onDrawSphereClick2);
//
//}
//
//MainWindow::~MainWindow() {
//	delete ui;
//}
//
//void MainWindow::onDrawSphereClick()
//{
//
//	vtkNew<vtkNamedColors> colors;
//	if (mChartVisible) {
//		// If the chart is visible, remove it from the view
//		mChartVisible = false;
//		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
//		this->ui->openGLWidget->setRenderWindow(renderWindow);
//		vtkNew<vtkContextView> view;
//		view->SetRenderWindow(renderWindow);
//		view->GetRenderer()->SetBackground(colors->GetColor3d("Black").GetData());
//		//vtkContextView* view = vtkContextView::SafeDownCast(ui->openGLWidget->GetRenderWindow());
//		if (view) {
//			view->GetScene()->ClearItems();
//			view->Render();
//		}
//	}
//	else {
//		mChartVisible = true;
//
//		const int MAX_PRF_ANGLE = 8192; //4096
//		const int size = 2600;
//		std::ifstream file("E:/FAVA/Reading DAT file/IQ_06-07-2023__10(LOG1).dat", std::ios::in | std::ios::binary);
//
//		std::vector<PRFData> prfData(MAX_PRF_ANGLE);
//
//		std::vector<double> I;
//		std::vector<double> Q;
//
//		for (int k = 0; k < MAX_PRF_ANGLE; ++k) {
//			auto& prf = prfData[k];
//
//			// read headers
//			uint16_t head;
//			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
//			prf.header.prf_angle = head;
//			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
//			prf.header.prf_number = head;
//			prf.header.video_length = size;
//			prf.header.short_range_offset = 100;
//
//			// read video i
//			std::vector<int16_t> data(size);
//			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
//			prf.i.resize(size);
//			for (int i = 0; i < size; i++) {
//				prf.i[i] = static_cast<double>(data[i]);
//				I.push_back(prf.i[i]);
//			}
//
//			// read video q
//			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
//			prf.q.resize(size);
//			for (int i = 0; i < size; i++) {
//				prf.q[i] = static_cast<double>(data[i]);
//				Q.push_back(prf.q[i]);
//			}
//
//			// skip other data
//			file.seekg(sizeof(int16_t) * size, std::ios::cur);
//		}
//
//		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
//		this->ui->openGLWidget->setRenderWindow(renderWindow);
//
//		vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
//		vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
//		view->GetRenderWindow()->SetWindowName("PRF Plot");
//		view->GetRenderer()->SetBackground(colors->GetColor3d("black").GetData());
//
//		vtkNew<vtkChartMatrix> matrix;
//		view->GetScene()->AddItem(matrix);
//		matrix->SetSize(vtkVector2i(2, 2));
//		matrix->SetGutter(vtkVector2f(50.0, 50.0));
//		view->SetRenderWindow(renderWindow);
//
//		vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
//
//		vtkSmartPointer<vtkFloatArray> arrX1 = vtkSmartPointer<vtkFloatArray>::New();
//		arrX1->SetName("X Axis");
//		table->AddColumn(arrX1);
//
//		vtkSmartPointer<vtkFloatArray> arrC1 = vtkSmartPointer<vtkFloatArray>::New();
//		arrC1->SetName("PRF");
//		table->AddColumn(arrC1);
//
//		//vtkSmartPointer<vtkFloatArray> arrX2 = vtkSmartPointer<vtkFloatArray>::New();
//		//arrX2->SetName("X Axis");
//		//table->AddColumn(arrX2);
//		//
//		//vtkSmartPointer<vtkFloatArray> arrC2 = vtkSmartPointer<vtkFloatArray>::New();
//		//arrC2->SetName("PRF");
//		//table->AddColumn(arrC2);
//		//
//		//vtkSmartPointer<vtkFloatArray> arrX3 = vtkSmartPointer<vtkFloatArray>::New();
//		//arrX3->SetName("X Axis");
//		//table->AddColumn(arrX3);
//		//
//		//vtkSmartPointer<vtkFloatArray> arrC3 = vtkSmartPointer<vtkFloatArray>::New();
//		//arrC3->SetName("PRF");
//		//table->AddColumn(arrC3);
//		//
//		//vtkSmartPointer<vtkFloatArray> arrX4 = vtkSmartPointer<vtkFloatArray>::New();
//		//arrX4->SetName("X Axis");
//		//table->AddColumn(arrX4);
//		//
//		//vtkSmartPointer<vtkFloatArray> arrC4 = vtkSmartPointer<vtkFloatArray>::New();
//		//arrC4->SetName("PRF");
//		//table->AddColumn(arrC4);
//
//		//charts
//		vtkSmartPointer<vtkChartXY> chart1 = vtkSmartPointer<vtkChartXY>::New();
//		chart1->SetTitle("Signal 1");
//		matrix->SetChart(vtkVector2i(0, 0), chart1);
//
//		vtkSmartPointer<vtkChartXY> chart2 = vtkSmartPointer<vtkChartXY>::New();
//		chart2->SetTitle("Signal 3");
//		matrix->SetChart(vtkVector2i(0, 1), chart2);
//
//		vtkSmartPointer<vtkChartXY> chart3 = vtkSmartPointer<vtkChartXY>::New();
//		chart3->SetTitle("Signal 2");
//		matrix->SetChart(vtkVector2i(1, 0), chart3);
//
//		vtkSmartPointer<vtkChartXY> chart4 = vtkSmartPointer<vtkChartXY>::New();
//		chart4->SetTitle("Signal 4");
//		matrix->SetChart(vtkVector2i(1, 1), chart4);
//
//		// Remove the grids
//		chart1->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
//		chart1->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
//		chart2->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
//		chart2->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
//		chart3->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
//		chart3->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
//		chart4->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
//		chart4->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
//
//		// color of titles
//		//1
//		vtkAxis* xAxis = chart1->GetAxis(vtkAxis::BOTTOM);
//		xAxis->SetTitle("PRF1");
//		xAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		xAxis->GetTitleProperties()->SetFontSize(10);
//		xAxis->GetTitleProperties()->ItalicOn();
//		xAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//		vtkAxis* yAxis = chart1->GetAxis(vtkAxis::LEFT);
//		yAxis->SetTitle("Val");
//		yAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		yAxis->GetTitleProperties()->SetFontSize(10);
//		yAxis->GetTitleProperties()->ItalicOn();
//		yAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//		//2
//		vtkAxis* xAxis2 = chart2->GetAxis(vtkAxis::BOTTOM);
//		xAxis2->SetTitle("PRF2");
//		xAxis2->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		xAxis2->GetTitleProperties()->SetFontSize(10);
//		xAxis2->GetTitleProperties()->ItalicOn();
//		xAxis2->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//		vtkAxis* yAxis2 = chart2->GetAxis(vtkAxis::LEFT);
//		yAxis2->SetTitle("Val");
//		yAxis2->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		yAxis2->GetTitleProperties()->SetFontSize(10);
//		yAxis2->GetTitleProperties()->ItalicOn();
//		yAxis2->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//		//2
//		vtkAxis* xAxis3 = chart3->GetAxis(vtkAxis::BOTTOM);
//		xAxis3->SetTitle("PRF3");
//		xAxis3->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		xAxis3->GetTitleProperties()->SetFontSize(10);
//		xAxis3->GetTitleProperties()->ItalicOn();
//		xAxis3->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//		vtkAxis* yAxis3 = chart3->GetAxis(vtkAxis::LEFT);
//		yAxis3->SetTitle("Val");
//		yAxis3->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		yAxis3->GetTitleProperties()->SetFontSize(10);
//		yAxis3->GetTitleProperties()->ItalicOn();
//		yAxis3->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//		//2
//		vtkAxis* xAxis4 = chart4->GetAxis(vtkAxis::BOTTOM);
//		xAxis4->SetTitle("PRF4");
//		xAxis4->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		xAxis4->GetTitleProperties()->SetFontSize(10);
//		xAxis4->GetTitleProperties()->ItalicOn();
//		xAxis4->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//		vtkAxis* yAxis4 = chart4->GetAxis(vtkAxis::LEFT);
//		yAxis4->SetTitle("Val");
//		yAxis4->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		yAxis4->GetTitleProperties()->SetFontSize(10);
//		yAxis4->GetTitleProperties()->ItalicOn();
//		yAxis4->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//
//		int start = 0;
//
//		while (true) 
//		{
//			////High perfomrmancing
//			chart1->RemovePlot(1);
//			chart2->RemovePlot(1);
//			chart3->RemovePlot(1);
//			chart4->RemovePlot(1);
//			chart1->RemovePlot(1);
//			chart1->RemovePlot(2);
//			chart1->RemovePlot(3);
//			chart1->RemovePlot(4);
//			view->RemoveAllObservers();
//			view->RemoveAllRepresentations();
//			table->RemoveAllObservers();
//
//			// extract data for the current step
//			std::vector<double> data(size);
//			std::vector<double> data2(size);
//			for (int i = 0; i < size; ++i)
//			{
//				data[i] = I[start + i];
//				data2[i] = Q[start + i];
//			}
//
//			int numPoints = data.size();
//			table->SetNumberOfRows(numPoints);
//			for (int i = 0; i < numPoints; ++i) {
//				table->SetValue(i, 0, i);
//				table->SetValue(i, 1, data[i]);
//				//table->SetValue(i, 1, data[i]);
//				//table->SetValue(i, 1, data[i]);
//				//table->SetValue(i, 1, data[i]);
//			}
//
//			//1
//			vtkSmartPointer<vtkPlot> line1 = chart1->AddPlot(vtkChart::LINE);
//			line1->SetInputData(table, 0, 1);
//			line1->SetColor(221, 221, 0, 255);
//			line1->SetWidth(.7);
//
//			//2
//			vtkSmartPointer<vtkPlot> line2 = chart2->AddPlot(vtkChart::LINE);
//			line2->SetInputData(table, 0, 1);
//			line2->SetColor(128, 0, 0, 255);
//			line2->SetWidth(.7);
//
//			//3
//			vtkSmartPointer<vtkPlot> line3 = chart3->AddPlot(vtkChart::LINE);
//			line3->SetInputData(table, 0, 1);
//			line3->SetColor(0, 51, 102, 255);
//			line3->SetWidth(.7);
//
//			//4
//			vtkSmartPointer<vtkPlot> line4 = chart4->AddPlot(vtkChart::LINE);
//			line4->SetInputData(table, 0, 1);
//			line4->SetColor(102, 204, 0, 255);
//			line4->SetWidth(.7);
//
//			// Update the plot with the new data
//			chart1->Update();
//			chart2->Update();
//			chart3->Update();
//			chart4->Update();
//
//			view->GetRenderWindow()->Render();
//			view->Render();
//
//			// Wait for 2 ms and update the start index
//
//			start += size;
//			if (start + size > I.size())
//			{
//				start = 0;
//			}
//			std::this_thread::sleep_for(std::chrono::milliseconds(20));
//			qApp->processEvents();
//		}
//
//		table->Delete();
//		view->Delete();
//	}
//
//
//}
//
//void MainWindow::onDrawSphereClick2()
//{
//
//	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
//
//	if (mChartVisible) {
//		// If the chart is visible, remove it from the view
//		mChartVisible = false;
//		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
//		this->ui->openGLWidget->setRenderWindow(renderWindow);
//		vtkNew<vtkContextView> view;
//		view->SetRenderWindow(renderWindow);
//		view->GetRenderer()->SetBackground(colors->GetColor3d("Black").GetData());
//		//vtkContextView* view = vtkContextView::SafeDownCast(ui->openGLWidget->GetRenderWindow());
//		if (view) {
//			view->GetScene()->ClearItems();
//			view->Render();
//		}
//	}
//	else
//	{
//		mChartVisible = true;
//
//		const int MAX_PRF_ANGLE = 8192; //4096
//		const int size = 2600;
//		std::ifstream file("E:/FAVA/Reading DAT file/IQ_06-07-2023__10(LOG1).dat", std::ios::in | std::ios::binary);
//
//		std::vector<PRFData> prfData(MAX_PRF_ANGLE);
//
//		std::vector<double> I;
//		std::vector<double> Q;
//
//		for (int k = 0; k < MAX_PRF_ANGLE; ++k) {
//			auto& prf = prfData[k];
//
//			// read headers
//			uint16_t head;
//			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
//			prf.header.prf_angle = head;
//			file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
//			prf.header.prf_number = head;
//			prf.header.video_length = size;
//			prf.header.short_range_offset = 100;
//
//			// read video i
//			std::vector<int16_t> data(size);
//			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
//			prf.i.resize(size);
//			for (int i = 0; i < size; i++) {
//				prf.i[i] = static_cast<double>(data[i]);
//				I.push_back(prf.i[i]);
//			}
//
//			// read video q
//			file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
//			prf.q.resize(size);
//			for (int i = 0; i < size; i++) {
//				prf.q[i] = static_cast<double>(data[i]);
//				Q.push_back(prf.q[i]);
//			}
//
//			// skip other data
//			file.seekg(sizeof(int16_t) * size, std::ios::cur);
//		}
//
//		vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
//		this->ui->openGLWidget->setRenderWindow(renderWindow);
//
//		vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
//		vtkSmartPointer<vtkContextView> view1 = vtkSmartPointer<vtkContextView>::New();
//		view1->GetRenderer()->SetBackground(colors->GetColor3d("black").GetData());
//		vtkSmartPointer<vtkTable> table1 = vtkSmartPointer<vtkTable>::New();
//		vtkSmartPointer<vtkChartXY> chart1 = vtkSmartPointer<vtkChartXY>::New();
//
//		vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
//		arrX->SetName("X Axis");
//		table1->AddColumn(arrX);
//
//		vtkSmartPointer<vtkFloatArray> arrY1 = vtkSmartPointer<vtkFloatArray>::New();
//		arrY1->SetName("Y1");
//		table1->AddColumn(arrY1);
//
//		vtkSmartPointer<vtkFloatArray> arrY2 = vtkSmartPointer<vtkFloatArray>::New();
//		arrY2->SetName("Y2");
//		table1->AddColumn(arrY2);
//
//		vtkSmartPointer<vtkFloatArray> arrY3 = vtkSmartPointer<vtkFloatArray>::New();
//		arrY3->SetName("Y3");
//		table1->AddColumn(arrY3);
//
//		vtkSmartPointer<vtkFloatArray> arrY4 = vtkSmartPointer<vtkFloatArray>::New();
//		arrY4->SetName("Y4");
//		table1->AddColumn(arrY4);
//
//		view1->SetRenderWindow(renderWindow);
//		view1->GetScene()->AddItem(chart1);
//
//		// color of titles
//		vtkAxis* xAxis = chart1->GetAxis(vtkAxis::BOTTOM);
//		xAxis->SetTitle("PRF");
//		xAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		xAxis->GetTitleProperties()->SetFontSize(16);
//		xAxis->GetTitleProperties()->ItalicOn();
//		xAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//		vtkAxis* yAxis = chart1->GetAxis(vtkAxis::LEFT);
//		yAxis->SetTitle("Val");
//		yAxis->GetTitleProperties()->SetColor(0.5, 0.5, 0.5);
//		yAxis->GetTitleProperties()->SetFontSize(16);
//		yAxis->GetTitleProperties()->ItalicOn();
//		yAxis->GetLabelProperties()->SetColor(0.5, 0.5, 0.5);
//
//
//		// Remove the grids
//		chart1->GetAxis(vtkAxis::LEFT)->SetGridVisible(false);
//		chart1->GetAxis(vtkAxis::BOTTOM)->SetGridVisible(false);
//
//		int start = 0;
//		while (true) {
//
//			////High performancing
//			chart1->RemovePlot(1);
//			chart1->RemovePlot(2);
//			view1->RemoveAllObservers();
//			view1->RemoveAllRepresentations();
//			table1->RemoveAllObservers();
//			table1->FIELD_OPERATION_REMOVED;
//
//
//			// extract data for the current step
//			std::vector<double> data(size);
//			std::vector<double> data2(size);
//			for (int i = 0; i < size; ++i) {
//				data[i] = I[start + i];
//				data2[i] = Q[start + i];
//			}
//
//			// Update the table with the new signal data
//			int numPoints = size;
//			table1->SetNumberOfRows(numPoints);
//			for (int i = 0; i < numPoints; ++i) {
//				table1->SetValue(i, 0, i);
//				table1->SetValue(i, 1, data[i]);
//				table1->SetValue(i, 2, data[i] + 1000);
//				table1->SetValue(i, 3, data[i] + 2000);
//				table1->SetValue(i, 4, data[i] + 3000);
//			}
//			// clear the chart and add the new plot
//			chart1->ClearPlots();
//
//			vtkSmartPointer<vtkPlot> line1 = chart1->AddPlot(vtkChart::LINE);
//			line1->SetInputData(table1, 0, 1);
//			line1->SetColor(221, 221, 0, 255);
//			line1->SetWidth(1);
//			//line1->SetOpacity(0.2);
//
//			vtkSmartPointer<vtkPlot> line2 = chart1->AddPlot(vtkChart::LINE);
//			//line1 = chart1->AddPlot(vtkChart::LINE);
//			line2->SetInputData(table1, 0, 2);
//			line2->SetColor(128, 0, 0, 255);
//			line2->SetWidth(1);
//			/*line2->SetOpacity(0.2);*/
//
//			line1 = chart1->AddPlot(vtkChart::LINE);
//			line1->SetInputData(table1, 0, 3);
//			line1->SetColor(0, 51, 102, 255);
//			line1->SetWidth(1);
//
//			line1 = chart1->AddPlot(vtkChart::LINE);
//			line1->SetInputData(table1, 0, 4);
//			line1->SetColor(102, 204, 0, 255);
//			line1->SetWidth(1);
//
//
//			// Wait for 2 ms and update the start index
//			start += size;
//			if (start + size > I.size()) {
//				start = 0;
//			}
//
//			// Update the plot with the new data and render the view
//
//			chart1->Update();
//			view1->GetRenderWindow()->Render();
//			view1->Render();
//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
//
//			qApp->processEvents();
//		}
//		chart1->Delete();
//		table1->Delete();
//		view1->Delete();
//	}
//}