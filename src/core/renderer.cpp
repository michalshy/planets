#include "renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <print>
#include "common.h"

// spheres
constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

Renderer::Renderer()
    : lit_shader(std::make_unique<Shader>("shaders/base.vert", "shaders/base.frag")),
      unlit_shader(std::make_unique<Shader>("shaders/light.vert", "shaders/light.frag")) {
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &lit_vao);
    glDeleteVertexArrays(1, &unlit_vao);
    glDeleteBuffers(1, &vertex_buffer);
}

bool Renderer::init() {
    glEnable(GL_DEPTH_TEST);

    // objs
    glGenVertexArrays(1, &lit_vao);
    glGenBuffers(1, &vertex_buffer);

    glBindVertexArray(lit_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // lights
    glGenVertexArrays(1, &unlit_vao);
    glBindVertexArray(unlit_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (reinterpret_cast<void*>(0)));
    glEnableVertexAttribArray(0);

    return true;
}

void Renderer::begin_frame(Camera* camera) {
    lit_shader->use();
    lit_shader->set_vec3("viewPos", camera->get_position());
    lit_shader->set_vec3("lightColor", 1.0f, 1.0f, 1.0f);

    glm::mat4 view       = camera->get_view_mat();
    glm::mat4 projection = glm::mat4(1.0f);
    projection           = glm::perspective(glm::radians(45.0f),
                                            static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT),
                                            0.1f, 100.0f);
    lit_shader->set_mat4("projection", projection);
    lit_shader->set_mat4("view", view);

    unlit_shader->use();
    unlit_shader->set_mat4("projection", projection);
    unlit_shader->set_mat4("view", view);
}

void Renderer::draw_lit(Shape shape, Transform transform, glm::vec3 color) {
    lit_shader->use();
    lit_shader->set_vec3("objectColor", color);

    lit_shader->set_mat4("model", transform.to_mat4());

    glBindVertexArray(lit_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::draw_unlit(Shape shape, Transform transform, glm::vec3 color) {
    unlit_shader->use();
    // unlit_shader->set_vec3("objectColor", color);

    glm::mat4 model = glm::mat4(1.0f);
    unlit_shader->set_mat4("model", transform.to_mat4());

    glBindVertexArray(unlit_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::add_light(glm::vec3 pos) {
    // currently just one
    lit_shader->use();
    lit_shader->set_vec3("lightPos", pos);
}

void Renderer::end_frame() {
    // nothing to do!
}

void Renderer::clear() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}