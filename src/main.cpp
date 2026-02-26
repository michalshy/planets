#include "core/shader.h"

#include <GLFW/glfw3.h>
#include <print>

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

int main() {
    // glfw
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(640, 480, "Planets", nullptr, nullptr);
    if (window == NULL) {
        std::println("Window creation failed!");
        return 1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // shaders
    Shader sh("shaders/base.vert", "shaders/base.frag");
    // sphere

    while (!glfwWindowShouldClose(window)) {
        // render(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}