#shader vertex   ------------------------------------------------------------------------------------------

#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 frag_color;

void main() {
    frag_color = color;
    gl_Position = position;
}

#shader fragment ------------------------------------------------------------------------------------------

#version 460 core

in vec4 frag_color;
out vec4 color;

void main() {
    color = frag_color;
}
