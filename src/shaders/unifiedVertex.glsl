#version 330 core

// Layouts
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexture;

// Uniforms
uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 lightViewProjMatrix = mat4(1.0);
uniform bool shouldApplyShadows;

// Outputs
out vec2 vertexUV;
out vec3 fragmentNormal;
out vec3 fragmentPosition;
out vec4 fragmentPositionLightSpace;

void main()
{
    mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
    
	if (shouldApplyShadows) {
        gl_Position = lightViewProjMatrix * worldMatrix * vec4(aPos, 1.0);
    }
    else {
        gl_Position = modelViewProjection * vec4(aPos, 1.0);
    }

    fragmentNormal = transpose(inverse(mat3(worldMatrix))) * aNorm;
    fragmentPosition = vec3(worldMatrix * vec4(aPos, 1.0));
    fragmentPositionLightSpace = lightViewProjMatrix * vec4(fragmentPosition, 1.0);
    vertexUV = aTexture;
}
