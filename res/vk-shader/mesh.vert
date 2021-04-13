#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out gl_PerVertex { vec4 gl_Position; };

layout(push_constant) uniform MeshConstants {
    mat4 model;
    mat4 camera;
    mat4 proj;
    vec3 color;
} meshConsts;

void main()
{
    gl_Position = meshConsts.mvp * vec4(position, 1.0f);
}
