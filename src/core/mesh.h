#pragma once

#include <vector>

class Mesh {
    unsigned int vao, vbo, ebo;
    int index_count;

  public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
    void draw();
    ~Mesh();
};