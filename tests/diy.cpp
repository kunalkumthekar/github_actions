#include <memory>
#include <atomic>
#include <thread>
#include <string>
#include <chrono>
#include <iostream>

// SlicerController.cpp

class Slicer {
public:
    Slicer(bool &gaurd_open, int &deadline) : is_guard_open(gaurd_open), deadline_missing(deadline) {
        monitoring_thread = std::thread([this] {
            if (!is_guard_open) {
                std::this_thread::sleep_for(std::chrono::microseconds(deadline_missing));
                // perform tasks
                set_motor_pwm(blade_speed);
            }
        });
    }

    ~Slicer() {
        is_guard_open = true;
        stop_blade_immediately();
        if (monitoring_thread.joinable()) monitoring_thread.detach();
    }
    float blade_speed = 0.0;

private:
    int deadline_missing;
    std::thread monitoring_thread;
    std::atomic<bool> is_guard_open;
    void stop_blade_immediately() { /* Low-level hardware stop */ }
    void set_motor_pwm(float speed) { /* Sets PWM registers */ }
};

// int main() {
//     bool gaurd_open = true;
//     int deadline = 10;
    
// }