#version 330

struct Light
{
    vec3 pos;
    vec3 colour;
};

struct Material
{
    sampler2D texture;
    float spec;
};


uniform Material gMaterial;
uniform Light gLight;
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
    // ambient
    vec3 ambient = 0.05 * gLight.colour;

    // diffuse
    vec3 lightDir = normalize(gLight.pos - vert.pos.xyz);
    vec3 normal = normalize(vert.normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * gLight.colour;

    // specular
    vec3 viewDir = normalize(gCamPos - vert.pos.xyz);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), gMaterial.spec);
    vec3 specular = gLight.colour * spec;

    // Grab unclamped final colour
    vec3 hdr = texture(gMaterial.texture, vert.uv).rgb * (ambient + diffuse + specular);

    /// Reinhard tone mapping
    vec3 mapped = hdr / (hdr + vec3(1.0));

    // Gamma correction
    FragColour = vec4(pow(mapped, vec3(1.0 / gGamma)), 1.0);
}