#shader vertex
#version 330 core

in vec3 aPos;
void main()
{
gl_Position = vec4(aPos, 1.0f);
};

#shader fragment
#version 330 core

out vec4 color;
void main()
{
color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};