#include "core/common.h"
#include "core/mesh.h"
#include "core/window.h"
#include "core/renderer.h"
#include "core/geometry.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr glm::vec3 light_pos{1.2f, 1.0f, 2.0f};

int main() {

    // Init window
    Window window;
    if (!window.init()) {
        return 1;
    }
    // Init renderer (after window!)
    Renderer renderer;
    if (!renderer.init()) {
        return 1;
    }

    Transform cube;
    Transform light;
    light.pos   = light_pos;
    light.scale = {0.2f, 0.2f, 0.2f};

    Mesh cube_msh(cube_vertices, cube_indices);

    auto [sphere_vertices, sphere_indices] = generate_sphere(1.0f, 40, 40);
    Mesh sphere_msh(sphere_vertices, sphere_indices);

    renderer.add_light(light_pos);

    while (!window.should_close()) {
        window.time_check();
        window.process_input();
        renderer.clear();

        renderer.begin_frame(window.get_camera());

        renderer.draw_lit(cube_msh, cube, {1.0f, 0.5f, 0.31f});
        renderer.draw_unlit(sphere_msh, light, {1.0f, 0.5f, 0.31f});
        window.end_frame();
    }
}
