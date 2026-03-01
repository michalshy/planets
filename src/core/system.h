#pragma once

#include "mesh.h"
#include <memory>
#include "renderer.h"

class System {
    std::unique_ptr<Mesh> planet_mesh;
    Renderer* renderer;

  public:
    void init(Renderer* _renderer);
    void simulate(float dt);
};