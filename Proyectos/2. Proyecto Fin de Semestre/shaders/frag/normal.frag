#version 430 core
in vec3 v_fragPos;
in vec2 tc;
in mat3 TBN;

layout(binding = 0) uniform sampler2D samp;
layout(binding = 1) uniform sampler2D normalMap;

// 3 luces: 0 = Luz lunar, 1 = Baliza (point + atenuación), 2 = UFO (spotlight)
uniform vec3 lightPos[3];
uniform vec3 lightAmb[3];
uniform vec3 lightDif[3];
uniform vec3 lightSpec[3];

// Spotlight: luz 2 (UFO apunta hacia abajo)
uniform vec3  spotDir;
uniform float spotCutOuter;
uniform float spotCutInner;

// Cámara y toggles
uniform vec3 viewPos;
uniform int  lightsOn;
uniform int  shadersOn;
uniform int  useNormalMap;

// Material por modelo
uniform float shininess;
uniform vec3  matSpecular;
uniform float matDiffuse;

out vec4 FragColor;

// Luz puntual genérica.
vec3 calcPointLight(int i, vec3 N, vec3 fragPos, vec3 viewDir, vec3 objectColor)
{
    vec3 lightDir = normalize(lightPos[i] - fragPos);

    vec3  ambient  = lightAmb[i] * objectColor;
    float diff     = max(dot(N, lightDir), 0.0);
    vec3  diffuse  = lightDif[i] * diff * objectColor;
    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(N, halfDir), 0.0), shininess);
    vec3  specular = lightSpec[i] * spec * matSpecular;

    // Baliza
    if (i == 1)
    {
        float dist  = length(lightPos[i] - fragPos);
        float atten = 1.0 / (1.0 + 0.18 * dist + 0.06 * dist * dist);
        diffuse  *= atten;
        specular *= atten;
    }

    return ambient + diffuse + specular;
}

// Spotlight del UFO
vec3 calcSpotLight(vec3 N, vec3 fragPos, vec3 viewDir, vec3 objectColor)
{
    vec3  lightDir  = normalize(lightPos[2] - fragPos);
    float theta     = dot(-lightDir, normalize(spotDir));
    float eps       = spotCutInner - spotCutOuter;
    float intensity = clamp((theta - spotCutOuter) / eps, 0.0, 1.0);

    vec3 ambient = lightAmb[2] * objectColor * 0.05;

    if (intensity > 0.0)
    {
        float diff     = max(dot(N, lightDir), 0.0);
        vec3  diffuse  = lightDif[2] * diff * objectColor * intensity;
        vec3  halfDir  = normalize(lightDir + viewDir);
        float spec     = pow(max(dot(N, halfDir), 0.0), shininess);
        vec3  specular = lightSpec[2] * spec * matSpecular * intensity;
        return ambient + diffuse + specular;
    }
    return ambient;
}

void main()
{
    vec3 objectColor = texture(samp, tc).rgb;

    // Modo flat: solo textura sin iluminación
    if (shadersOn == 0)
    {
        FragColor = vec4(objectColor, 1.0);
        return;
    }

    // Normal: desde el mapa de normales o desde la geometría
    vec3 N;
    if (useNormalMap == 1)
    {
        vec3 nm = texture(normalMap, tc).rgb;
        nm = nm * 2.0 - 1.0;
        N = normalize(TBN * nm);
    }
    else
    {
        N = normalize(TBN[2]);
    }

    vec3 viewDir = normalize(viewPos - v_fragPos);
    vec3 result  = vec3(0.0);

    if (lightsOn == 1)
    {
        result += calcPointLight(0, N, v_fragPos, viewDir, objectColor); // Luz lunar
        result += calcPointLight(1, N, v_fragPos, viewDir, objectColor); // Baliza
        result += calcSpotLight(N, v_fragPos, viewDir, objectColor);     // UFO spotlight
    }
    else
    {
        result = objectColor * 0.15;
    }

    // Color grading frío
    result *= vec3(0.90, 0.95, 1.08);
    result *= 0.85;

    FragColor = vec4(result, 1.0);
}
