#include <cmath>
#include <numbers>
#include <vector>
#include <tuple>

static std::tuple<std::vector<float>, std::vector<unsigned int>>
generate_sphere(float radius, int stacks, int slices) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    for (int stack = 0; stack <= stacks; stack++) {
        for (int slice = 0; slice <= slices; slice++) {

            float phi   = std::numbers::pi * stack / stacks;
            float theta = 2 * std::numbers::pi * slice / slices;

            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    for (int stack = 0; stack < stacks; stack++) {
        for (int slice = 0; slice < slices; slice++) {
            unsigned int top_left     = stack * (slices + 1) + slice;
            unsigned int top_right    = top_left + 1;
            unsigned int bottom_left  = (stack + 1) * (slices + 1) + slice;
            unsigned int bottom_right = bottom_left + 1;

            indices.push_back(top_left);
            indices.push_back(bottom_left);
            indices.push_back(top_right);

            indices.push_back(top_right);
            indices.push_back(bottom_left);
            indices.push_back(bottom_right);
        }
    }

    return {vertices, indices};
}