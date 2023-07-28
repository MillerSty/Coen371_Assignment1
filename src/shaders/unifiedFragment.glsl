#version 330 core

in vec2 vertexUV;
out vec4 FragColor;

uniform vec3 objectColor;
uniform sampler2D textureSampler;

uniform bool shouldApplyTexture;
uniform bool shouldApplyShadows;

const float PI = 3.1415926535897932384626433832795;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightDirection;

const float shadingAmbientStrength = 0.6;
const float shadingDiffuseStrength = 0.6;
const float shadingSpecularStrength = 0.6;

uniform float lightCutoffOuter;
uniform float lightCutoffInner;
uniform float lightNearPlane;
uniform float lightFarPlane;

uniform vec3 viewPosition;

in vec4 gl_FragCoord;

uniform sampler2D shadowMap;

in vec3 fragmentPosition;
in vec4 fragmentPositionLightSpace;
in vec3 fragmentNormal;

vec3 ambientColor(vec3 lightColorArg) {
    return shadingAmbientStrength * lightColorArg;
}

vec3 diffuseColor(vec3 lightColorArg, vec3 lightPositionArg) {
    vec3 lightDirection = normalize(lightPositionArg - fragmentPosition);
    return shadingDiffuseStrength * lightColorArg * max(dot(normalize(fragmentNormal), lightDirection), 0.0f);
}

float shadowScalar() {
    // this function returns 1.0 when the surface receives light, and 0.0 when it is in a shadow
    // perform perspective divide
    vec3 normalizedDeviceCoordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
    // transform to [0,1] range
    normalizedDeviceCoordinates = normalizedDeviceCoordinates * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragment_position_light_space as coords)
    float closestDepth = texture(shadowMap, normalizedDeviceCoordinates.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = normalizedDeviceCoordinates.z;
    // check whether current frag pos is in shadow
    float bias = 0.003;  // bias applied in depth map: see shadow_vertex.glsl
    return ((currentDepth - bias) < closestDepth) ? 1.0 : 0.0;
}

float spotlightScalar() {
    float theta = dot(normalize(fragmentPosition - lightPosition), lightDirection);

    if(theta > lightCutoffInner) {
        return 1.0;
    } else if(theta > lightCutoffOuter) {
        return (1.0 - cos(PI * (theta - lightCutoffOuter) / (lightCutoffInner - lightCutoffOuter))) / 2.0;
    } else {
        return 0.0;
    }
}

vec3 specularColor(vec3 lightColorArg, vec3 lightPositionArg) {
    vec3 lightDirection = normalize(lightPositionArg - fragmentPosition);
    vec3 viewDirection = normalize(viewPosition - fragmentPosition);
    vec3 reflectLightDirection = reflect(-lightDirection, normalize(fragmentNormal));
    return shadingSpecularStrength * lightColorArg * pow(max(dot(reflectLightDirection, viewDirection), 0.0f),32);
}

void main()
{
    vec4 fragColor = vec4(objectColor, 1.0);

    if (shouldApplyTexture){
        vec4 textureColor=texture(textureSampler,vertexUV);
        fragColor = textureColor * fragColor;
    }

    if (shouldApplyShadows) {
        gl_FragDepth = gl_FragCoord.z;
        fragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
    }
    else{
        vec3 ambient = vec3(0.0f);
        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f);

        float scalar = shadowScalar() * spotlightScalar();
        ambient = ambientColor(lightColor);
        diffuse = 1.0f * diffuseColor(lightColor, lightPosition);
        specular = 1.0f * specularColor(lightColor, lightPosition);

        vec3 color = specular + diffuse + ambient;

        fragColor = vec4(color, 1.0f) * fragColor;
    }

    FragColor = fragColor;
}
