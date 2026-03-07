#include "system.h"

#include "core/geometry.h"
#include "core/transform.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "core/common.h"
#include "imgui.h"
#include "renderer/mesh.h"
#include <cmath>
#include <cstdlib>
#include <string_view>

constexpr int STANDARD_STACKS = 20;
constexpr int STANDARD_SLICES = 20;

constexpr float TIME_MULT_MAX = 10.0f;
constexpr float TIME_MULT_MIN = 0.01f;

constexpr float VIS_SCALE_MAX = 1.0f;
constexpr float VIS_SCALE_MIN = 0.0001f;

constexpr glm::vec3 STARTING_GRID{0.0f, -250.0f, 0.0f};
constexpr float SOFTENING = 50.0f;

enum class Type {
    Planet,
    Star,
};

struct Celestial {
    std::string name;
    Transform transform;
    Type type;
    glm::vec3 velocity{0.0f};
    glm::vec3 momentum{0.0f};
    glm::vec3 acc{0.0f};
    float gamma{1.0f};
    float mass;
    float rest_mass;
    float radius;
    glm::vec3 color{0.3f, 0.3f, 0.0f};
    std::unique_ptr<Mesh> msh;
    float proper_time{0.0f};

    Celestial(std::string_view _name, Transform _transform, Type _type, float _mass, float _radius,
              glm::vec3 _color)
        : name(_name), transform(_transform), type(_type), mass(_mass), radius(_radius),
          color(_color) {
        auto [vertices, indices] = generate_sphere(_radius, STANDARD_STACKS, STANDARD_SLICES);
        msh       = std::make_unique<Mesh>(vertices, indices, 6, false, DRAW_MODE::TRIANGLES);
        rest_mass = mass;
    }

    void draw() {
        msh->draw();
    }

    void calculate_forces(Celestial& body) {
        glm::vec3 dir    = body.transform.pos - transform.pos;
        float distance   = glm::length(dir);
        dir              = glm::normalize(dir);
        float v_avg      = (glm::length(this->velocity) + glm::length(body.velocity)) / 2.0f;
        float correction = 1.0f + (3.0f * v_avg * v_avg) / (c * c) +
                           (2.0f * G * (mass + body.mass)) / (distance * c * c);

        float magnitude = rest_mass * body.rest_mass * G / (distance * distance) * correction;
        glm::vec3 force = dir * magnitude;

        acc += force / rest_mass;
        body.acc -= force / body.rest_mass;
    }

    void update(float dt) {
        momentum += acc * rest_mass * dt;
        float p2 = glm::dot(momentum, momentum);
        gamma    = sqrt(1.0f + p2 / (rest_mass * rest_mass * c * c));
        velocity = momentum / (gamma * rest_mass);
        transform.pos += velocity * dt;
        proper_time += dt / gamma;
    }
};

static std::vector<Celestial> bodies;
static std::vector<float> grid_vertices;

void System::init(Renderer* _renderer) {
    // planets
    bodies.clear();
    renderer = _renderer;

    Celestial sun =
        Celestial("sun", Transform{SUN_POSITION}, Type::Star, 333000.0f, 30.0f, {1.0f, 1.0f, 0.0f});
    Celestial mercury = Celestial("mercury", Transform{glm::vec3(-142.1f, 0.0f, 0.0f)},
                                  Type::Planet, 0.0553f, 8.0f, {0.55f, 0.50f, 0.48f});
    Celestial venus   = Celestial("venus", Transform{glm::vec3(-91.8f, 0.0f, 0.0f)}, Type::Planet,
                                  0.815f, 12.0f, {1.0f, 0.5f, 0.0f});
    Celestial earth   = Celestial("earth", Transform{glm::vec3(-50.4f, 0.0f, 0.0f)}, Type::Planet,
                                  1.0f, 12.0f, {0.0f, 0.75f, 0.0f});
    Celestial mars    = Celestial("mars", Transform{glm::vec3(27.9f, 0.0f, 0.0f)}, Type::Planet,
                                  0.107f, 8.0f, {1.0f, 0.0f, 0.0f});
    Celestial jupiter = Celestial("jupiter", Transform{glm::vec3(578.5f, 0.0f, 0.0f)}, Type::Planet,
                                  317.8f, 200.0f, {0.76f, 0.60f, 0.42f});
    Celestial saturn  = Celestial("saturn", Transform{glm::vec3(1226.7f, 0.0f, 0.0f)}, Type::Planet,
                                  95.2f, 180.0f, {0.87f, 0.78f, 0.57f});
    Celestial uran    = Celestial("uran", Transform{glm::vec3(2671.0f, 0.0f, 0.0f)}, Type::Planet,
                                  14.5f, 100.0f, {0.56f, 0.84f, 0.86f});
    Celestial neptun  = Celestial("neptun", Transform{glm::vec3(4298.3f, 0.0f, 0.0f)}, Type::Planet,
                                  17.1f, 100.0f, {0.25f, 0.41f, 0.88f});

    auto orbital_velocity = [&](Celestial& planet) {
        float r           = glm::length(planet.transform.pos - sun.transform.pos);
        planet.velocity.z = sqrt(G * sun.mass / r);
        planet.momentum   = planet.velocity * planet.rest_mass; // add this
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

    // grid

    std::vector<unsigned int> grid_indices;

    glm::ivec2 grid_size{50, 50};
    for (int i = 0; i < grid_size.x; i++) {
        for (int j = 0; j < grid_size.y; j++) {
            float x = -5000 + (i / (static_cast<float>(grid_size.x - 1))) * 10000;
            float z = -5000 + (j / (static_cast<float>(grid_size.y - 1))) * 10000;

            grid_vertices.push_back(x);
            grid_vertices.push_back(0.0f);
            grid_vertices.push_back(z);
        }
    }

    for (int i = 0; i < grid_size.x; i++) {
        for (int j = 0; j < grid_size.y - 1; j++) {
            grid_indices.push_back(i * grid_size.y + j);
            grid_indices.push_back(i * grid_size.y + j + 1);
        }
    }

    for (int i = 0; i < grid_size.y; i++) {
        for (int j = 0; j < grid_size.x - 1; j++) {
            grid_indices.push_back(j * grid_size.x + i);
            grid_indices.push_back((j + 1) * grid_size.x + i);
        }
    }

    grid_mesh = std::make_unique<Mesh>(grid_vertices, grid_indices, 3, true, DRAW_MODE::LINES);
}

void System::simulate(float dt) {
    ImGui::Begin("Window", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    render_sliders();
    render_planets();
    ImGui::End();

    dt *= time_multiplier;

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

    for (int i = 0; i < grid_vertices.size(); i += 3) {
        float& x = grid_vertices[i];
        float& y = grid_vertices[i + 1];
        float& z = grid_vertices[i + 2];

        float dip = 0.0f;
        for (const auto& body : bodies) {
            float dx              = x - body.transform.pos.x;
            float dz              = z - body.transform.pos.z;
            float horizontal_dist = sqrt(dx * dx + dz * dz) + SOFTENING;
            dip += body.mass / horizontal_dist;
        }

        y = STARTING_GRID.y - dip * vis_scale;
    }

    grid_mesh->update_vertices(grid_vertices);
    renderer->draw_unlit(*grid_mesh, Transform{}, glm::vec3(0.3f, 0.3f, 0.3f));
}

void System::render_sliders() {
    ImGui::SliderFloat("Time multiplicator", &time_multiplier, TIME_MULT_MIN, TIME_MULT_MAX);
    ImGui::SliderFloat("Visual grid scale", &vis_scale, VIS_SCALE_MIN, VIS_SCALE_MAX);
}

void System::render_planets() {
    for (auto& body : bodies) {
        ImGui::PushID(body.name.c_str());
        ImGui::CollapsingHeader(body.name.c_str());
        if (ImGui::SliderFloat("mass", &body.mass, 0, 1000000)) {
            body.rest_mass = body.mass;
        }
        ImGui::PopID();
    }
}