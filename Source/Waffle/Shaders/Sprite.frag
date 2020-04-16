#version 330

in vec2 vTexCoord;
out vec4 oColor;

void main()
{
    oColor = vec4(vTexCoord,0.0,1.0);
}