#ifndef ELEVATORLABWINDOW_H
#define ELEVATORLABWINDOW_H

#include <QMainWindow>

namespace Ui {
class ElevatorLabWindow;
}

class ElevatorLabWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ElevatorLabWindow(QWidget *parent = 0);
    ~ElevatorLabWindow();
    
private:
    Ui::ElevatorLabWindow *ui;

    /**
      Get the error in velocity that the system has accumulated at a given timestamp

      This is calculated with the quadratic function error = (-8.0797E-5) * time^2 - 0.0391 * time + 0.0368

      @param time The time in seconds after the start of the experiment to get the error for
      @return The current estimated velocity error in meters/second^2. This will be negative.
      */
    static double getError(double time);
};

#endif // ELEVATORLABWINDOW_H
