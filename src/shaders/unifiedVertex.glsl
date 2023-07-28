#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexture;


uniform vec3 objectColor;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 lightViewProjMatrix = mat4(1.0);

uniform bool shouldApplyShadows;

out vec2 vertexUV;
out vec3 fragmentNormal;
out vec3 fragmentPosition;
out vec4 fragmentPositionLightSpace;
out vec3 Normal;
flat out vec3 theColor;
void main()
{
    
    
	if (shouldApplyShadows) {
        gl_Position = lightViewProjMatrix * worldMatrix * vec4(aPos, 1.0);
    }
    else {
        theColor = objectColor;
        Normal = aNorm;
    mat4 modelViewProjection= projectionMatrix * viewMatrix * worldMatrix;
    gl_Position = modelViewProjection * vec4(aPos, 1.0f);
    fragmentNormal =  mat3(transpose(inverse(worldMatrix))) *aNorm;
    fragmentPosition = (worldMatrix * vec4(aPos, 1.0f)).xyz;
    fragmentPositionLightSpace = lightViewProjMatrix * vec4(fragmentPosition, 1.0);
    vertexUV=aTexture;
    }
    //
    
}