#version 330

uniform sampler2D gTexture;
uniform vec3 gLightPos;
uniform vec3 gCamPos;
uniform float gGamma;

out vec4 FragColour;

in Vertex
{
    vec4 pos;
    vec3 normal;
    vec2 uv;
}vert;

void main()
{
    vec3 colour = texture(gTexture, vert.uv).rgb;

    // ambient
    vec3 ambient = 0.05 * colour;
    
    // diffuse
    vec3 lightDir = normalize(gLightPos - vert.pos.xyz);
    vec3 normal = normalize(vert.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * colour;

    // specular
    vec3 viewDir = normalize(gCamPos - vert.pos.xyz);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);

    vec3 specular = vec3(0.3) * spec;
    FragColour = vec4(ambient + diffuse + specular, 1.0);
    FragColour.rgb = pow(FragColour.rgb, vec3(1.0/gGamma));
}