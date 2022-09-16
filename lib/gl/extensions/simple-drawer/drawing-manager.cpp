#include "math.h"
#include "drawing-manager.h"
#include <iostream>

namespace gl {

    void drawing_manager::set_color(math::vec3 color) {
        m_current_color = color;
    }

    void drawing_manager::set_axes(axes axes) { m_axes = axes; }

    void drawing_manager::set_width(float width) {
        m_width = width;
    }


    void drawing_manager::draw_triangle(math::vec2 p0, math::vec2 p1, math::vec2 p2) {
        m_vertices.insert(m_vertices.end(), {
            { m_axes.get_view_coordinates(p0), m_current_color },
            { m_axes.get_view_coordinates(p1), m_current_color },
            { m_axes.get_view_coordinates(p2), m_current_color }
        });
    }

    void drawing_manager::draw_line(math::vec2 from, math::vec2 to) {
        math::vec shift = (from - to).perpendicular().normalized() * (m_width / 2.0f);

        // Vertices of rectangle
        math::vec p0 = from + shift, p1 = from - shift,
                  p2 = to   + shift, p3 = to   - shift;

        draw_triangle(p0, p1, p2), draw_triangle(p3, p1, p2);
    }

    static math::vec2 rot(math::vec2 current, float angle) {
        return { (float) cos(angle) * current.x() - (float) sin(angle) * current.y(),
                 (float) sin(angle) * current.x() + (float) cos(angle) * current.y() };
    }

    void drawing_manager::draw_vector(math::vec2 from, math::vec2 to) {
        draw_line(from, to);

        math::vec l = rot(from - to, -0.5f).normalized() * 0.1f;
        math::vec r = rot(from - to, +0.5f).normalized() * 0.1f;

        draw_line(to, to + l);
        draw_line(to, to + r);
    }

}
