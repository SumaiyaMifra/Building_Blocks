#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};



struct PointLight {
    vec3 position;
    
    float k_c;  // attenuation factors
    float k_l;  // attenuation factors
    float k_q;  // attenuation factors
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct DirectionalLight {

vec3 direction;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};


struct SpotLight {                    //Spot Light
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Kc;
    float Kl;
    float Kq;
};




#define NR_POINT_LIGHTS 8

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;


// function prototypes
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V);

vec3 CalcDirLight(Material material,DirectionalLight light, vec3 normal, vec3 viewDir);  

vec3 CalcSpotLight(Material material, SpotLight light, vec3 normal, vec3 fragPos);


void main()
{
    // properties
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);


    
    
    vec3 result;
    // point lights
    for(int i = 0; i < 1; i++)
        result += CalcPointLight(material, pointLights[i], N, FragPos, V);
    
    result += CalcDirLight(material,directionalLight,N, V);

    result += CalcSpotLight(material,spotLight,N,FragPos);

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    
    // attenuation
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}



vec3 CalcDirLight(Material material, DirectionalLight light, vec3 N, vec3 V)
{
    vec3 L = normalize(-light.direction);
    vec3 R = reflect(-L, N);
    
    
    
    vec3 ambient = material.ambient * light.ambient;
    vec3 diffuse = material.diffuse * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = material.specular * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    
    return (ambient + diffuse + specular);
}





vec3 CalcSpotLight(Material material, SpotLight light, vec3 normal, vec3 fragPos)
{
    // Attenuation based on distance
    float distance  = length(light.position - fragPos);
    float attenuation = 1.0 / (light.Kc + light.Kl * distance + light.Kq * (distance * distance));

    // Ambient lighting
    vec3 ambient = light.ambient * material.ambient;
    ambient *= attenuation;

    // Normalized vectors
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    diffuse *= attenuation;

    // View and specular lighting
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    specular *= attenuation;

    // Spotlight effect
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}
