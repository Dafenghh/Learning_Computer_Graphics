#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 pointPosition;
uniform vec3 pointColor;
out vec3 ourColor;

void main() {
   gl_Position = vec4(aPos + pointPosition, 1.0f, 1.0f);
    ourColor = pointColor;
}