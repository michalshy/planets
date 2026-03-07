#include "core/common.h"
#include "simulation/system.h"
#include "core/window.h"
#include "renderer/renderer.h"
#include "core/geometry.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    renderer.add_light(SUN_POSITION);

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
