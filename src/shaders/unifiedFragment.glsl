#version 330 core

in vec4 gl_FragCoord;
in vec2 vertexUV;

in vec3 fragmentPosition;
in vec4 fragmentPositionLightSpace;
in vec3 fragmentNormal;

out vec4 FragColor;

uniform vec4 objectColor;
uniform sampler2D textureSampler;

uniform bool shouldApplyTexture;
uniform bool shouldApplyShadows;
uniform bool shouldApplySpLight;
uniform bool shouldApplyMLight;


//so directional light has colour, ambInt,diffInt, direction
//      Plight has color, ambInt,diffInt,position, constants
//      Slight has color, ambInt,diffInt, direction, edge?
uniform vec3 mlightColor;
uniform vec3 mlightPosition;
uniform vec3 mlightDirection;

uniform float dAmb;
uniform float dDiff;
//
//uniform vec3 splightColor;
//uniform vec3 splightPosition;
//uniform vec3 splightDirection;
//uniform float spAmb;
//uniform float spDiff;
//uniform vec3 splightPosition;

//uniform vec3 lightColor;
//uniform vec3 lightPosition;
//uniform vec3 lightDirection;

const float shadingAmbientStrength = .50f;
const float shadingDiffuseStrength = 0.3;
const float shadingSpecularStrength = 0.3;

//uniform float lightCutoffOuter;
//uniform float lightCutoffInner;

uniform vec3 viewPosition;
uniform sampler2D shadowMap;

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

const int MAX_SPOT_LIGHTS = 3;
struct SpotLight {//vec3's~!!!! https://learnopengl.com/Lighting/Materials
    vec3 lightColor;
    vec3 lightPosition;
    vec3 lightDirection;
    float Amb;
    float Diff;
    float lightCutoffInner;
    float lightCutoffOuter;
};
uniform int spotlightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec3 ambientColor() {
    return vec3(objectColor).xyz  ;
}
vec3 diffuseColor(vec3 Position) { //times in light color
    vec3 lightDir = normalize(Position - fragmentPosition);
    return  vec3(objectColor).xyz *  max(dot(normalize(fragmentNormal), lightDir), 0.0f);
}

vec3 specularColor(vec3 Position) { //times in light color
    vec3 lightDir = normalize(Position - fragmentPosition);
    vec3 viewDir = normalize(viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, normalize(fragmentNormal));
    return vec3(  pow(max(dot(reflectDir, viewDir), 0.0f),32));
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
    float bias = .0001;  // bias applied in depth map: see shadow_vertex.glsl
    return ((currentDepth - bias) < closestDepth) ? 1.0 : 0.0;
}

float spotlightScalar(SpotLight sp) {
    float theta = dot(normalize(fragmentPosition - sp.lightPosition), sp.lightDirection);

    if (theta > sp.lightCutoffInner) {
        return 1.0;
    }
    else if (theta > sp.lightCutoffOuter) {
        return (1.0 - cos(3.14f * (theta - sp.lightCutoffOuter) / (sp.lightCutoffInner - sp.lightCutoffOuter))) / 2.0;
    }
    else {
        return 0.0;
    }
}
//
vec3 calcSpotlightColor() {
    vec3 color=vec3(0.0f, 0.0f, 0.0f);
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    for (int i = 0; i < spotlightCount; i++) {
    ambient = ambientColor() * spotLights[i].Amb;
    diffuse = diffuseColor(spotLights[i].lightPosition) * spotLights[i].lightColor * spotLights[i].Diff;
    specular = specularColor(spotLights[i].lightPosition) * spotLights[i].lightColor;
    float scalar = shadowScalar() * spotlightScalar(spotLights[i]);
        color+= (ambient + (diffuse + specular) * scalar);

    }
    return color;
   // return ambientColor() * spAmb +
    //  diffuseColor() * splightColor * spDiff + 
    //     specularColor() * splightColor)*
    //     shadowScalar() * spotlightScalar();

}

//vec3 calcSpotlightColor() {
//    return ambientColor()*splightColor * spAmb + (diffuseColor() * splightColor * spDiff + specularColor()) ;
//
//}
vec3 calcMainLight() {
    return ambientColor() * dAmb + (diffuseColor(mlightPosition) * mlightColor* dDiff + specularColor(mlightPosition)*mlightColor)  * shadowScalar();

}
void main()
{
    vec3 fragColor = vec3(objectColor.xyz); 

    if (shouldApplyShadows) {
        fragColor = vec3(gl_FragCoord.z);
    }
    else{
        vec3 ambient = vec3(0.0f);
        vec3 diffuse = vec3(0.0f);
        vec3 specular = vec3(0.0f); 

        //point light? can we programatically set these somehow -> uniforms?
        float constantPoint = .50f;// for high //1 for low height
        float linearPoint = .001932f; //3 for low height
        float quadPoint = .0013f; //3 for low height      

        vec3 directionPoint = fragmentPosition - mlightPosition;
        float distancePoint = length(directionPoint);
        float attenuation = (quadPoint * distancePoint * distancePoint) + (linearPoint * distancePoint) + constantPoint;

        float shadow = shadowScalar();
        //float spotlight = spotlightScalar();
        float scalar = shadow;

        vec3 color=vec3(0, 0, 0);
        ambient = ambientColor()*shadingAmbientStrength;
        diffuse = diffuseColor(mlightPosition) * shadingDiffuseStrength;
        specular = specularColor(mlightPosition) * shadingSpecularStrength;
        //vec3 check = calcSpotlightColor();
        vec3 checkMain = calcMainLight();
        //vec3 color = ambient + (specular + diffuse) *(scalar)*(1/attenuation)+ ambient + (specular + diffuse) * (scalar) * spotlight;
        //vec3 color = calcMainLight() * (1.0f / attenuation) + calcSpotlightColor();
        //calcMainLight() * (1.0f / attenuation) +
     
        
        if (shouldApplyMLight)
            color += calcMainLight() * (1.0f / attenuation);

        if (shouldApplySpLight)
            color += calcSpotlightColor();

        if (!shouldApplyMLight && !shouldApplySpLight)
            color += ambient;
        fragColor = color;
        if (shouldApplyTexture) {
            vec3 textureColor = texture(textureSampler, vertexUV).xyz;
            fragColor = textureColor * fragColor;
        }
    }
    FragColor = vec4(fragColor.x,fragColor.y,fragColor.z,objectColor.a);
}
