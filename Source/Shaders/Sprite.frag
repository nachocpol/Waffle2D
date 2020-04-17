#version 330

in vec2 vTexCoord;

out vec4 oColor;

uniform vec4 uTint;
uniform sampler2D uImage;

void main()
{
    vec3 img = textureLod(uImage, vTexCoord, 0.0).rgb; // Texture is sRGB!
    oColor.rgb = img * uTint.rgb;
    oColor.rgb = pow(oColor.rgb,vec3(0.4545)); // Gamma correction.
    
    oColor.a = 1.0; // meh
}