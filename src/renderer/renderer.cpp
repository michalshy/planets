#include "renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "core/common.h"

Renderer::Renderer()
    : lit_shader(std::make_unique<Shader>("shaders/lit.vert", "shaders/lit.frag")),
      unlit_shader(std::make_unique<Shader>("shaders/unlit.vert", "shaders/unlit.frag")) {
}

Renderer::~Renderer() {
    // nothing
}

bool Renderer::init() {
    glEnable(GL_DEPTH_TEST);
    return true;
}

void Renderer::begin_frame(Camera* camera, Window* window) {
    lit_shader->use();
    lit_shader->set_vec3("viewPos", camera->get_position());
    lit_shader->set_vec3("lightColor", 1.0f, 1.0f, 1.0f);

    glm::mat4 view       = camera->get_view_mat();
    glm::mat4 projection = glm::mat4(1.0f);
    projection           = glm::perspective(glm::radians(45.0f),
                                            window->aspect_ratio(),
                                            0.1f, 100000.0f);
    lit_shader->set_mat4("projection", projection);
    lit_shader->set_mat4("view", view);

    unlit_shader->use();
    unlit_shader->set_mat4("projection", projection);
    unlit_shader->set_mat4("view", view);
}

void Renderer::draw_lit(const Mesh& mesh, Transform transform, glm::vec3 color) {
    lit_shader->use();
    lit_shader->set_vec3("objectColor", color);

    lit_shader->set_mat4("model", transform.to_mat4());

    mesh.draw();
}

void Renderer::draw_unlit(const Mesh& mesh, Transform transform, glm::vec3 color) {
    unlit_shader->use();

    glm::mat4 model = glm::mat4(1.0f);
    unlit_shader->set_mat4("model", transform.to_mat4());

    mesh.draw();
}

void Renderer::draw_grid(const Mesh& mesh, Transform transform, glm::vec3 color) {
    
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}