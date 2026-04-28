#shader vertex
#version 330 core
layout (location=0) in vec3 aPosition;
layout (location=1) in vec2 aTextCoordinate;

out vec3 outColor;
out vec2 outTextureCoordinates;

uniform mat4 transform;

void main()
{
gl_Position = transform * vec4(aPosition, 1.0f);
outTextureCoordinates = aTextCoordinate;
};


#shader fragment
#version 330 core
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 outTextureCoordinates;
in vec3 outColor;
out vec4 color;
void main()
{
color = mix(
    texture(texture1, outTextureCoordinates),
    texture(texture2, outTextureCoordinates),
    0.2);
};