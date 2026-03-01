#pragma once

#include <glm/glm.hpp>
#include "core/mesh.h"
#include "shader.h"
#include <memory>
#include "camera.h"
#include <glm/gtc/quaternion.hpp>

enum class Shape { Cube, Sphere };

struct Transform {
    glm::vec3 pos{0.0f, 0.0f, 0.0f};
    glm::quat rot{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    glm::mat4 to_mat4() {
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model *= glm::mat4_cast(rot);
        model = glm::scale(model, scale);

        return model;
    }
};

class Renderer {
  public:
    Renderer();
    ~Renderer();

    bool init();
    void begin_frame(Camera* camera);
    void draw_lit(Shape shape, Transform transform, glm::vec3 color);
    void draw_unlit(Shape shape, Transform transform, glm::vec3 color);
    void add_light(glm::vec3 pos);
    void end_frame();
    void clear();

  private:
    std::unique_ptr<Shader> lit_shader;
    std::unique_ptr<Shader> unlit_shader;

    std::unique_ptr<Mesh> cube_mesh;
};