#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <mutex>
#include <vector>
#include <chrono>
#include <array>


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow* ui;
	bool mChartVisible = false; // Declare mChartVisible and initialize to false

public:
	explicit MainWindow(QWidget* parent = nullptr);
	virtual ~MainWindow();

public slots:
	void onDrawSphereClick();
	void onDrawSphereClick2();

};

#endif // MAINWINDOW_H
