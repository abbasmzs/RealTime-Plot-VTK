/////plot random signal in for loop : It is OK

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <vtkSmartPointer.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkContextView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>

class Signal {
public:
    Signal(std::vector<double> data) : data_(data) {}
    std::vector<double> data() const { return data_; }
private:
    std::vector<double> data_;
};

int main() {
    // Create a vector with 2600 elements
    std::vector<double> signal_data(2600);

    // Set up the view
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderWindow()->SetWindowName("PRF Plot");
    view->GetRenderWindow()->SetSize(1000, 800);
    view->GetRenderer()->SetBackground(colors->GetColor3d("slate_grey_light").GetData());

    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
    arrX->SetName("X Axis");
    table->AddColumn(arrX);

    vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
    arrC->SetName("PRF");
    table->AddColumn(arrC);

    vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem(chart);

    //view->GetRenderWindow()->Render();

    // Render the view
    //view->GetRenderWindow()->Render();

    while (true) {
        
        chart->RemovePlot(1);
        //chart = vtkSmartPointer<vtkChartXY>::New();
        // Simulate receiving the signal data from a UDP or TCP socket
        // Here, we just fill the vector with random values
        for (int i = 0; i < signal_data.size(); i++) {
            signal_data[i] = rand() % 100;
        }

        // Create a signal with the vector as data
        Signal signal(signal_data);

        // Update the table with the new signal data
        int numPoints = signal_data.size();
        table->SetNumberOfRows(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            table->SetValue(i, 0, i);
            table->SetValue(i, 1, signal_data[i]);
        }

        // Add multiple line plots, setting the colors etc.
        vtkSmartPointer<vtkPlot> line = chart->AddPlot(vtkChart::LINE);
        line->SetInputData(table, 0, 1);
        line->SetColor(3, 46, 202, 255);
        line->SetWidth(.8);

        // Update the plot with the new data
        chart->Update();

        // Render the view
        view->GetRenderWindow()->Render();
        //view->Update();

        // Pause for 1ms
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //line->Delete();
    }

    //chart->Delete();
    //table->Delete();
    //view->Delete();

    // Initialize and start the interactor
    view->GetInteractor()->Initialize();
    view->GetInteractor()->Start();
}

//ba payini connectesh kon
////miss bagheri help poe //with error

//#include <vtkSmartPointer.h>
//#include <vtkChartXY.h>
//#include <vtkPlotPoints.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include <vtkDataArray.h>
//#include <vtkTable.h>
//#include <vtkTimerLog.h>
//#include <vtkMultiThreader.h>
//
//vtkSmartPointer<vtkChartXY> chart;
//vtkSmartPointer<vtkPlotPoints> plot;
//vtkSmartPointer<vtkTable> table;
//vtkSmartPointer<vtkDataArray> xData;
//vtkSmartPointer<vtkDataArray> yData;
//vtkSmartPointer<vtkRenderWindow> renderWindow;
//vtkSmartPointer<vtkRenderWindowInteractor> interactor;
//
//void UpdatePlot()
//{
//    // Generate new data
//    double x = xData->GetTuple1(xData->GetNumberOfTuples() - 1) + 1;
//    double y = vtkMath::Random(-1.0, 1.0);
//
//    // Update the plot data
//    xData->InsertNextTuple1(x);
//    yData->InsertNextTuple1(y);
//    table->Modified();
//
//    // Trigger a render
//    renderWindow->Render();
//}
//
//void TimerCallbackFunction(vtkObject* caller, long unsigned int eventId, void* clientData, void* callData)
//{
//    vtkTimerLog::MarkStartEvent("UpdatePlot");
//
//    UpdatePlot();
//
//    vtkTimerLog::MarkEndEvent("UpdatePlot");
//}
//
//int main(int argc, char* argv[])
//{
//    // Create the chart and plot
//    chart = vtkSmartPointer<vtkChartXY>::New();
//    plot = vtkSmartPointer<vtkPlotPoints>::New();
//    chart->AddPlot(plot);
//
//    // Create the data table
//    table = vtkSmartPointer<vtkTable>::New();
//    xData = vtkSmartPointer<vtkDataArray>::New();
//    xData->SetName("X");
//    yData = vtkSmartPointer<vtkDataArray>::New();
//    yData->SetName("Y");
//    table->AddColumn(xData);
//    table->AddColumn(yData);
//
//    // Initialize the plot data
//    xData->InsertNextTuple1(0.0);
//    yData->InsertNextTuple1(0.0);
//    plot->SetInputData(table, "X", "Y");
//
//    // Create the render window and interactor
//    renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//    renderWindow->SetSize(640, 480);
//    renderWindow->SetMultiSamples(0);
//    interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    interactor->SetRenderWindow(renderWindow);
//
//    // Add the chart to the renderer and start the interactor
//    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
//    renderer->SetBackground(1, 1, 1);
//    renderer->AddActor(chart);
//    
//    renderWindow->AddRenderer(renderer);
//    renderWindow->Render();
//    interactor->Start();
//
//    // Set up the timer to update the plot
//    vtkSmartPointer<vtkCallbackCommand> timerCallback = vtkSmartPointer<vtkCallbackCommand>::New();
//    timerCallback->SetCallback(TimerCallbackFunction);
//    interactor->AddObserver(vtkCommand::TimerEvent, timerCallback);
//    interactor->CreateRepeatingTimer(1);
//
//    // Enter the main loop
//    interactor->Start();
//
//    return 0;
//}

//inm vase griding
////basic chart martrix

//#if VTK_VERSION_NUMBER >= 90220220630ULL
//#define VTK_HAS_SETCOLORF 1
//#endif
//
////----------------------------------------------------------------------------
//int main(int, char*[])
//{
//  vtkNew<vtkNamedColors> colors;
//
//  // Set up a 2D scene, add an XY chart to it
//  vtkNew<vtkContextView> view;
//  view->GetRenderWindow()->SetSize(1280, 1024);
//  view->GetRenderWindow()->SetWindowName("ChartMatrix");
//
//  vtkNew<vtkChartMatrix> matrix;
//  view->GetScene()->AddItem(matrix);
//  matrix->SetSize(vtkVector2i(2, 2));
//  matrix->SetGutter(vtkVector2f(30.0, 30.0));
//
//  // Create a table with some points in it...
//  vtkNew<vtkTable> table;
//  vtkNew<vtkFloatArray> arrX;
//
//  arrX->SetName("X Axis");
//  table->AddColumn(arrX);
//
//  vtkNew<vtkFloatArray> arrC;
//  arrC->SetName("Cosine");
//  table->AddColumn(arrC);
//
//  vtkNew<vtkFloatArray> arrS;
//  arrS->SetName("Sine");
//  table->AddColumn(arrS);
//
//  vtkNew<vtkFloatArray> arrS2;
//  arrS2->SetName("Sine2");
//  table->AddColumn(arrS2);
//
//  vtkNew<vtkFloatArray> tangent;
//  tangent->SetName("Tangent");
//  table->AddColumn(tangent);
//
//  int numPoints = 42;
//  float inc = 7.5 / (numPoints - 1);
//  table->SetNumberOfRows(numPoints);
//  for (int i = 0; i < numPoints; ++i)
//  {
//    table->SetValue(i, 0, i * inc);
//    table->SetValue(i, 1, cos(i * inc));
//    table->SetValue(i, 2, sin(i * inc));
//    table->SetValue(i, 3, sin(i * inc) + 0.5);
//    table->SetValue(i, 4, tan(i * inc));
//  }
//
//  // Add multiple line plots, setting the colors etc
//  // lower left plot, a point chart
//  vtkChart* chart = matrix->GetChart(vtkVector2i(0, 0));
//  vtkPlot* plot = chart->AddPlot(vtkChart::POINTS);
//  plot->SetInputData(table, 0, 1);
//  dynamic_cast<vtkPlotPoints*>(plot)->SetMarkerStyle(vtkPlotPoints::DIAMOND);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("sea_green").GetRed(),
//                 colors->GetColor3ub("sea_green").GetGreen(),
//                 colors->GetColor3ub("sea_green").GetBlue(), 255);
//
//  // upper left plot, a point chart
//  chart = matrix->GetChart(vtkVector2i(0, 1));
//  plot = chart->AddPlot(vtkChart::POINTS);
//  plot->SetInputData(table, 0, 2);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("rose_madder").GetRed(),
//                 colors->GetColor3ub("rose_madder").GetGreen(),
//                 colors->GetColor3ub("rose_madder").GetBlue(), 255);
//
//  //
//  chart = matrix->GetChart(vtkVector2i(1, 0));
//  plot = chart->AddPlot(vtkChart::LINE);
//  plot->SetInputData(table, 0, 3);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("dark_orange").GetRed(),
//                 colors->GetColor3ub("dark_orange").GetGreen(),
//                 colors->GetColor3ub("dark_orange").GetBlue(), 255);
//
//  // upper right plot, a bar and point chart
//  chart = matrix->GetChart(vtkVector2i(1, 1));
//  plot = chart->AddPlot(vtkChart::BAR);
//  plot->SetInputData(table, 0, 4);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("burnt_sienna").GetRed(),
//                 colors->GetColor3ub("burnt_sienna").GetGreen(),
//                 colors->GetColor3ub("burnt_sienna").GetBlue(), 255);
//
//  plot = chart->AddPlot(vtkChart::POINTS);
//  plot->SetInputData(table, 0, 1);
//  dynamic_cast<vtkPlotPoints*>(plot)->SetMarkerStyle(vtkPlotPoints::CROSS);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("rose_madder").GetRed(),
//                 colors->GetColor3ub("rose_madder").GetGreen(),
//                 colors->GetColor3ub("rose_madder").GetBlue(), 255);
//
//  // Lower right plot, a line chart
//
//  chart = matrix->GetChart(vtkVector2i(1, 0));
//  plot = chart->AddPlot(vtkChart::LINE);
//  plot->SetInputData(table, 0, 3);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("dark_orange").GetRed(),
//                 colors->GetColor3ub("dark_orange").GetGreen(),
//                 colors->GetColor3ub("dark_orange").GetBlue(), 255);
//
//  plot = chart->AddPlot(vtkChart::LINE);
//  plot->SetInputData(table, 0, 3);
//#if VTK_HAS_SETCOLORF
//  plot->GetXAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColorF(
//      colors->GetColor3d("warm_grey").GetData());
//#else
//  plot->GetXAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//  plot->GetYAxis()->GetGridPen()->SetColor(
//      colors->GetColor3d("warm_grey").GetData());
//#endif
//  plot->SetColor(colors->GetColor3ub("royal_blue").GetRed(),
//                 colors->GetColor3ub("royal_blue").GetGreen(),
//                 colors->GetColor3ub("royal_blue").GetBlue(), 255);
//
//  // Finally render the scene and compare the image to a reference image
//  view->GetRenderer()->SetBackground(
//      colors->GetColor3d("navajo_white").GetData());
//  view->GetRenderWindow()->Render();
//  view->GetInteractor()->Initialize();
//  view->GetInteractor()->Start();
//  return EXIT_SUCCESS;
//}

/////generating data thread and camera o grid //with error

//#include <iostream>
//#include <vector>
//#include <chrono>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include <vtkSmartPointer.h>
//#include <vtkTable.h>
//#include <vtkFloatArray.h>
//#include <vtkChartXY.h>
//#include <vtkPlot.h>
//#include <vtkContextView.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkNamedColors.h>
//#include <vtkPlot.h>
//#include <vtkAxis.h>
//#include <vtkBrush.h>
//#include <vtkChartXY.h>
//#include <vtkContextActor.h>
//#include <vtkContextScene.h>
//#include <vtkFloatArray.h>
//#include <vtkNamedColors.h>
//#include <vtkNew.h>
//#include <vtkPen.h>
//#include <vtkPlot.h>
//#include <vtkPlotPoints.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include <vtkTable.h>
//#include <vtkVersion.h>
//
//class Signal {
//public:
//    Signal(std::vector<double> data) : data_(data) {}
//    std::vector<double> data() const { return data_; }
//private:
//    std::vector<double> data_;
//};
//
//void receiveSignal(std::vector<double>& signal_data, std::mutex& signal_mutex, std::condition_variable& signal_cv) {
//    while (true) {
//        // Simulate receiving the signal data from a UDP or TCP socket
//        // Here, we just fill the vector with random values
//        for (int i = 0; i < signal_data.size(); i++) {
//            signal_data[i] = rand() % 100;
//        }
//
//        // Create a signal with the vector as data
//        Signal signal(signal_data);
//
//        // Lock the mutex and update the signal data
//        {
//            std::unique_lock<std::mutex> lock(signal_mutex);
//            signal_data = signal.data();
//        }
//
//        // Notify the condition variable that new signal data is available
//        signal_cv.notify_one();
//
//        // Pause for 1ms
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//    }
//}
//
//void plotSignal(std::vector<double>& signal_data, std::mutex& signal_mutex, std::condition_variable& signal_cv) {
//    // Set up the view
//    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
//    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//    renderWindow->SetWindowName("PRF Plot");
//    renderWindow->SetSize(800, 400);
//
//    // Create two renderers and set up their viewports
//    vtkSmartPointer<vtkRenderer> leftRenderer = vtkSmartPointer<vtkRenderer>::New();
//    leftRenderer->SetViewport(0.0, 0.0, 0.5, 1.0);
//    leftRenderer->SetBackground(colors->GetColor3d("slate_grey_light").GetData());
//    leftRenderer->GetActiveCamera()->ParallelProjectionOn();
//    leftRenderer->GetActiveCamera()->SetParallelScale(1.0);
//    leftRenderer->GetRenderWindow()->SetSize(800, 400);
//    leftRenderer->GetRenderWindow()->SetPosition(0, 0);
//    leftRenderer->GetRenderWindow()->SetBorderSize(2);
//    leftRenderer->GetRenderWindow()->SetWindowName("Left Viewport");
//    renderWindow->AddRenderer(leftRenderer);
//
//    vtkSmartPointer<vtkRenderer> rightRenderer = vtkSmartPointer<vtkRenderer>::New();
//    rightRenderer->SetViewport(0.5, 0.0, 1.0, 1.0);
//    rightRenderer->SetBackground(colors->GetColor3d("slate_grey_light").GetData());
//    rightRenderer->GetActiveCamera()->ParallelProjectionOn();
//    rightRenderer->GetActiveCamera()->SetParallelScale(1.0);
//    rightRenderer->GetRenderWindow()->SetSize(800, 400);
//    rightRenderer->GetRenderWindow()->SetPosition(800, 0);
//    rightRenderer->GetRenderWindow()->SetBorderSize(2);
//    rightRenderer->GetRenderWindow()->SetWindowName("Right Viewport");
//    renderWindow->AddRenderer(rightRenderer);
//
//    // Set up the table with the signal data
//    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();
//    vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
//    arrX->SetName("X Axis");
//    table->AddColumn(arrX);
//    vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
//    arrC->SetName("PRF");
//    table->AddColumn(arrC);
//
//    // Set up the chart properties
//    vtkSmartPointer<vtkChartXY> chart1 = vtkSmartPointer<vtkChartXY>::New();
//    vtkSmartPointer<vtkChartXY> chart2 = vtkSmartPointer<vtkChartXY>::New();
//    leftRenderer->GetActiveCamera()->ParallelProjectionOn();
//    leftRenderer->GetActiveCamera()->SetParallelScale(1.0);
//    rightRenderer->GetActiveCamera()->ParallelProjectionOn();
//    rightRenderer->GetActiveCamera()->SetParallelScale(1.0);
//    leftRenderer->GetRenderWindow()->SetSize(800, 400);
//    rightRenderer->GetRenderWindow()->SetSize(800, 400);
//    leftRenderer->GetRenderWindow()->SetBorderSize(2);
//    rightRenderer->GetRenderWindow()->SetBorderSize(2);
//    leftRenderer->GetRenderWindow()->SetPosition(0, 0);
//    rightRenderer->GetRenderWindow()->SetPosition(800, 0);
//
//    vtkSmartPointer<vtkContextScene> leftScene = vtkSmartPointer<vtkContextScene>::New();
//    leftScene->AddItem(chart1);
//    leftRenderer->SetScene(leftScene);
//
//    vtkSmartPointer<vtkContextScene> rightScene = vtkSmartPointer<vtkContextScene>::New();
//    rightScene->AddItem(chart2);
//    rightRenderer->SetScene(rightScene);
//
//    // Add the line plots to the charts
//    vtkSmartPointer<vtkPlot> line1 = chart1->AddPlot(vtkChart::LINE);
//    line1->SetInputData(table, 0, 1);
//    line1->SetColor(3, 46, 202, 255);
//    line1->SetWidth(.8);
//
//    vtkSmartPointer<vtkPlot> line2 = chart2->AddPlot(vtkChart::LINE);
//    line2->SetInputData(table, 0, 1);
//    line2->SetColor(3, 46, 202, 255);
//    line2->SetWidth(.8);
//
//    // Render the view
//    renderWindow->Render();
//
//    while (true) {
//        // Wait for new signal data to become available
//        std::unique_lock<std::mutex> lock(signal_mutex);
//        signal_cv.wait(lock);
//
//        // Get the signal data
//        std::vector<double> data = signal_data;
//
//        // Update the table with the new signal data
//        int numPoints = data.size();
//        table->SetNumberOfRows(numPoints);
//        for (int i = 0; i < numPoints; ++i) {
//            table->SetValue(i, 0, i);
//            table->SetValue(i, 1, data[i]);
//        }
//
//        // Update the plots with the new data
//        chart1->Update();
//        chart2->Update();
//
//        // Render the view
//        renderWindow->Render();
//    }
//
//    // Clean up
//    line1->Delete();
//    line2->Delete();
//    chart1->Delete();
//    chart2->Delete();
//    table->Delete();
//    renderWindow->Delete();
//
//    // Initialize and start the interactor
//    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    interactor->SetRenderWindow(renderWindow);
//    interactor->Initialize();
//    interactor->Start();
//}
//
//int main() {
//    // Create a vector with 2600 elements
//    std::vector<double> signal_data(2600);
//
//    // Create a mutex and condition variable for synchronizing the signal data between threads
//    std::mutex signal_mutex;
//    std::condition_variable signal_cv;
//
//    // Create a thread for receiving signals
//    std::thread receive_thread(receiveSignal, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv));
//
//    // Create a thread for plotting signals
//    std::thread plot_thread(plotSignal, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv));
//
//
//    // Wait for both threads to finish
//    receive_thread.join();
//    plot_thread.join();
//
//    return 0;
//}


/////OLd for text plot with basic VTK

//int main(int, char* [])
//{
    ////reading text file
    //// Create a new SmartVTK vtkIntArray with shape 1000x2600
    //vtkSmartPointer<vtkIntArray> array = vtkSmartPointer<vtkIntArray>::New();
    //array->SetNumberOfComponents(1);
    //array->SetNumberOfTuples(1000 * 2600);
    //std::ifstream file("E:/WORK/FAVA/Projects/Visual/LinePlot/I.txt", std::ios::in);
    //if (file.is_open()) {
    //    // Loop through each row of elements in the file
    //    for (int i = 0; i < 1000; i++) {
    //        // Loop through each element in the row
    //        for (int j = 0; j < 2600; j++) {
    //            // Read the element from the file
    //            int value;
    //            file >> value;
    //            // Set the value of the corresponding element in the array
    //            array->SetValue(i * 2600 + j, value);
    //            // Ignore the comma separator between elements
    //            file.ignore();
    //        }
    //    }
    //    file.close();
    //}
    //else {
    //    std::cout << "Unable to open file";
    //}
    //// Get the shape of the array
    //int numRows = array->GetNumberOfTuples();
    //int numCols = array->GetNumberOfComponents();
    //std::cout << "Shape: (" << numRows << ", " << numCols << ")" << std::endl;
//    vtkNew<vtkNamedColors> colors;
//    // Set up the view
//    vtkNew<vtkContextView> view;
//    view->GetRenderWindow()->SetWindowName("PRF Plot");
//    view->GetRenderWindow()->SetSize(1000, 800);
//    view->GetRenderer()->SetBackground(colors->GetColor3d("slate_grey_light").GetData());
//
//    // Render the view
//    view->GetRenderWindow()->Render();
//
//    vtkNew<vtkTable> table;
//
//    vtkNew<vtkFloatArray> arrX;
//    arrX->SetName("X Axis");
//    table->AddColumn(arrX);
//
//    vtkNew<vtkFloatArray> arrC;
//    arrC->SetName("PRF");
//    table->AddColumn(arrC);
//
//    vtkNew<vtkChartXY> chart;
//    view->GetScene()->AddItem(chart);
//
//    // Iterate over each row of the input array and plot it as a separate line plot
//    for (int row = 0; row < surface->row(); ++row) {
//
//        // Fill in the table with some example values.
//        int numPoints = surface->col();
//        table->SetNumberOfRows(numPoints);
//        for (int i = 0; i < numPoints; ++i) {
//            table->SetValue(i, 0, i);
//            table->SetValue(i, 1, (*surface)(row, i));
//        }
//
//        // Add multiple line plots, setting the colors etc.
//        vtkPlot* line = chart->AddPlot(vtkChart::LINE);
//        line->SetInputData(table, 0, 1);
//        line->SetColor(3, 46, 202, 255);
//        line->SetWidth(.8);
//        // Update the plot
//        chart->Update();
//        // Render the view
//        view->GetRenderWindow()->Render();
//
//        // Pause for 1ms
//        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//        // Clear the cache memory to free up memory
//        vtkObject::GlobalWarningDisplayOff();
//        line->Delete();
//        table->Delete();
//        view->Delete();
//
//    }
//    vtkObject::GlobalWarningDisplayOn();  // Enable warnings about memory usage
//    chart->Delete();
//
//    // Initialize and start the interactor
//    view->GetInteractor()->Initialize();
//    view->GetInteractor()->Start();
//}
