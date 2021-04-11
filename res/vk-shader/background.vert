#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 normal;

out gl_PerVertex { vec4 gl_Position; };

layout(push_constant) uniform PC {
    mat4 mvp;
} pc;

void main()
{
    gl_Position = pc.mvp * vec4(position, 1.0f);
}
