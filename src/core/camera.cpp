#include "camera.h"

Camera::Camera() {
    update_camera_vecs();
}

glm::mat4 Camera::get_view_mat() {
    return glm::lookAt(position, position + front, up);
}

void Camera::process_keyboard(CAM_MOVEMENT dir, float dt) {
    float vel = movement_speed * dt;
    if (dir == CAM_MOVEMENT::FORWARD)
        position += front * vel;
    if (dir == CAM_MOVEMENT::BACKWARD)
        position -= front * vel;
    if (dir == CAM_MOVEMENT::LEFT)
        position -= right * vel;
    if (dir == CAM_MOVEMENT::RIGHT)
        position += right * vel;
}

void Camera::process_mouse(float x_offset, float y_offset, GLboolean constraint_pitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (constraint_pitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    update_camera_vecs();
}

void Camera::process_scroll(float y_offset) {
    zoom -= y_offset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::update_camera_vecs() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front   = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(
        front, world_up)); // normalize the vectors, because their length gets closer to 0 the
                           // more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}