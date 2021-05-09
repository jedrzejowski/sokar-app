
#include "./SolidMaterial.glsl"
#include "./SimpleLight.glsl"

uniform FragUniformBufferObject {
    vec3 cameraPos;
    SimpleLight light;
    SolidMaterial material;
};

uniform VertUniformBufferObject {
    mat4 model;
    mat4 camera;
    mat4 proj;
};
