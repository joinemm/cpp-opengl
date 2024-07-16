#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  sampler2D diffuseMap;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 brightness;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;
uniform sampler2D texture1;

void main() {
  // ambient
  vec3 ambient = light.ambient * material.ambient *
                 vec3(texture(material.diffuseMap, TexCoord));

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse =
      light.brightness * diff * vec3(texture(material.diffuseMap, TexCoord));

  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflection = reflect(-lightDir, norm);
  float spec =
      pow(max(dot(viewDir, reflection), 0.0f), material.shininess * 128.0f);
  vec3 specular = light.brightness * (spec * material.specular);

  FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
