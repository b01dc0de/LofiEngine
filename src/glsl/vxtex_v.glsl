#version 330

uniform mat4 MVP;

in vec3 vPos;
in vec2 vUV;

out vec2 uv;

void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    uv = vUV;
}
