#version 330

in vec2 uv;

out vec4 fragment;

uniform sampler2D testTexture;

void main()
{
    fragment = texture(testTexture, uv);
}
