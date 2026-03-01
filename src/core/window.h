#pragma once
#include "camera.h"
#include <memory>
#include <GLFW/glfw3.h>

class Window {
    GLFWwindow* window;
    std::unique_ptr<Camera> camera;
    // camera pos
    float last_x;
    float last_y;
    // time
    float delta_time;
    float last_frame;

  public:
    Window();
    ~Window();
    bool init();
    void end_frame();
    void time_check();
    bool should_close();

    float get_delta() {
        return delta_time;
    }

    Camera* get_camera() {
        return camera.get();
    }

    // callbacks
    void process_resize(GLFWwindow* window, int width, int height);
    void process_input();
    void process_mouse(GLFWwindow* window, double xposIn, double yposIn);
    void process_scroll(GLFWwindow* window, double xoffset, double yoffset);
};