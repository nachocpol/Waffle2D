#version 330

in vec2 vTexCoord;

out vec4 oColor;

uniform vec4 uTint;
uniform vec4 uScaleBias;
uniform sampler2D uImage;
uniform int uFontRender;

void main()
{
    vec2 curTexCoord = vTexCoord * uScaleBias.xy + uScaleBias.zw;

    vec4 img = textureLod(uImage, curTexCoord, 0.0); // Texture is sRGB!

    if(uFontRender == 0)
    {
        oColor.rgb = img.rgb * uTint.rgb;
        oColor.rgb = pow(oColor.rgb,vec3(0.4545)); // Gamma correction.
        oColor.a = uTint.a;
    }
    else
    {
        oColor.rgba = img.rrrr;
        oColor.rgb *= uTint.rgb;
        oColor.a *= uTint.a;
    }
    
    if(img.a < 0.9)
    {
        discard;
    }
}