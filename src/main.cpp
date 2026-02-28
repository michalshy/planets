#include "core/shader.h"
#include "core/camera.h"

#include <GLFW/glfw3.h>
#include <print>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);

Camera camera    = Camera();
float last_x     = SCR_WIDTH / 2.0f;
float last_y     = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

glm::vec3 light_pos{1.2f, 1.0f, 2.0f};

int main() {
    // glfw
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planets", nullptr, nullptr);
    if (window == NULL) {
        std::println("Window creation failed!");
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("GL loading failed!");
        return 1;
    }
    glEnable(GL_DEPTH_TEST);
    // shaders
    Shader base_sh("shaders/base.vert", "shaders/base.frag");
    Shader light_sh("shaders/light.vert", "shaders/light.frag");
    // spheres
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    // lights
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (reinterpret_cast<void *>(0)));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time          = current_frame - last_frame;
        last_frame          = current_frame;

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        base_sh.use();
        base_sh.set_vec3("objectColor", 1.0f, 0.5f, 0.31f);
        base_sh.set_vec3("lightColor", 1.0f, 1.0f, 1.0f);

        glm::mat4 view       = camera.get_view_mat();
        glm::mat4 projection = glm::mat4(1.0f);
        projection           = glm::perspective(
            glm::radians(45.0f), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
            0.1f, 100.0f);
        base_sh.set_mat4("projection", projection);
        base_sh.set_mat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        base_sh.set_mat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        light_sh.use();
        light_sh.set_mat4("projection", projection);
        light_sh.set_mat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_pos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        light_sh.set_mat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(CAM_MOVEMENT::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(CAM_MOVEMENT::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(CAM_MOVEMENT::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(CAM_MOVEMENT::RIGHT, delta_time);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// Had to rewrite a little bit due to non-working wsl input capture from mouse...
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
        first_mouse = true;
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse) {
        last_x      = xpos;
        last_y      = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;

    last_x = xpos;
    last_y = ypos;

    camera.process_mouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.process_scroll(static_cast<float>(yoffset));
}