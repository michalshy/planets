#pragma once

#include <vector>

enum class DRAW_MODE {
  TRIANGLES,
  LINES
};

class Mesh {
    unsigned int vao, vbo, ebo;
    int index_count;
    bool is_static;
    DRAW_MODE draw_mode;

  public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, int stride, bool dynamic, DRAW_MODE mode);
    ~Mesh();
    
    void draw() const;
    int get_mode() const;

    void update_vertices(std::vector<float>& vertices);
};