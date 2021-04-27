#version 330 core
// This is a sample fragment shader.

struct Material{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 direction;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 norm_color;
in vec3 Normal;
in vec3 FragPos;
in float obeyLight;

uniform vec3 viewPos;
uniform int coloringType; // which light sourse

uniform Material mat;
uniform Light DirLight;
uniform Light PointLight;

uniform vec3 eyePos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    vec3 ambient  = light.ambient         * mat.ambient;
    vec3 diffuse  = light.diffuse  * (diff * mat.diffuse);
    vec3 specular = light.specular * (spec * mat.specular);

    return (ambient + diffuse + specular) * mat.color;
}

vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient  = light.ambient         * mat.ambient;
    vec3 diffuse  = light.diffuse  * (diff * mat.diffuse);
    vec3 specular = light.specular * (spec * mat.specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * mat.color;
}

void main()
{
    if(obeyLight < 0.0f) { fragColor = vec4(1.0f); return;}
//    vec3 viewDir = normalize(viewPos - eyePos);
    vec3 viewDir = normalize(eyePos - FragPos);
    
    if(coloringType == 0) { // no light
        fragColor = vec4(0.05f, 0.05f, 0.05f, 0.05f);
    }
    else if(coloringType == 1) { // dir light only
        fragColor =  vec4(calcDirLight(DirLight, Normal, viewDir), 1.0f);
    }
    else if(coloringType == 2) { // point light only
        fragColor =  vec4(calcPointLight(PointLight, Normal, FragPos, viewDir), 1.0f);
    }
    else if(coloringType == 3) { // both light sourse
        fragColor =  vec4(calcDirLight(DirLight, Normal, viewDir) + calcPointLight(PointLight, Normal, FragPos, viewDir), 1.0f);
    }
    else if(coloringType == 4) {
        // Use the color passed in. An alpha of 1.0f means it is not transparent.
        fragColor = vec4(norm_color, 1.0f);
        
    }
}
