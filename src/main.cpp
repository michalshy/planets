#include "simulation/system.h"
#include "core/window.h"
#include "renderer/renderer.h"
#include "core/geometry.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr glm::vec3 light_pos{1.2f, 12.0f, 2.0f};

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

    System solar;

    Transform cube;
    Transform light;
    light.pos   = light_pos;
    light.scale = {0.2f, 0.2f, 0.2f};

    renderer.add_light(light_pos);

    solar.init(&renderer);

    while (!window.should_close()) {
        window.time_check();
        window.process_input();
        renderer.clear();

        window.begin_frame();
        renderer.begin_frame(window.get_camera(), &window);

        solar.simulate(window.get_delta());

        window.end_frame();
    }
}
