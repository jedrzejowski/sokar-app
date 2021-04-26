#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

//
layout(binding = 0)
uniform MeshUniformVertBuf {
    mat4 model;
    mat4 camera;
    mat4 proj;
    vec3 color;
    vec3 lightPos;
} uniBuf;

//layout(push_constant) uniform MeshConstants {
//    mat4 model;
//    mat4 camera;
//    mat4 proj;
//    vec3 color;
//} meshConsts;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    gl_Position = uniBuf.proj * uniBuf.camera * uniBuf.model * vec4(position, 1.0f);
    //        gl_Position = vec4(position, 1.0f);
}
