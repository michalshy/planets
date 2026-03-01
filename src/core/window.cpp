#include "window.h"
#include <GLFW/glfw3.h>
#include <print>
#include "common.h"

static bool first_mouse = true;

static void mouse_callback(GLFWwindow* window, double x, double y) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->process_mouse(window, x, y);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->process_scroll(window, xoffset, yoffset);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->process_resize(window, width, height);
}

Window::Window()
    : last_x(SCR_WIDTH / 2.0f), last_y(SCR_HEIGHT / 2.0f), camera(std::make_unique<Camera>()) {
    glfwInit();
}

Window::~Window() {
    glfwDestroyWindow(window);
}

bool Window::init() {
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planets", nullptr, nullptr);
    if (window == NULL) {
        std::println("Window creation failed!");
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("GL loading failed!");
        return false;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    return true;
}

void Window::end_frame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::time_check() {
    const float current_frame = static_cast<float>(glfwGetTime());
    delta_time                = current_frame - last_frame;
    last_frame                = current_frame;
}

bool Window::should_close() {
    return glfwWindowShouldClose(window);
}

void Window::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->process_keyboard(CAM_MOVEMENT::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->process_keyboard(CAM_MOVEMENT::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->process_keyboard(CAM_MOVEMENT::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->process_keyboard(CAM_MOVEMENT::RIGHT, delta_time);
}

void Window::process_resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Had to rewrite a little bit due to non-working wsl input capture from mouse...
void Window::process_mouse(GLFWwindow* window, double xposIn, double yposIn) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
        first_mouse = true;
        return;
    }

    const float xpos = static_cast<float>(xposIn);
    const float ypos = static_cast<float>(yposIn);

    if (first_mouse) {
        last_x      = xpos;
        last_y      = ypos;
        first_mouse = false;
    }

    const float xoffset = xpos - last_x;
    const float yoffset = last_y - ypos;

    last_x = xpos;
    last_y = ypos;

    camera->process_mouse(xoffset, yoffset);
}

void Window::process_scroll(GLFWwindow* window, double xoffset, double yoffset) {
    camera->process_scroll(static_cast<float>(yoffset));
}