#version 330 core
in vec3 OurColorFromPos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(OurColorFromPos, 1.0f);
}