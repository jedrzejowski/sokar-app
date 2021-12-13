#version 440
#extension GL_GOOGLE_include_directive : require

#include "SolidMaterial.glsl"
#include "SimpleLight.glsl"

layout(location = 0) in vec3 vertNormal;
layout(location = 1) in vec3 vertPos;

layout(binding = 1)
uniform FragUniformBufferObject {
    vec3 cameraPos;
    SimpleLight light;
    SolidMaterial material;
} uniBuf;

layout(location = 0) out vec4 fragColor;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    //    vec3 my_color = hsv2rgb(vec3(vertColor.x, 1.0f, 1.0f));
    vec3 my_color = uniBuf.material.color;

    vec3 ambient = uniBuf.light.ambient * my_color;

    vec3 lightDir = normalize(uniBuf.light.position - vertPos);
    float diff = max(dot(vertNormal, lightDir), 0.0);
    vec3 diffuse = uniBuf.light.diffuse * diff * my_color;

    //specular
    vec3 viewDir = normalize(uniBuf.cameraPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, vertNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uniBuf.material.shininess);
    vec3 specular = uniBuf.light.specular * (spec * uniBuf.material.specular);

    vec3 result = ambient + diffuse + specular;

    fragColor = vec4(result, 1.0f);
}
