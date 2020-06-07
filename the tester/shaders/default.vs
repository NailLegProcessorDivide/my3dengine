#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MSM = mat4(1);
void main(){
  gl_Position = MSM*vec4(vertexPosition_modelspace,1);
  //gl_Position.w = 1.0;
}