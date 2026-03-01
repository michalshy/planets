#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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