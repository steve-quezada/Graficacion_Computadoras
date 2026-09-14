#version 430 core
in vec2 tc;
in vec3 FragPos;
in vec3 Normal;

layout(binding = 0) uniform sampler2D samp;

// 3 luces: 0 = Luz lunar, 1 = Baliza (point + atenuación), 2 = UFO (spotlight)
uniform vec3 lightPos[3];
uniform vec3 lightAmb[3];
uniform vec3 lightDif[3];
uniform vec3 lightSpec[3];

// Spotlight: luz 2 (UFO apunta hacia abajo)
uniform vec3  spotDir;       // dirección del cono
uniform float spotCutOuter;  // cos(ángulo exterior)
uniform float spotCutInner;  // cos(ángulo interior)

// Cámara
uniform vec3 viewPos;        // posición de la cámara para cálculo especular
uniform int  lightsOn;       // 1 = luces encendidas, 0 = apagadas
uniform int  shadersOn;      // 1 = Phong activo, 0 = solo textura flat

// Material por modelo
uniform float shininess;     // 10 = piedra opaca, 24 = semi-mate, 128 = metal
uniform vec3  matSpecular;   // intensidad y color del reflejo especular
uniform float matDiffuse;    // multiplicador del componente difuso

out vec4 FragColor;

// Luz puntual genérica.
vec3 calcPointLight(int i, vec3 norm, vec3 fragPos, vec3 viewDir, vec3 objColor)
{
    vec3 lightDir = normalize(lightPos[i] - fragPos);

    vec3  ambient  = lightAmb[i] * objColor;
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = lightDif[i] * (diff * matDiffuse) * objColor;
    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), shininess);
    vec3  specular = lightSpec[i] * spec * matSpecular;

    // Baliza: con atenuación para que no ilumine todo el modelo
    if (i == 1)
    {
        float dist  = length(lightPos[i] - fragPos);
        float atten = 1.0 / (1.0 + 0.18 * dist + 0.06 * dist * dist);
        diffuse  *= atten;
        specular *= atten;
    }

    return ambient + diffuse + specular;
}

// Spotlight del UFO (luz 2). Solo ilumina dentro del cono con suavizado en los bordes.
vec3 calcSpotLight(vec3 norm, vec3 fragPos, vec3 viewDir, vec3 objColor)
{
    vec3  lightDir  = normalize(lightPos[2] - fragPos);
    float theta     = dot(-lightDir, normalize(spotDir));
    float eps       = spotCutInner - spotCutOuter;
    float intensity = clamp((theta - spotCutOuter) / eps, 0.0, 1.0);

    // Ambiente mínimo fuera del cono
    vec3 ambient = lightAmb[2] * objColor * 0.01;

    if (intensity > 0.0)
    {
        float diff     = max(dot(norm, lightDir), 0.0);
        vec3  diffuse  = lightDif[2] * (diff * matDiffuse) * objColor * intensity;
        vec3  halfDir  = normalize(lightDir + viewDir);
        float spec     = pow(max(dot(norm, halfDir), 0.0), shininess);
        vec3  specular = lightSpec[2] * spec * matSpecular * intensity;
        return ambient + diffuse + specular;
    }
    return ambient;
}

void main()
{
    vec3 objColor = texture(samp, tc).rgb;

    // Modo flat: solo textura sin iluminación
    if (shadersOn == 0)
    {
        FragColor = vec4(objColor, 1.0);
        return;
    }

    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result  = vec3(0.0);

    if (lightsOn == 1)
    {
        result += calcPointLight(0, norm, FragPos, viewDir, objColor); // Luz lunar
        result += calcPointLight(1, norm, FragPos, viewDir, objColor); // Baliza
        result += calcSpotLight(norm, FragPos, viewDir, objColor);     // UFO spotlight
    }
    else
    {
        // Luces apagadas
        result = objColor * 0.15;
    }

    // Color grading
    // 1. Desaturar para reducir verdes/marrones
    float luma = dot(result, vec3(0.299, 0.587, 0.114));
    result = mix(vec3(luma), result, 0.75);

    // 2. Tinte frío azulado
    result *= vec3(0.90, 0.96, 1.06);

    // 3. Bajar exposición para escena nocturna 
    result *= 1.05;

    // 4. Contraste suave
    result = pow(result, vec3(1.06));

    FragColor = vec4(result, 1.0);
}
