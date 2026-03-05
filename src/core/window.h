#pragma once
#include "camera.h"
#include <memory>
#include <GLFW/glfw3.h>
#include "common.h"



class Window {
    GLFWwindow* window;
    std::unique_ptr<Camera> camera;
    int width = SCR_WIDTH;
    int height = SCR_HEIGHT;
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
    void begin_frame();
    void end_frame();
    void time_check();
    bool should_close();

    float get_delta() {
        return delta_time;
    }

    Camera* get_camera() {
        return camera.get();
    }
    float aspect_ratio() const { return static_cast<float>(width) / static_cast<float>(height); }

    // callbacks
    void process_resize(GLFWwindow* window, int width, int height);
    void process_input();
    void process_mouse(GLFWwindow* window, double xposIn, double yposIn);
    void process_scroll(GLFWwindow* window, double xoffset, double yoffset);
};