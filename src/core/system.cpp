#include "system.h"

#include "geometry.h"
#include "transform.h"

constexpr int STANDARD_STACKS = 20;
constexpr int STANDARD_SLICES = 20;

enum class Type {
    Planet,
    Star,
};

struct Celestial {
    Transform transform;
    Type type;
    glm::vec3 velocity{0.0f};
    glm::vec3 acc{0.0f};
    float mass;
    float radius;
    glm::vec3 color{0.3f, 0.3f, 0.0f};
    std::unique_ptr<Mesh> msh;

    Celestial(Transform _transform, Type _type, float _mass, float _radius)
        : transform(_transform), type(_type), mass(_mass), radius(_radius) {
        auto [vertices, indices] =
            generate_sphere(_radius, _radius * STANDARD_STACKS, _radius * STANDARD_SLICES);
        msh = std::make_unique<Mesh>(vertices, indices);
    }

    void draw() {
        msh->draw();
    }
};

static std::vector<Celestial> bodies;

void System::init(Renderer* _renderer) {
    renderer = _renderer;

    Celestial simple = Celestial(Transform(), Type::Planet, 10.0f, 1.0f);
    bodies.push_back(std::move(simple));
}

void System::simulate(float dt) {
    for (const auto& body : bodies) {
        switch (body.type) {
        case Type::Planet:
            renderer->draw_lit(*body.msh, body.transform, body.color);
            break;
        case Type::Star:
            renderer->draw_unlit(*body.msh, body.transform, body.color);
            break;
        default:
            break;
        }
    }
}