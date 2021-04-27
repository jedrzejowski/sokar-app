#version 440

struct SolidMaterial {
    vec3 color;
    vec3 specular;
    float shininess;
};

struct SimpleLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 vertPos;

layout(binding = 1)
uniform FragUniformBufferObject {
    mat4 camera;
    SimpleLight light;
    SolidMaterial material;
} uniBuf;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = vec4(uniBuf.material.color, 1.0);
//        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
