#version 440

layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 tex;
layout(location = 1) in vec3 normal;

//
layout(binding = 0)
uniform VertUniformBufferObject {
    mat4 model;
    mat4 camera;
    mat4 proj;
} uniBuf;

//layout(push_constant) uniform MeshConstants {
//    mat4 model;
//    mat4 camera;
//    mat4 proj;
//    vec3 color;
//} meshConsts;

out gl_PerVertex { vec4 gl_Position; };
layout(location = 0) out vec3 vertNormal;
layout(location = 1) out vec3 vertPos;

void main()
{
    vertNormal = vec3(normalize(uniBuf.model * vec4(normal, 1.0f)));
    vertPos = vec3(uniBuf.model * vec4(position, 1.0f));
    gl_Position = uniBuf.proj * uniBuf.camera * uniBuf.model * vec4(position, 1.0f);
}
