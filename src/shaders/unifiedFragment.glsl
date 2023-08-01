#version 330 core

in vec2 vertexUV;
out vec3 FragColor;

uniform vec3 objectColor;
uniform sampler2D textureSampler;

uniform bool shouldApplyTexture;
uniform bool shouldApplyShadows;

const float PI = 3.1415926535897932384626433832795;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightDirection;

const float shadingAmbientStrength = 1.0f;
const float shadingDiffuseStrength = 0.5;
const float shadingSpecularStrength = 0.5;

uniform float lightCutoffOuter;
uniform float lightCutoffInner;

uniform vec3 viewPosition;

in vec4 gl_FragCoord;

uniform sampler2D shadowMap;

in vec3 fragmentPosition;
in vec4 fragmentPositionLightSpace;
in vec3 fragmentNormal;

uniform float materialSpec;

//i think there is something wrong with out Lighting
//material properties dont seem to have an affect
struct Material{//vec3's~!!!! https://learnopengl.com/Lighting/Materials

    float diffuseStrength;
    float specularStrength;
    float ambientStrength;
    float shininessStrength;

};

uniform Material mats;

vec3 ambientColor() {
    return objectColor * shadingAmbientStrength * mats.ambientStrength;
}

vec3 diffuseColor() {
    vec3 lightDir = normalize(lightPosition - fragmentPosition);
    return mats.diffuseStrength * shadingDiffuseStrength* objectColor * lightColor * max(dot(normalize(fragmentNormal), lightDir), 0.0f);
}

vec3 specularColor() {
    vec3 lightDir = normalize(lightPosition - fragmentPosition);
    vec3 viewDir = normalize(viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, normalize(fragmentNormal));
    return mats.specularStrength*shadingSpecularStrength * lightColor * pow(max(dot(reflectDir, viewDir), 0.0f), mats.shininessStrength);
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
    float bias = 0.00001f;  // bias applied in depth map: see shadow_vertex.glsl
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


void main()
{
    vec3 fragColor = objectColor; 

    if (shouldApplyShadows) {
        fragColor = vec3(gl_FragCoord.z);
    }
    else{
        vec3 ambient = vec3(0.0f);
        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f); 

        //point light? can we programmatically set these somehow -> uniforms?
        float constantPoint = 1.0f;//.50f for high //1 for low height
            float linearPoint = 3.0f;//  .001932;//3 for low height
            float quadPoint = 3.0f;//   .0013; //3 for low height      
        vec3 directionPoint = fragmentPosition - lightPosition;
        float distancePoint = length(directionPoint);
        float attenuation = quadPoint * distancePoint * distancePoint + linearPoint * distancePoint + constantPoint;

        float shadow = shadowScalar();
        float spotlight = spotlightScalar();
        float scalar = shadow* spotlight;

        ambient = ambientColor();
        diffuse = diffuseColor();
        specular = specularColor();

        vec3 color = ambient +  (specular + diffuse)*scalar / attenuation;
           fragColor = color;

        if (shouldApplyTexture) {
            vec3 textureColor = texture(textureSampler, vertexUV).xyz;
            fragColor = textureColor * fragColor;
        }
    }
    FragColor = fragColor;
}
