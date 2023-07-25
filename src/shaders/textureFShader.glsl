#version 330 core

in vec2 vertexUV;
out vec4 FragColor;

uniform vec3 objectColor;
uniform sampler2D textureSampler;
void main()
{
vec4 textureColor=texture(textureSampler,vertexUV);
   FragColor = textureColor*vec4(objectColor.r, objectColor.g,objectColor.b, 1.0f);
}