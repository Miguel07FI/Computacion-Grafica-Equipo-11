#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float explosionFactor;

void main()
{
    vec3 explodedPosition = position + normal * explosionFactor;

    // Usa la posición explosionada para todo
    vec4 worldPosition = model * vec4(explodedPosition, 1.0);
    FragPos = vec3(worldPosition);
    gl_Position = projection * view * worldPosition;

    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}
