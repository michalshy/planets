#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CAM_MOVEMENT { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right;
    glm::vec3 world_up{0.0f, 1.0f, 0.0f};
    float yaw{-90.0f};
    float pitch{0.0f};
    float movement_speed{2.5f};
    float mouse_sensitivity{0.1f};
    float zoom{45.0f};

  public:
    Camera();

    glm::mat4 get_view_mat();
    void process_keyboard(CAM_MOVEMENT dir, float dt);
    void process_mouse(float x_offset, float y_offset, GLboolean constraint_pitch = true);
    void process_scroll(float y_offset);

  private:
    void update_camera_vecs();
};