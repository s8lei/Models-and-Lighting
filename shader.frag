#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 normalOutput;
in vec3 posOutput;
uniform vec3 color;
uniform bool colorMode;
uniform bool isLight;
uniform bool isDirLight;
uniform bool isPtLight;
uniform bool isSptLight;
uniform vec3 lightPos;
uniform vec3 dirLightDir;
uniform vec3 sptLightDir;
uniform float cutoff;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

vec3 CalcDirLight(vec3 lightDir);
vec3 CalcSptLight(vec3 lightDir, vec3 spotDir);
void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    //fragColor = vec4(color, sampleExtraOutput);
    vec3 viewPos = vec3(0.0f, 0.0f, 20.0f);
    vec3 lightColor = vec3(1.0f, 0.5f, 0.31f);
    //vec3 lightColor = vec3(0.0f, 0.0f, 0.0f);
    lightColor = lightColor/length(lightPos - posOutput);
    if (colorMode) {
        fragColor = vec4((normalOutput+vec3(1.0, 1.0, 1.0))/2, sampleExtraOutput);
    }
    else if (!isLight){
        // ambient
        vec3 ambient = material.ambient;
        
        // diffuse
        vec3 lightDir = normalize(lightPos - posOutput);
        float diff = max(dot(normalOutput, lightDir), 0.0);
        vec3 diffuse = lightColor * (diff * material.diffuse);
        //vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
        // specular
        vec3 viewDir = normalize(viewPos - posOutput);
        vec3 reflectDir = reflect(-lightDir, normalOutput);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lightColor * (spec * material.specular);
        //vec3 specular = vec3(0.1f, 0.1f, 0.1f);
        vec3 result = vec3(0.0f, 0.0f, 0.0f);
        if (isPtLight)
            result += ambient + diffuse + specular;
        if (isDirLight)
            result += CalcDirLight(dirLightDir);
        if (isSptLight)
            result += CalcSptLight(lightDir, sptLightDir);
        fragColor = vec4(result, sampleExtraOutput);
    }
    else {
        fragColor = vec4(material.ambient, sampleExtraOutput);
    }
}
vec3 CalcDirLight(vec3 lightDir)
{
    vec3 viewPos = vec3(0.0f, 0.0f, 20.0f);
    vec3 lightColor = vec3(1.0f, 0.5f, 0.31f);
    vec3 ambient = material.ambient;
    float diff = max(dot(normalOutput, normalize(-lightDir)), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);
    //vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
    // specular
    vec3 viewDir = normalize(viewPos - posOutput);
    vec3 reflectDir = reflect(-lightDir, normalOutput);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);
    //vec3 specular = vec3(0.1f, 0.1f, 0.1f);
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalcSptLight(vec3 lightDir, vec3 spotDir)
{
    float theta = dot(lightDir, normalize(-spotDir));
    vec3 result;
    if(theta > cutoff)
    {
      vec3 viewPos = vec3(0.0f, 0.0f, 20.0f);
      vec3 lightColor = vec3(1.0f, 0.5f, 0.31f);
      vec3 ambient = material.ambient;
      float diff = max(dot(normalOutput, normalize(-lightDir)), 0.0);
      vec3 diffuse = lightColor * (diff * material.diffuse);
      //vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
      // specular
      vec3 viewDir = normalize(viewPos - posOutput);
      vec3 reflectDir = reflect(-lightDir, normalOutput);
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      vec3 specular = lightColor * (spec * material.specular);
      //vec3 specular = vec3(0.1f, 0.1f, 0.1f);
      result = ambient + diffuse + specular;
    }
    else // else, use ambient light so scene isn't completely dark outside the spotlight.
      result = vec3(0.0f, 0.0f, 0.0f);
    return result;
}
