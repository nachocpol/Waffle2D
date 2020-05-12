#version 430

in vec2 Position;
in vec2 TexCoord;

out vec2 vTexCoord;
out flat int vInstanceID;

uniform vec2 uProjection;

struct InstanceData
{
    vec4 ScaleBias;
    vec4 Tint;
    mat4 Transform;
};
layout(std430, binding = 0) buffer InstanceBuffer 
{ 
    InstanceData uPerInstance[]; 
};

void main()
{
    // https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations
    // If a vector is multiplied to a matrix from the left, the result corresponds
    //  to multiplying a row vector from the left to the matrix. This corresponds to multiplying a column
    //  vector to the transposed matrix from the right. Matrix are colum major.. but in my math lib they are row major! So this is useful
    
    vInstanceID = gl_InstanceID;    
    vTexCoord = TexCoord;
    vec2 worldPos = (vec3(Position, 1.0) * mat3(uPerInstance[gl_InstanceID].Transform)).xy;

    gl_Position = vec4(worldPos * uProjection, 1.0 ,1.0);
}