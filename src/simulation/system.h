#pragma once

#include "renderer/mesh.h"
#include <memory>
#include "renderer/renderer.h"

class System {
    std::unique_ptr<Mesh> grid_mesh;
    Renderer* renderer;

  public:
    void init(Renderer* _renderer);
    void simulate(float dt);

  private:
    float time_multiplier{1.0f};
    float vis_scale{0.5f};

    void render_sliders();
    void render_planets();
};