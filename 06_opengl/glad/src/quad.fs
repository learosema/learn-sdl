#version 410 core
in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

void main() {
    vec3 color = vec3(
        0.5 + 0.5 * sin(uTime + vUV.x * 6.2831),
        vUV.y,
        0.5 + 0.5 * cos(uTime + vUV.x * 6.2831)
    );
    FragColor = vec4(color, 1.0);
}
