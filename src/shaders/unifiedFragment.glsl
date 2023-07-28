#version 330 core

in vec2 vertexUV;
out vec4 FragColor;


uniform sampler2D textureSampler;

uniform bool shouldApplyTexture;
uniform bool shouldApplyShadows;

const float PI = 3.1415926535897932384626433832795;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightDirection;

uniform float shadingAmbientStrength ;
uniform float shadingDiffuseStrength ;
uniform float shadingSpecularStrength;

uniform float lightCutoffOuter;
uniform float lightCutoffInner;
uniform float lightNearPlane;
uniform float lightFarPlane;

uniform vec3 viewPosition;

in vec4 gl_FragCoord;

uniform sampler2D shadowMap;
flat in vec3 theColor;
in vec3 fragmentPosition;
in vec4 fragmentPositionLightSpace;
in vec3 fragmentNormal;
in vec3 Normal;
vec3 ambientColor(vec3 lightColorArg) {
    return  lightColorArg *shadingAmbientStrength ;
}

vec3 diffuseColor(vec3 lightColorArg, vec3 lightPositionArg) {
    vec3 lightDirection = normalize(lightPositionArg - fragmentPosition);
    float diffuseFactor = max(dot(normalize(fragmentNormal), normalize(lightDirection)), 0.0f);
    vec3 specularColour=vec3(0.0f, 0.0f, 0.0f);
    if (diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(viewPosition - fragmentPosition);
        vec3 reflectedVertex = normalize(reflect(lightDirection, normalize(fragmentNormal)));
       //
        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, shadingSpecularStrength);
            specularColour = lightColorArg * specularFactor;
        }

    }

    return  lightColorArg *shadingDiffuseStrength  * diffuseFactor ;
}

vec3 specularColor(vec3 lightColorArg, vec3 lightPositionArg) {
    vec3 lightDirection = normalize(lightPositionArg - fragmentPosition);
    vec3 viewDirection = normalize(viewPosition - fragmentPosition);
    
    vec3 reflectLightDirection = reflect(-lightDirection, normalize(fragmentNormal));
    return shadingSpecularStrength * pow(max(dot(viewDirection, reflectLightDirection), 0.0f), 32) * lightColorArg;
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
    float bias = 0.001;  // bias applied in depth map: see shadow_vertex.glsl

    float shadow = (currentDepth-bias) < closestDepth ? 1.0 : 0.0;
    return shadow;
        //((currentDepth - bias) < closestDepth) ? 1.0 : 0.0;
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
   // vec4 fragColor = vec4(objectColor, 1.0);   

    if (shouldApplyShadows) {
        float depthValue = texture(shadowMap, vertexUV).r;
        FragColor = vec4(vec3(depthValue), 1.0);
    }
    else{
        vec3 ambient = vec3(0.0f);
        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f);

        float scalar = shadowScalar() * spotlightScalar();
        ambient = ambientColor(lightColor);
        diffuse =  diffuseColor(lightColor, lightPosition);
        specular =  specularColor(lightColor, lightPosition);


        if (shouldApplyTexture) {

            float brightness = dot(fragmentNormal, normalize(lightPosition - fragmentPosition)) / (length(normalize(lightPosition - fragmentPosition)) * length(fragmentNormal));
            brightness = clamp(brightness, 0, 1);


            vec3 result = (ambient + (1.0f-shadowScalar())*(diffuse+specular) ) * theColor;
            vec4 textureColor = texture(textureSampler, vertexUV);
            FragColor = textureColor * vec4(result, 1.0f);
        }
        else {

            vec3 color =  diffuse + ambient;
            FragColor = vec4(clamp(normalize(Normal),0,1),1.0f);
            FragColor = vec4(color* theColor, 1.0f);
        }
        
    }

   // FragColor = fragColor;
}
