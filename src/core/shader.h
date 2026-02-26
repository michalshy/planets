#pragma once

#include <cstdint>

#include <string_view>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
    uint32_t id;

  public:
    Shader(std::string_view vertex_path, std::string_view fragment_path);

    void use();
    void set_bool(std::string_view name, bool value) const;
    void set_int(std::string_view name, int value) const;
    void set_float(std::string_view name, float value) const;
    void set_float4(std::string_view name, float x, float y, float z, float w) const;
    void set_mat4(std::string_view name, const glm::mat4& value) const;
    void set_vec3(std::string_view name, float x, float y, float z) const;
    void set_vec3(std::string_view name, const glm::vec3& value) const;
};