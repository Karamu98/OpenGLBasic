#version 330

uniform sampler2D gTexture;
//uniform vec3 gLightPos;
//uniform vec3 gCamPos;

out vec4 FragColour;

in Vertex
{
    vec4 pos;
    vec3 normal;
    vec2 uv;
}vert;


vec3 lightColour;

void main()
{
    lightColour = vec3(1, 1, 1);

    //float ambientStrength = 0.1;
    //vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
    //vec3 norm = normalize(vert.normal);
    //vec3 lightDir = normalize(gLightPos - vert.pos.xyz);
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * lightColour;
    
    // specular
    //float specularStrength = 0.5;
    //vec3 viewDir = normalize(gCamPos - vert.pos.xyz);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColour;  
        
    //vec3 result = (ambient + diffuse + specular) * texture(gTexture, vert.uv).rgb; ;
    //FragColour = vec4(result, 1.0);
    FragColour = texture(gTexture, vert.uv);
}