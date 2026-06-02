#version 430 core
in vec2 tc;
in vec3 FragPos;
in vec3 Normal;

layout(binding = 0) uniform sampler2D samp;

uniform vec3 lightPos;      // posicion de la luz en world space
uniform vec3 viewPos;       // posicion de la camara especular

uniform vec3 lightAmbient;  // color de luz ambiente
uniform vec3 lightDiffuse;  // color de luz difusa
uniform vec3 lightSpecular; // color de luz especular


out vec4 FragColor;

void main()
{
    
    vec3 objectColor = texture(samp, tc).rgb; // obtenemos el color de la textura 

    // Ambient light 
    vec3 ambient = lightAmbient;

    // Diffuse Light
    vec3 norm = normalize(Normal); // Target - Origin 
    vec3 lightDir = normalize(lightPos - FragPos); // vector hacia la luz
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightDiffuse;

    // Specular 
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightSpecular;

    // vec3 result = (ambient + diffuse) * objectColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
