#pragma once

#include "axes.h"
#include "colored-vertex.h"
#include "vertex-vector-array.h"
#include "vec.h"

namespace gl {

    class drawing_manager {
    public:
        // Uses black color by default
        drawing_manager(gl::vertex_vector_array<colored_vertex>& vertices)
            : m_vertices(vertices), m_current_color(0.0f, 0.0f, 0.0f) {}

        // ==> Control current settings:

        void set_color(math::vec3 color);
        void set_width(float width);

        void set_axes(axes axes);

        // ==> Draw shapes:

        void draw_triangle(math::vec2 p0, math::vec2 p1, math::vec2 p2);
        void draw_line(math::vec2 from, math::vec2 to);

        void draw_vector(math::vec2 from, math::vec2 to);

    private:
        gl::vertex_vector_array<colored_vertex>& m_vertices;

        // ==> Current settings:
        axes m_axes;

        math::vec3 m_current_color;
        float m_width;
    };

}
