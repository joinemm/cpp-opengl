#version 330 core

layout(location = 0) in vec3 aPos;

void main() {
  // just return the positions of the vertex
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}
