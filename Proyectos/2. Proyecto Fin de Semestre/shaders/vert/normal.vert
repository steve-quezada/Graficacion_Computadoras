#version 430 core

layout (location = 0) in vec3 aPos;        // posición del vértice
layout (location = 1) in vec3 aNormal;     // normal geométrica del vértice
layout (location = 2) in vec2 aTexCoords;  // coordenadas de textura UV
layout (location = 3) in vec3 aTangent;    // tangent
layout (location = 4) in vec3 aBitangent;  // bitangente

// Salidas al fragment shader
out vec3 v_fragPos; // posición en world space
out vec2 tc;        // coordenadas UV pasadas al sampler
out mat3 TBN;       // matriz tangente-bitangente-normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    v_fragPos = vec3(worldPos);
    tc = aTexCoords;

    // Matriz normal: transpuesta de la inversa del model para corregir escalas no uniformes
    mat3 normalMat = transpose(inverse(mat3(model)));

    // Construir los tres ejes de la matriz
    vec3 T = normalize(normalMat * aTangent);
    vec3 B = normalize(normalMat * aBitangent);
    vec3 N = normalize(normalMat * aNormal);
    TBN = mat3(T, B, N); // tangente, bitangente, normal

    gl_Position = projection * view * worldPos;
}
