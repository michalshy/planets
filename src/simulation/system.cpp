#include "system.h"

#include "core/geometry.h"
#include "core/transform.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "core/common.h"
#include <print>

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

    Celestial(Transform _transform, Type _type, float _mass, float _radius, glm::vec3 _color)
        : transform(_transform), type(_type), mass(_mass), radius(_radius), color(_color) {
        auto [vertices, indices] =
            generate_sphere(_radius, STANDARD_STACKS, STANDARD_SLICES);
        msh = std::make_unique<Mesh>(vertices, indices);
    }

    void draw() {
        msh->draw();
    }

    void calculate_forces(Celestial& body) {
        glm::vec3 dir = body.transform.pos - transform.pos;
        float distance = glm::length(dir);
        dir = glm::normalize(dir);

        float magnitude = mass * body.mass * G / (distance * distance); 
        glm::vec3 force = dir * magnitude;

        acc += force / mass;
        body.acc -= force / body.mass;
    }

    void update(float dt) {
        velocity += acc * dt;
        transform.pos += velocity * dt;
    }
};

static std::vector<Celestial> bodies;

void System::init(Renderer* _renderer) {
    renderer = _renderer;

    Celestial sun = Celestial(Transform{glm::vec3(-200.0f, 0.0f, 0.0f)}, Type::Planet, 333000.0f, 30.0f, {1.0f, 1.0f, 0.0f});
    Celestial mercury = Celestial(Transform{glm::vec3(-142.1f, 0.0f, 0.0f)}, Type::Planet, 0.0553f, 8.0f, {0.55f, 0.50f, 0.48f});
    Celestial venus = Celestial(Transform{glm::vec3(-91.8f, 0.0f, 0.0f)}, Type::Planet, 0.815f, 12.0f, {1.0f, 0.5f, 0.0f});
    Celestial earth = Celestial(Transform{glm::vec3(-50.4f, 0.0f, 0.0f)}, Type::Planet, 1.0f, 12.0f, {0.0f, 0.75f, 0.0f});
    Celestial mars = Celestial(Transform{glm::vec3(27.9f, 0.0f, 0.0f)}, Type::Planet, 0.107f, 8.0f, {1.0f, 0.0f, 0.0f});
    Celestial jupiter = Celestial(Transform{glm::vec3(578.5f, 0.0f, 0.0f)}, Type::Planet, 317.8f, 200.0f, {0.76f, 0.60f, 0.42f});
    Celestial saturn = Celestial(Transform{glm::vec3(1226.7f, 0.0f, 0.0f)}, Type::Planet, 95.2f, 180.0f, {0.87f, 0.78f, 0.57f});
    Celestial uran = Celestial(Transform{glm::vec3(2671.0f, 0.0f, 0.0f)}, Type::Planet, 14.5f, 100.0f, {0.56f, 0.84f, 0.86f});
    Celestial neptun = Celestial(Transform{glm::vec3(4298.3f, 0.0f, 0.0f)}, Type::Planet, 17.1f, 100.0f, {0.25f, 0.41f, 0.88f});

    auto orbital_velocity = [&](Celestial& planet) {
        float r = glm::length(planet.transform.pos - sun.transform.pos);
        planet.velocity.z = sqrt(G * sun.mass / r);
    };

    orbital_velocity(mercury);
    orbital_velocity(venus);
    orbital_velocity(earth);
    orbital_velocity(mars);
    orbital_velocity(jupiter);
    orbital_velocity(saturn);
    orbital_velocity(uran);
    orbital_velocity(neptun);

    bodies.push_back(std::move(sun));
    bodies.push_back(std::move(mercury));
    bodies.push_back(std::move(venus));
    bodies.push_back(std::move(earth));
    bodies.push_back(std::move(mars));
    bodies.push_back(std::move(jupiter));
    bodies.push_back(std::move(saturn));
    bodies.push_back(std::move(uran));
    bodies.push_back(std::move(neptun));

}

void System::simulate(float dt) {
    for (auto& body : bodies) {
        body.acc = glm::vec3{0.0f};
    }

    for (int i = 0; i < bodies.size(); ++i) {
        for (int j = i + 1; j < bodies.size(); ++j) {
            bodies[i].calculate_forces(bodies[j]);    
        }
    }   

    for (auto& body : bodies) {
        body.update(dt);

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