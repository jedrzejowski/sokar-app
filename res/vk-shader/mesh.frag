#version 440

layout(binding = 0)
uniform MeshUniformVertBuf {
    mat4 model;
    mat4 camera;
    mat4 proj;
    vec3 color;
    vec3 lightPos;
} uniBuf;

layout(location = 0) out vec4 fragColor;

void main()
{
//        fragColor = vec4(uniBuf.color, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
