#version 410 core
layout (location = 0) in vec2 aPos;
out vec2 vUV;
out vec2 vPos;

void main() {
    vUV = aPos * 0.5 + 0.5; // -1..1 -> 0..1
    vUV.y = 1. - vUV.y;
    vPos = aPos;
    gl_Position = vec4(aPos, 0.0, 1.0);
}