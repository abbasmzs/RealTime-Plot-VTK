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
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdint>
#include <fstream>
#include <cstdint>
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
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>
#include <cstdint>
#include <fstream>
#include <cstdint>
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
		//view->GetRenderWindow()->SetWindowName("renderWindow");
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

		//view->GetRenderWindow()->SetMultiSamples(0);
		view->Render();
	}
}

void MainWindow::onDrawSphereClick2()
{
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    this->ui->openGLWidget->setRenderWindow(renderWindow);

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

    // check the shape of I and Q vectors
    //std::cout << "Shape of I: " << I.size() / size << " x " << size << std::endl;
    //std::cout << "Shape of Q: " << Q.size() / size << " x " << size << std::endl;

    // Set up the view
    
    vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
    view->SetRenderWindow(renderWindow);

    //view1->GetRenderWindow()->SetWindowName("PRF Plot");
    //view1->GetRenderWindow()->SetSize(1000, 700);
    view->GetRenderer()->SetBackground(colors->GetColor3d("slate_grey_light").GetData());

    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
    arrX->SetName("X Axis");
    table->AddColumn(arrX);

    vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
    arrC->SetName("PRF");
    table->AddColumn(arrC);

    vtkNew<vtkFloatArray> arrS;
    arrS->SetName("PRF2");
    table->AddColumn(arrS);

    vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
    //chart1->SetTitle("Area Plot");
    view->GetScene()->AddItem(chart);

    int start = 0;
    while (true) {

        // High performancing
        //chart->RemovePlot(1);
        //chart->RemovePlot(2);
        //view->RemoveAllObservers();
        //view->RemoveAllRepresentations();
        //table->RemoveAllObservers();

        // extract data for the current step
        std::vector<double> data(size);
        std::vector<double> data2(size);
        for (int i = 0; i < size; ++i) {
            data[i] = I[start + i];
            data2[i] = Q[start + i];
        }

        // Update the table with the new signal data
        int numPoints = size;
        table->SetNumberOfRows(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            table->SetValue(i, 0, i);
            table->SetValue(i, 1, data[i]);
            table->SetValue(i, 2, data2[i]);
        }

        // clear the chart and add the new plot
        chart->ClearPlots();
        vtkSmartPointer<vtkPlot> line = chart->AddPlot(vtkChart::LINE);
        line->SetInputData(table, 0, 1);
        line->SetColor(50, 70, 245, 255);
        line->SetWidth(1);

        line = chart->AddPlot(vtkChart::LINE);
        line->SetInputData(table, 0, 2);
        line->SetColor(255, 0, 0, 255);
        line->SetWidth(1);

        // Update the plot with the new data and render the view
        chart->Update();
        //view->GetRenderWindow()->Render();

        // Wait for 2 ms and update the start index
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        start += size;
        if (start + size > I.size()) {
            start = 0;
        }

        
        //view->GetRenderWindow()->SetWindowName("renderWindow");
        view->GetRenderWindow()->SetMultiSamples(0);
        view->Render();
    }

    chart->Delete();
    table->Delete();
    view->Delete();

}