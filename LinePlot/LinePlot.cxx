////debuged with mrs bagheri with error

///thread is ok

//#include <vtkSmartPointer.h>
//#include <vtkChartXY.h>
//#include <vtkPlot.h>
//#include <vtkTable.h>
//#include <vtkDoubleArray.h>
//#include <vtkContextView.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkMath.h>
//#include <vtkTimerLog.h>
//#include <vtkAxis.h>
//#include <cmath>
//
//#define M_PI 3.14159265358979323846
//
//class RealTimePlot {
//public:
//    RealTimePlot() {
//        // Create a table to hold the data
//        table_ = vtkSmartPointer<vtkTable>::New();
//
//        // Create an array to hold the x values
//        vtkSmartPointer<vtkDoubleArray> xArray = vtkSmartPointer<vtkDoubleArray>::New();
//        xArray->SetName("X");
//        table_->AddColumn(xArray);
//
//        // Create an array to hold the y values
//        vtkSmartPointer<vtkDoubleArray> yArray = vtkSmartPointer<vtkDoubleArray>::New();
//        yArray->SetName("Y");
//        table_->AddColumn(yArray);
//
//        /// Create a line chart
//        chart_ = vtkSmartPointer<vtkChartXY>::New();
//        chart_->GetAxis(vtkAxis::LEFT)->SetTitle("Signal");
//        chart_->GetAxis(vtkAxis::BOTTOM)->SetTitle("Time");
//
//        // Add the chart to a context view
//        contextView_ = vtkSmartPointer<vtkContextView>::New();
//        contextView_->GetRenderWindow()->SetWindowName("Real-Time Plot");
//        contextView_->GetRenderWindow()->SetSize(400, 300);
//        contextView_->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
//        contextView_->GetScene()->AddItem(chart_);
//        contextView_->GetRenderWindow()->Render();
//
//        // Start the timer to update the plot at a regular interval
//        vtkSmartPointer<vtkTimerCallback> timerCallback = vtkSmartPointer<vtkTimerCallback>::New();
//        timerCallback->SetTable(table_);
//        timerCallback->SetChart(chart_);
//        timerCallback->SetContextView(contextView_);
//        vtkSmartPointer<vtkTimerLog> timer = vtkSmartPointer<vtkTimerLog>::New();
//        timer->InsertTimedEvent("plot",1,1000);
//        timer->StartTimer();
//        timerCallback->SetTimer(timer);
//        contextView_->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::TimerEvent, timerCallback);
//        contextView_->GetRenderWindow()->GetInteractor()->CreateRepeatingTimer(50);
//    }
//
//private:
//    vtkSmartPointer<vtkTable> table_;
//    vtkSmartPointer<vtkChartXY> chart_;
//    vtkSmartPointer<vtkContextView> contextView_;
//
//    class vtkTimerCallback : public vtkCommand {
//    public:
//        static vtkTimerCallback* New() {
//            return new vtkTimerCallback;
//        }
//
//        void SetTable(vtkSmartPointer<vtkTable> table) {
//            table_ = table;
//        }
//
//        void SetChart(vtkSmartPointer<vtkChartXY> chart) {
//            chart_ = chart;
//        }
//
//        void SetContextView(vtkSmartPointer<vtkContextView> contextView) {
//            contextView_ = contextView;
//        }
//
//        void SetTimer(vtkSmartPointer<vtkTimerLog> timer) {
//            timer_ = timer;
//        }
//
//        void Execute(vtkObject* caller, unsigned long eventId, void* vtkNotUsed(callData)) override {
//            if (vtkCommand::TimerEvent == eventId) {
//                // Generate a new data point for the signal
//                static int x = 0;
//                double y = std::sin(M_PI / 50 * x) * 100 + vtkMath::Random(-20, 20);
//                x++;
//
//                // Add the new data point to the table
//                //table_->GetColumnByName("X")
//                vtkSmartPointer<vtkDoubleArray> xArray = vtkSmartPointer<vtkDoubleArray>::New();
//                vtkSmartPointer<vtkDoubleArray> yArray = vtkSmartPointer<vtkDoubleArray>::New();
//                xArray->InsertNextValue(x);
//                yArray->InsertNextValue(y);
//
//                // Remove old data points from the table
//                if (table_->GetNumberOfRows() > 1000) {
//                    table_->RemoveRow(0);
//                }
//
//                // Update the chart's axis ranges to fit the new data
//                chart_->GetAxis(vtkAxis::BOTTOM)->SetRange(x - 1000, x);
//                chart_->GetAxis(vtkAxis::LEFT)->SetRange(-120, 120);
//
//                // Render the plot
//                contextView_->GetRenderWindow()->Render();
//
//                // Print the frame rate
//                double fps = 1000.0 / timer_->GetElapsedTime();
//                std::cout << "FPS: " << fps << std::endl;
//                //timer_->Reset();
//                timer_->StartTimer();
//            }
//        }
//
//    private:
//        vtkSmartPointer<vtkTable> table_;
//        vtkSmartPointer<vtkChartXY> chart_;
//        vtkSmartPointer<vtkContextView> contextView_;
//        vtkSmartPointer<vtkTimerLog> timer_;
//    };
//};
//
//int main(int argc, char* argv[]) {
//    //RealTimePlot realTimePlotContextView_;
//    //realTimePlotContextView_.GetRenderWindow()->GetInteractor()->Start();
//    Execute
//    return 0;
//}

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

#if VTK_VERSION_NUMBER >= 90220220630ULL
#define VTK_HAS_SETCOLORF 1
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


void receiveSignal2(std::vector<double>& signal_data, std::mutex& signal_mutex2, std::condition_variable& signal_cv2) {
    while (true) {
        // Simulate receiving the signal data from a UDP or TCP socket
        // Here, we just fill the vector with random values
        for (int i = 0; i < signal_data.size(); i++) {
            signal_data[i] = rand() % 85;
        }

        // Create a signal with the vector as data
        Signal signal(signal_data);

        // Lock the mutex and update the signal data
        {
            std::unique_lock<std::mutex> lock(signal_mutex2);
            signal_data = signal.data();
        }

        // Notify the condition variable that new signal data is available
        signal_cv2.notify_one();

        // Pause for 1ms
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void plotSignalall(std::vector<double>& signal_data, std::mutex& signal_mutex, std::condition_variable& signal_cv, std::vector<double>& signal_data2, std::mutex& signal_mutex2, std::condition_variable& signal_cv2) {
    // Set up the view
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkSmartPointer<vtkContextView> view1 = vtkSmartPointer<vtkContextView>::New();
    view1->GetRenderWindow()->SetWindowName("PRF Plot");
    view1->GetRenderWindow()->SetSize(700, 500);
    view1->GetRenderer()->SetBackground(colors->GetColor3d("slate_grey_light").GetData());

    vtkSmartPointer<vtkTable> table1 = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkFloatArray> arrX1 = vtkSmartPointer<vtkFloatArray>::New();
    arrX1->SetName("X Axis");
    table1->AddColumn(arrX1);

    vtkSmartPointer<vtkFloatArray> arrC1 = vtkSmartPointer<vtkFloatArray>::New();
    arrC1->SetName("PRF");
    table1->AddColumn(arrC1);

    vtkNew<vtkFloatArray> arrS;
    arrS->SetName("PRF2");
    table1->AddColumn(arrS);

    vtkSmartPointer<vtkChartXY> chart1 = vtkSmartPointer<vtkChartXY>::New();
    chart1->SetTitle("Area Plot");
    //chart1->GetTitleProperties()->SetFontSize(36);
    //chart1->GetTitleProperties()->SetColor(colors->GetColor3d("Banana").GetData());
    view1->GetScene()->AddItem(chart1);


    // Set the desired update rate to 10 Hz (1 ms)
    const double update_rate = 1000.0;    const std::chrono::duration<double> update_interval{ 1.2 / update_rate };
    // Initialize the last update time
    auto last_update_time = std::chrono::steady_clock::now();

    // Render the view new nmikhad
    //view->GetRenderWindow()->Render();

    while (true) {

        ////High performancing
        chart1->RemovePlot(1);
        chart1->RemovePlot(2);
        view1->RemoveAllObservers();
        view1->RemoveAllRepresentations();
        table1->RemoveAllObservers();

        // Wait for the remaining time until the next update is due
        auto elapsed_time = std::chrono::steady_clock::now() - last_update_time;
        auto remaining_time = update_interval - elapsed_time;
        std::this_thread::sleep_for(remaining_time);
        // Update the last update time
        last_update_time = std::chrono::steady_clock::now();
        // Lock the mutex and wait for new signal data
        std::unique_lock<std::mutex> lock(signal_mutex);
        signal_cv.wait(lock);
        // Get the signal data
        std::vector<double> data = signal_data;
        std::vector<double> data2 = signal_data2;
        // Update the table with the new signal data
        int numPoints1 = data.size();
        table1->SetNumberOfRows(numPoints1);
        for (int i = 0; i < numPoints1; ++i) {
            table1->SetValue(i, 0, i);
            table1->SetValue(i, 1, data[i]);
            table1->SetValue(i, 2, data2[i]);
        }

        //// Add multiple line plots, setting the colors etc.
        vtkSmartPointer<vtkPlot> line1 = chart1->AddPlot(vtkChart::LINE);
        line1->SetInputData(table1, 0, 1);
        line1->SetColor(110, 110, 235, 255);
        line1->SetWidth(.7);

        line1 = chart1->AddPlot(vtkChart::LINE);
        line1->SetInputData(table1, 0, 2);
        line1->SetColor(255, 0, 0, 255);
        line1->SetWidth(.7);
        // Update the plot with the new data
        chart1->Update();
        // Render the view
        view1->GetRenderWindow()->Render();

    }
    chart1->Delete();
    table1->Delete();
    view1->Delete();

    view1->GetInteractor()->Start();
    // Initialize and start the interactor
    view1->GetInteractor()->Initialize();
}


void plotSignal(std::vector<double>& signal_data, std::mutex& signal_mutex, std::condition_variable& signal_cv) {
    // Set up the view
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
    //vtkSmartPointer<vtkContextView> view2 = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderWindow()->SetWindowName("PRF Plot");
    view->GetRenderWindow()->SetSize(1000, 1000);
    view->GetRenderer()->SetBackground(colors->GetColor3d("SlateGray").GetData());

    vtkNew<vtkChartMatrix> matrix;
    view->GetScene()->AddItem(matrix);
    matrix->SetSize(vtkVector2i(2, 2));
    matrix->SetGutter(vtkVector2f(50.0, 50.0));

    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkFloatArray> arrX1 = vtkSmartPointer<vtkFloatArray>::New();
    arrX1->SetName("X Axis");
    table->AddColumn(arrX1);

    vtkSmartPointer<vtkFloatArray> arrC1 = vtkSmartPointer<vtkFloatArray>::New();
    arrC1->SetName("PRF");
    table->AddColumn(arrC1);

    vtkSmartPointer<vtkFloatArray> arrX2 = vtkSmartPointer<vtkFloatArray>::New();
    arrX2->SetName("X Axis");
    table->AddColumn(arrX2);

    vtkSmartPointer<vtkFloatArray> arrC2 = vtkSmartPointer<vtkFloatArray>::New();
    arrC2->SetName("PRF");
    table->AddColumn(arrC2);

    vtkSmartPointer<vtkFloatArray> arrX3 = vtkSmartPointer<vtkFloatArray>::New();
    arrX3->SetName("X Axis");
    table->AddColumn(arrX3);

    vtkSmartPointer<vtkFloatArray> arrC3 = vtkSmartPointer<vtkFloatArray>::New();
    arrC3->SetName("PRF");
    table->AddColumn(arrC3);

    vtkSmartPointer<vtkFloatArray> arrX4 = vtkSmartPointer<vtkFloatArray>::New();
    arrX4->SetName("X Axis");
    table->AddColumn(arrX4);

    vtkSmartPointer<vtkFloatArray> arrC4 = vtkSmartPointer<vtkFloatArray>::New();
    arrC4->SetName("PRF");
    table->AddColumn(arrC4);

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

    // Add interactor style for zooming
    vtkSmartPointer<vtkInteractorStyleRubberBandZoom> style = vtkSmartPointer<vtkInteractorStyleRubberBandZoom>::New();
    view->GetInteractor()->SetInteractorStyle(style);

    // Set the desired update rate to 10 Hz (1 ms)
    const double update_rate = 1000.0;    const std::chrono::duration<double> update_interval{ 1.2 / update_rate };
    // Initialize the last update time
    auto last_update_time = std::chrono::steady_clock::now();

    while (true) {

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

        // Wait for the remaining time until the next update is due
        auto elapsed_time = std::chrono::steady_clock::now() - last_update_time;
        auto remaining_time = update_interval - elapsed_time;
        std::this_thread::sleep_for(remaining_time);

        // Update the last update time
        last_update_time = std::chrono::steady_clock::now();

        // Lock the mutex and wait for new signal data
        std::unique_lock<std::mutex> lock(signal_mutex);
        signal_cv.wait(lock);

        // Get the signal data
        std::vector<double> data = signal_data;

        int numPoints = data.size();
        table->SetNumberOfRows(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            table->SetValue(i, 0, i);
            table->SetValue(i, 1, data[i]);
            //table->SetValue(i, 1, data[i]);
            //table->SetValue(i, 1, data[i]);
            //table->SetValue(i, 1, data[i]);
        }

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

        //// Add interactor style for zooming
        //vtkSmartPointer<vtkChartInteractor> interactor = vtkSmartPointer<vtkChartInteractor>::New();
        //interactor->SetChart(chart1);
        //view->GetRenderWindow()->GetInteractor()->SetInteractorStyle(interactor);
        //// Set up zooming on mouse wheel events
        //interactor->AddObserver(vtkCommand::MouseWheelForwardEvent, chart1, &vtkChartXY::ZoomIn);
        //interactor->AddObserver(vtkCommand::MouseWheelBackwardEvent, chart1, &vtkChartXY::ZoomOut);

        // Update the plot with the new data
        chart1->Update();
        chart2->Update();
        chart3->Update();
        chart4->Update();

        view->GetRenderWindow()->Render();
    }

    table->Delete();
    view->Delete();

    view->GetInteractor()->Initialize();
    view->GetInteractor()->Start();
}


int main() {
    // Create a vector with 2600 elements
    std::vector<double> signal_data(2600);
    std::vector<double> signal_data2(2600);
    // Create a mutex and condition variable for synchronizing the signal data between threads
    std::mutex signal_mutex;
    std::condition_variable signal_cv;
    std::mutex signal_mutex2;
    std::condition_variable signal_cv2;

    // Create a thread for receiving signals
    std::thread receive_thread(receiveSignal, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv));
    std::thread receive_thread2(receiveSignal2, std::ref(signal_data2), std::ref(signal_mutex2), std::ref(signal_cv2));

    std::thread plot_threadall(plotSignalall, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv), std::ref(signal_data2), std::ref(signal_mutex2), std::ref(signal_cv2));

    // Create a thread for plotting signals
    std::thread plot_thread4in1(plotSignal, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv));

    // Wait for both threads to finish
    receive_thread.join();
    plot_threadall.join();
    plot_thread4in1.join();

    return 0;
}
