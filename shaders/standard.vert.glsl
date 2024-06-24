#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  //
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  FragPos = vec3(model * vec4(aPos, 1.0));
  TexCoord = aTexCoord;

  // For learning purposes this is fine, but for an efficient application
  // you'll likely want to calculate the normal matrix on the CPU and
  // send it to the shaders via a uniform before drawing
  Normal = mat3(transpose(inverse(model))) * aNormal;
}
