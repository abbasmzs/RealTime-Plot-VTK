#include "plotter.hpp"

#include <vector>
#include <chrono>
#include <cmath>


class ExampleVTKRealtimePlotter : public VTKRealtimePlotter<vtkFloatArray, float>
{
    void pre_render() override
    {
        vtkAxis* x_axis = chart_->GetAxis(1);
        vtkAxis* y_axis = chart_->GetAxis(0);
        x_axis->SetRange(0, 7.5);
        y_axis->SetRange(-1, 1);
    }

public:
    ExampleVTKRealtimePlotter(unsigned long timer_interval = 1): VTKRealtimePlotter(timer_interval)
    {
        add_columns({"X", "Sine", "Cosine"});
        
        view_->GetRenderer()->SetBackground(0.5, 0.5, 0.5);
        
        vtkPlot *line = add_plot(vtkChart::LINE, 0, 1);
        line->SetColor(0, 255, 0, 255);
        line->SetWidth(2.5);
        
        line = add_plot(vtkChart::LINE, 0, 2);
        line->SetColor(255, 0, 0, 255);
        line->SetWidth(5.0);

        line->GetPen()->SetLineType(vtkPen::DASH_LINE);

        view_->GetRenderWindow()->SetMultiSamples(4);
        view_->GetRenderWindow()->SetFullScreen(true);
        vtkAxis* x_axis = chart_->GetAxis(1);
        vtkAxis* y_axis = chart_->GetAxis(0);
        //x_axis->SetTitle("angle");
        //y_axis->SetTitle("sin / cos");
    }
};

//int main(int, char *[])
//{
//    ExampleVTKRealtimePlotter plotter;
//    plotter.start();
//
//    constexpr size_t target_num_points = 5000;
//    const float increment = 7.5 / (target_num_points - 1);
//
//    for (size_t i = 0; i <= target_num_points; ++i)
//    {
//        std::this_thread::sleep_for(0.001s);
//        const float x = i * increment;
//        plotter.insert_values({x, sin(x), cos(x)});
//    }
//
//    return 0;
//}
//


#include "plotter.hpp"
#include <random>

class RandomSignalGenerator {
public:
    RandomSignalGenerator(size_t num_samples, float amplitude, float frequency) : num_samples_(num_samples), amplitude_(amplitude), frequency_(frequency), rng_(std::random_device{}()) {}

    std::vector<float> generate_signal() {
        std::vector<float> signal;
        signal.reserve(num_samples_);

        for (size_t i = 0; i < num_samples_; ++i) {
            signal.push_back(amplitude_ * std::sin(2 * 3.14 * frequency_ * i / num_samples_) + std::normal_distribution<float>(0, 0.1)(rng_));
        }

        return signal;
    }

private:
    size_t num_samples_;
    float amplitude_;
    float frequency_;
    std::mt19937 rng_;
};

int main(int, char* [])
{
    ExampleVTKRealtimePlotter plotter;
    plotter.start();

    RandomSignalGenerator signal_generator(1000, 0.5, 10);

    while (true)
    {
        std::this_thread::sleep_for(100ms);
        const float x = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;

        std::vector<float> signal = signal_generator.generate_signal();
        plotter.insert_values({ x, signal[0], signal[1] });
    }

    return 0;
}