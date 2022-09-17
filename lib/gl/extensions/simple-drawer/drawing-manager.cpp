#include "colored-vertex.h"
#include "math.h"
#include "drawing-manager.h"
#include <iostream>

namespace gl {

    void drawing_manager::set_color(math::vec3 color) {
        m_current_color = { color.r(), color.g(), color.b(), 1.0f };
    }

    void drawing_manager::set_alpha(float alpha) {
        m_current_color.a() = alpha;
    }

    void drawing_manager::set_axes(axes axes) { m_axes = axes; }

    void drawing_manager::set_width(float width) {
        m_width = width;
    }


    void drawing_manager::draw_interpolated_triangle(colored_vertex p0, colored_vertex p1, colored_vertex p2) {
        m_vertices.insert(m_vertices.end(), {
            { m_axes.get_view_coordinates(p0.point), p0.color },
            { m_axes.get_view_coordinates(p1.point), p1.color },
            { m_axes.get_view_coordinates(p2.point), p2.color }
        });
    }

    void drawing_manager::draw_triangle(math::vec2 p0, math::vec2 p1, math::vec2 p2) {
        draw_interpolated_triangle({ p0, m_current_color },
                                   { p1, m_current_color },
                                   { p2, m_current_color });
    }

    void drawing_manager::draw_line(math::vec2 from, math::vec2 to) {
        math::vec direction = from - to;
        math::vec shift = direction
            .perpendicular().normalized();

        // Line will be separated in 1/4 (2/4 for monochrome middle,
        // and 1/2 for antialiased halfs):
        shift *= (m_width / 2.0f);

        math::vec cap_shift = direction.normalized();
        cap_shift.normalize() *= m_width / 2.0f;

        // Apply rectangulare cap to the line:
        from += cap_shift, to -= cap_shift;

        // Vertices of rectangle
        math::vec p0 = from, p1 = from, p2 = to, p3 = to;

        // ==> Draw monochrome middle:

        p1 -= shift, p3 -= shift;
        p0 += shift, p2 += shift;

        draw_triangle(p0, p1, p2), draw_triangle(p3, p1, p2);
    }

    void drawing_manager::draw_antialiased_line(math::vec2 from, math::vec2 to,
                                                float antialiasing_level) {
        math::vec direction = from - to;
        math::vec shift = direction
            .perpendicular().normalized();

        // Line will be separated in 1/4 (2/4 for monochrome middle,
        // and 1/2 for antialiased halfs):
        shift *= (m_width / 4.0f);

        math::vec cap_shift = direction.normalized();
        cap_shift.normalize() *= m_width / 2.0f;

        // Apply rectangulare cap to the line:
        from += cap_shift, to -= cap_shift;

        // Vertices of rectangle
        math::vec p0 = from, p1 = from, p2 = to, p3 = to;

        // ==> Draw monochrome middle:

        p1 -= shift, p3 -= shift;
        p0 += shift, p2 += shift;

        draw_triangle(p0, p1, p2), draw_triangle(p3, p1, p2);


        // Draw left half:
        math::vec desaturated_color = m_current_color;
        // Should be completely transparent:
        desaturated_color.a() = 1.0f - antialiasing_level;

        std::swap(m_current_color, desaturated_color);

        p0 += shift * 1.0f; p2 += shift * 1.0f; 
        p1 += shift * 2.0f; p3 += shift * 2.0f;

        draw_interpolated_triangle({ p0,   m_current_color },
                                   { p2,   m_current_color },
                                   { p1, desaturated_color });

        draw_interpolated_triangle({ p2,   m_current_color },
                                   { p1, desaturated_color },
                                   { p3, desaturated_color });

        p0 -= shift * 1.0f; p2 -= shift * 1.0f; 
        p1 -= shift * 2.0f; p3 -= shift * 2.0f; // Restore previous state

        // Draw right half:
        p1 -= shift * 1.0f; p3 -= shift * 1.0f;
        p0 -= shift * 2.0f, p2 -= shift * 2.0f;
        
        draw_interpolated_triangle({ p1,   m_current_color },
                                   { p3,   m_current_color },
                                   { p0, desaturated_color });

        draw_interpolated_triangle({ p3,   m_current_color },
                                   { p2, desaturated_color },
                                   { p0, desaturated_color });
        std::swap(m_current_color, desaturated_color);
    }

    static math::vec2 rot(math::vec2 current, float angle) {
        return { (float) cos(angle) * current.x() - (float) sin(angle) * current.y(),
                 (float) sin(angle) * current.x() + (float) cos(angle) * current.y() };
    }

    void drawing_manager::draw_vector(math::vec2 from, math::vec2 to) {
        math::vec l = rot(from - to, -0.5f).normalized() * 0.1f;
        math::vec r = rot(from - to, +0.5f).normalized() * 0.1f;

        draw_antialiased_line(from, to - (to - from).normalized() * 0.04f);
        draw_triangle(to, to + l, to + r);

    }

}
