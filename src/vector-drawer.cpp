#include "axes.h"
#include "drawing-manager.h"
#include "gl.h"
#include "simple-window.h"

class vector_drawer: public gl::simple_drawing_window {
public:
    using gl::simple_drawing_window::simple_drawing_window;

    void window_setup() override {
        rectangle left_view {
            { -1.0f, -0.5f },
            {  0.0f, +0.5f } 
        };

        const float left_delta = 0.05f;
        m_rotating_vec_axes = left_view.shrink({ left_delta, left_delta });

        rectangle right_view {
            {  0.0f, -0.5f },
            { +1.0f, +0.5f } 
        };

        const float right_delta = 0.2f;
        m_selectable_vec_axes =
            right_view.shrink({ right_delta, right_delta });
    }

    void loop_draw(gl::drawing_manager &mgr) override {
        // Choose green-ish whatever color: 
        mgr.set_color({ 0.5f, 0.7f, 0.3f });

        mgr.set_axes(m_rotating_vec_axes);
        if (get_fps()) // At the start (before enough data is collected), fps is zero
            m_rotating_vec.rotate(-0.1f / (float) get_fps());

        draw_vec_with_axes(mgr, m_rotating_vec);

        mgr.set_axes(m_selectable_vec_axes);
        draw_vec_with_axes(mgr, m_selectable_vec);

    }

    virtual void on_mouse_moved(math::vec2 cursor) override {
        math::vec transformed =
            m_selectable_vec_axes.get_world_coordinates(cursor);

        if (transformed.x() > +1.0f || transformed.y() > +1.0f)
            return;

        if (transformed.x() < -1.0f || transformed.y() < -1.0f)
            return;

        m_selectable_vec = transformed;
    }

private:
    // Set to a random value that looks nice at the start:
    math::vec2 m_selectable_vec = { 0.7f,  0.7f };
    math::vec2   m_rotating_vec = { 0.7f,  0.7f };


    axes m_rotating_vec_axes, m_selectable_vec_axes;

    const float BOUNDING_BOX_WIDTH = 0.02f;
    const float AXES_WIDTH = 0.02f;

    void draw_vec_with_axes(gl::drawing_manager &mgr, math::vec2 vector) {
        // All sides of our available rectangle:
        math::vec x0 = { -1.0f, -1.0f }, x1 = { -1.0f,   1.0f };
        math::vec x2 = {  1.0f,  1.0f }, x3 = {  1.0f,  -1.0f };
            
        // ==> Draw bounding box:

        mgr.set_color({ 0.5f, 0.5f, 0.5f });

        mgr.draw_triangle(x0, x1, x2);
        mgr.draw_triangle(x2, x3, x0);

        mgr.set_color({ 0.0f, 0.0f, 0.0f });

        mgr.set_width(BOUNDING_BOX_WIDTH);

        mgr.draw_line(x0, x1);
        mgr.draw_line(x1, x2);
        mgr.draw_line(x2, x3);
        mgr.draw_line(x3, x0);

        // Draw vector with same with as bounding box:
        mgr.draw_vector({ 0.0f, 0.0f }, vector);

        // ==> Draw "axes":

        mgr.set_width(AXES_WIDTH);

        mgr.draw_line({  0.0f, -1.0f }, {  0.0f, 1.0f });
        mgr.draw_line({ -1.0f,  0.0f }, {  1.0f, 0.0f });

    }

};

int main() {
    vector_drawer drawer(1080, 1080, "My vector drawer!");
    drawer.draw_loop();
}
