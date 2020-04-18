#version 330

in vec2 Position;
in vec2 TexCoord;

out vec2 vTexCoord;

uniform vec2 uProjection;
uniform mat3 uTransform;

void main()
{
    // https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations
    // If a vector is multiplied to a matrix from the left, the result corresponds
    //  to multiplying a row vector from the left to the matrix. This corresponds to multiplying a column
    //  vector to the transposed matrix from the right. Matrix are colum major.. but in my math lib they are row major! So this is useful
    
    vTexCoord = TexCoord;
    vec2 worldPos = (vec3(Position, 1.0) * uTransform ).xy;

    gl_Position = vec4(worldPos * uProjection, 1.0 ,1.0);
}