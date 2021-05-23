#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal; 
in vec2 TexCoords;
struct Material {
    vec3 Diffuse;
    vec3 Specular;
    vec3 Ambient;
    float Shininess;
}; 
uniform sampler2D texture_diffuse1;
uniform Material material;
uniform vec3 base_color;

uniform vec3 AmbientColor = vec3(0.0);
uniform vec3 LightDirection = normalize(vec3(1, 1, 1));
uniform vec3 LightColor = vec3(1);

void main()
{    
    vec3 irradiance = material.Ambient + LightColor * max(0, dot(LightDirection, Normal));

	// Diffuse reflectance
	vec3 reflectance = irradiance * material.Diffuse;

	// Gamma correction
	FragColor = vec4(sqrt(reflectance), 1);
    
    // add texture if have one
    vec4 texture_color = texture(texture_diffuse1, TexCoords);
    if(texture_color.xyz != vec3(0.0,0.0,0.0))
        FragColor = texture_color;
}