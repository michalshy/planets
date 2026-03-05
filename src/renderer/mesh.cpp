#include "mesh.h"

#include <glad/glad.h>

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, int stride, bool dynamic, DRAW_MODE mode) {
    draw_mode = mode;
    is_static = !dynamic;
    index_count = indices.size();
    // objs
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
                 reinterpret_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                 reinterpret_cast<void*>(indices.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::draw() const {
    glBindVertexArray(vao);
    glDrawElements(get_mode(), index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

int Mesh::get_mode() const {
    switch(draw_mode) {
        case DRAW_MODE::TRIANGLES:
            return GL_TRIANGLES;
        case DRAW_MODE::LINES:
            return GL_LINES;
    }
    return GL_TRIANGLES;
}

void Mesh::update_vertices(std::vector<float>& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
