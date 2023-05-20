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
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkContextView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkPlot.h>

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

///One Plot

void plotSignal(std::vector<double>& signal_data, std::mutex& signal_mutex, std::condition_variable& signal_cv) {
    // Set up the view
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderWindow()->SetWindowName("PRF Plot");
    view->GetRenderWindow()->SetSize(800, 800);
    view->GetRenderer()->SetBackground(colors->GetColor3d("slate_grey_light").GetData());
    //view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkFloatArray> arrX = vtkSmartPointer<vtkFloatArray>::New();
    arrX->SetName("X Axis");
    table->AddColumn(arrX);

    vtkSmartPointer<vtkFloatArray> arrC = vtkSmartPointer<vtkFloatArray>::New();
    arrC->SetName("PRF");
    table->AddColumn(arrC);

    vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem(chart);

    // Set the desired update rate to 10 Hz (1 ms)
    const double update_rate = 1000.0;    const std::chrono::duration<double> update_interval{ 1.0 / update_rate };
    // Initialize the last update time
    auto last_update_time = std::chrono::steady_clock::now();

    // Render the view new nmikhaaaad
    //view->GetRenderWindow()->Render();

    while (true) {
        
        chart->RemovePlot(1);
        //chart = vtkSmartPointer<vtkChartXY>::New();
       
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

        // Clear the table
        //table->RemoveAllRows();

        // Update the table with the new signal data
        int numPoints = data.size();
        table->SetNumberOfRows(numPoints);
        for (int i = 0; i < numPoints; ++i) {
            table->SetValue(i, 0, i);
            table->SetValue(i, 1, data[i]);
        }

        //table->InsertNextRow();

        //// Add multiple line plots, setting the colors etc.
        vtkSmartPointer<vtkPlot> line = chart->AddPlot(vtkChart::LINE);
        line->SetInputData(table, 0, 1);
        line->SetColor(3, 46, 202, 255);
        line->SetWidth(.8);

        // Update the plot with the new data
        chart->Update();

        // Render the view
        view->GetRenderWindow()->Render();
        
        //line->Delete();

    }

    chart->Delete();
    table->Delete();
    view->Delete();


    view->GetInteractor()->Start();
    // Initialize and start the interactor
    view->GetInteractor()->Initialize();
}

int main() {
    // Create a vector with 2600 elements
    std::vector<double> signal_data(2600);

    // Create a mutex and condition variable for synchronizing the signal data between threads
    std::mutex signal_mutex;
    std::condition_variable signal_cv;

    // Create a thread for receiving signals
    std::thread receive_thread(receiveSignal, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv));

    // Create a thread for plotting signals
    std::thread plot_thread(plotSignal, std::ref(signal_data), std::ref(signal_mutex), std::ref(signal_cv));


    // Wait for both threads to finish
    receive_thread.join();
    plot_thread.join();

    return 0;
}