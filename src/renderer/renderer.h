#pragma once

#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"
#include <memory>
#include "core/camera.h"
#include <glm/gtc/quaternion.hpp>
#include "core/transform.h"

class Renderer {
  public:
    Renderer();
    ~Renderer();

    bool init();
    void begin_frame(Camera* camera);
    void draw_lit(const Mesh& mesh, Transform transform, glm::vec3 color);
    void draw_unlit(const Mesh& mesh, Transform transform, glm::vec3 color);
    void draw_grid(const Mesh& mesh, Transform transform, glm::vec3 color);
    void add_light(glm::vec3 pos);
    void end_frame();
    void clear();

  private:
    std::unique_ptr<Shader> lit_shader;
    std::unique_ptr<Shader> unlit_shader;
    std::unique_ptr<Shader> grid_shader;
};