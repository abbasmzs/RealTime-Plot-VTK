#pragma once

#include <thread>
#include <condition_variable>
#include <assert.h>

//#include <stop_token.hpp>

#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkCallbackCommand.h>
#include <vtkAxis.h>
#include <vtkAutoInit.h> 

using namespace std::chrono_literals;

template <typename vtkArrayType = vtkFloatArray, typename Rep = float>
class VTKRealtimePlotter
{
    using rep = Rep;
    
    vtkSmartPointer<vtkTable> table_;
    std::thread view_handler_;
    std::mutex view_handler_mtx_;
    std::condition_variable view_handler_cv_;
    bool started_;
    std::stop_source stop_source_;
    std::stop_token view_handler_stop_token_;
    unsigned long update_interval_;
    int view_handler_timer_id_;

protected:
    vtkSmartPointer<vtkContextView> view_;
    vtkSmartPointer<vtkChartXY> chart_;

private:
    static void update(vtkObject*, unsigned long eid, void* clientdata, void* calldata)
    {
        VTKRealtimePlotter* plotter = static_cast<VTKRealtimePlotter*>(clientdata);
        plotter->pre_render();
        plotter->view_->Render();
    }

    void start_view_handler()
    {
        std::unique_lock<std::mutex> lk(view_handler_mtx_);
        view_handler_cv_.wait(lk, [this]{ return started_; });

        view_->GetInteractor()->Initialize();
        view_->Render();

        auto update_callback = vtkSmartPointer<vtkCallbackCommand>::New();
        update_callback->SetCallback(update);
        update_callback->SetClientData(this);
        
        auto iren = view_->GetInteractor();
        iren->AddObserver("TimerEvent", update_callback);
        view_handler_timer_id_ = iren->CreateRepeatingTimer(update_interval_);
        std::stop_callback cb(view_handler_stop_token_, [this, iren]() { iren->DestroyTimer(view_handler_timer_id_); });
        iren->Start();
    }

    vtkArrayType* get_column(size_t idx) { return static_cast<vtkArrayType*>(table_->GetColumn(idx)); }
    vtkArrayType* get_column(const std::string& name) { return static_cast<vtkArrayType*>(table_->GetColumnByName(name.c_str())); }

protected:
    VTKRealtimePlotter(unsigned long update_interval = 1):
        table_(vtkSmartPointer<vtkTable>::New()),
        view_(vtkSmartPointer<vtkContextView>::New()),
        chart_(vtkSmartPointer<vtkChartXY>::New()),
        started_(false),
        view_handler_stop_token_(stop_source_.get_token()),
        update_interval_(update_interval),
        view_handler_(&VTKRealtimePlotter::start_view_handler, this)
    {
        view_->GetScene()->AddItem(chart_);
    }

    VTKRealtimePlotter(VTKRealtimePlotter&) = delete;

    ~VTKRealtimePlotter()
    {
        stop();
        view_handler_.join();
    }

    size_t add_column()
    {
        auto column = vtkSmartPointer<vtkArrayType>::New();
        table_->AddColumn(column);
        return table_->GetNumberOfColumns();
    }

    size_t add_column(const std::string& name)
    {
        auto column = vtkSmartPointer<vtkArrayType>::New();
        column->SetName(name.c_str());
        table_->AddColumn(column);
        return table_->GetNumberOfColumns();
    }

    void add_columns(const std::initializer_list<std::string> names)
    {
        for (const std::string& name : names) { add_column(name); }
    }

    vtkPlot* add_plot(int type, size_t x_column_idx, size_t y_column_idx)
    {
        vtkPlot *line = chart_->AddPlot(type);
        line->SetInputData(table_, x_column_idx, y_column_idx);
        return line;
    }

public:
    void insert_values(const std::vector<rep>& values)
    {
        assert(values.size() == table_->GetNumberOfColumns());
        for (size_t column_idx = 0; column_idx < table_->GetNumberOfColumns(); ++column_idx)
        {
            const rep& value = values[column_idx];
            auto column = get_column(column_idx);
            column->InsertNextValue(value);
        }
        table_->Modified();
    }
private:
    virtual void pre_render() {}

public:
    void start()
    {
        {
            std::lock_guard<std::mutex> lk(view_handler_mtx_);
            started_ = true;
        }
        view_handler_cv_.notify_one();
    }

    void stop()
    {
        stop_source_.request_stop();
    }
};