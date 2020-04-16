#version 330

in vec2 Position;
in vec2 TexCoord;
in vec4 Color0;

out vec2 vTexCoord;

uniform vec2 uProjection;
uniform mat3 uTransform;

void main()
{
    vTexCoord = TexCoord;
    vec2 worldPos = (uTransform * vec3(Position, 1.0)).xy;

    gl_Position = vec4(worldPos * uProjection, 1.0 ,1.0);
}