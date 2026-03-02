#pragma once

#include "renderer/mesh.h"
#include <memory>
#include "renderer/renderer.h"

class System {
    std::unique_ptr<Mesh> planet_mesh;
    Renderer* renderer;

  public:
    void init(Renderer* _renderer);
    void simulate(float dt);
};