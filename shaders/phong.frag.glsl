#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture1;

void main() {
  float ambientStrength = 0.1f;
  float specularStrength = 0.5f;
  vec3 ambient = ambientStrength * lightColor;
  vec3 norm = normalize(Normal);
  // from object towards the light source
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = diff * lightColor;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflection = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflection), 0.0f), 32);
  vec3 specular = specularStrength * spec * lightColor;
  vec3 final = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(final, 1.0f);
  // * texture(texture1, TexCoord);
}
