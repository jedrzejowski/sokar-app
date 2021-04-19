#version 440

layout(push_constant) uniform MeshConstants {
    mat4 model;
    mat4 camera;
    mat4 proj;
    vec3 color;
} meshConsts;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(meshConsts.color, 1.0);
}
