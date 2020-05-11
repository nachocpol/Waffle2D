#version 430

in vec2 vTexCoord;
in flat int vInstanceID;

out vec4 oColor;

uniform sampler2D uImage;
uniform int uFontRender;

struct InstanceData
{
    vec4 ScaleBias;
    vec4 Tint;
    mat3 Transform;
};
layout(std430, binding = 0) buffer InstanceBuffer 
{ 
    InstanceData uPerInstance[]; 
};

void main()
{
    vec4 scaleBias = uPerInstance[vInstanceID].ScaleBias;
    vec2 curTexCoord = vTexCoord * scaleBias.xy + scaleBias.zw;

    vec4 img = textureLod(uImage, curTexCoord, 0.0); // Texture is sRGB!

    if(uFontRender == 0)
    {
        oColor.rgb = img.rgb * uPerInstance[vInstanceID].Tint.rgb;
        oColor.rgb = pow(oColor.rgb,vec3(0.4545)); // Gamma correction.
        oColor.a = uPerInstance[vInstanceID].Tint.a;
    }
    else
    {
        oColor.rgba = img.rrrr;
        oColor.rgb *= uPerInstance[vInstanceID].Tint.rgb;
        oColor.a *= uPerInstance[vInstanceID].Tint.a;
    }
    
    if(img.a < 0.9)
    {
        discard;
    }
}