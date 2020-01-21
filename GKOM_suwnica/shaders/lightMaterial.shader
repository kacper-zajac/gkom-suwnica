#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
#shader fragment
#version 450 core

layout(location = 0) out vec4 color;
in vec3 Normal;  
in vec3 FragPos;  
 
struct Material {
    vec3 ambient;
    vec3 diffuse;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{    

    vec3 ambient = vec3(0.5) * material.ambient * objectColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.direction - FragPos);
    float diff = max(dot(norm, lightDir), 0.1);
    vec3 diffuse = objectColor * light.diffuse * (diff * material.diffuse);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = objectColor * light.specular * spec;  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}