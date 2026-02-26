#include "shader.h"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <print>

Shader::Shader(std::string_view vertex_path, std::string_view fragment_path) {
    // raw code
    std::string vertex_code;
    std::string fragment_code;

    // file streams
    std::ifstream vshader_file;
    std::ifstream fshader_file;

    vshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fshader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vshader_file.open(vertex_path.data());
        fshader_file.open(fragment_path.data());

        std::stringstream vshader_stream, fshader_stream;
        vshader_stream << vshader_file.rdbuf();
        fshader_stream << fshader_file.rdbuf();

        vshader_file.close();
        fshader_file.close();

        vertex_code   = vshader_stream.str();
        fragment_code = fshader_stream.str();
    } catch (std::ifstream::failure e) {
        std::println("Shader not succesfully read");
    }

    const char *vshader_code = vertex_code.c_str();
    const char *fshader_code = fragment_code.c_str();

    unsigned int vertex, fragment;
    int success;
    char info_log[512];
    // vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vshader_code, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::println("Vertex compilation failed");
    }
    // fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fshader_code, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(fragment, 512, NULL, info_log);
        std::println("Fragment compilation failed");
    }
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, info_log);
        std::println("Shader linking failed");
    }
    // delete linked shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::set_bool(std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.data()), (int)value);
}

void Shader::set_int(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.data()), value);
}

void Shader::set_float(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.data()), value);
}

void Shader::set_float4(std::string_view name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, name.data()), x, y, z, w);
}

void Shader::set_mat4(std::string_view name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_vec3(std::string_view name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.data()), x, y, z);
}

void Shader::set_vec3(std::string_view name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id, name.data()), 1, glm::value_ptr(value));
}