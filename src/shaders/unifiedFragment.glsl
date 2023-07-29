#version 330 core




uniform vec3 objectColor;
 uniform sampler2D textureSampler;
 uniform sampler2D shadowMap;

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
 in vec2 vertexUV;
 in vec3 fragmentNormal;
 in vec3 fragmentPosition;
 in vec4 fragmentPositionLightSpace;
 in vec3 Normal;
 out vec4 FragColor;

//vec3 ambientColor(vec3 lightColorArg) {
//    return  lightColorArg *shadingAmbientStrength ;
//}
//
//vec3 diffuseColor(vec3 lightColorArg, vec3 lightPositionArg) {
//    vec3 lightDirection = normalize(lightPositionArg - fragmentPosition);
//    float diffuseFactor = max(dot(normalize(fragmentNormal), normalize(lightDirection)), 0.0f);
//    vec3 specularColour=vec3(0.0f, 0.0f, 0.0f);
//    if (diffuseFactor > 0.0f) {
//        vec3 fragToEye = normalize(viewPosition - fragmentPosition);
//        vec3 reflectedVertex = normalize(reflect(lightDirection, normalize(fragmentNormal)));
//       //
//        float specularFactor = dot(fragToEye, reflectedVertex);
//        if (specularFactor > 0.0f)
//        {
//            specularFactor = pow(specularFactor, shadingSpecularStrength);
//            specularColour = lightColorArg * specularFactor;
//        }
//
//    }
//
//    return  lightColorArg *shadingDiffuseStrength  * diffuseFactor ;
//}
//
//vec3 specularColor(vec3 lightColorArg, vec3 lightPositionArg) {
//    vec3 lightDirection = normalize(lightPositionArg - fragmentPosition);
//    vec3 viewDirection = normalize(viewPosition - fragmentPosition);
//    
//    vec3 reflectLightDirection = reflect(-lightDirection, normalize(fragmentNormal));
//    return shadingSpecularStrength * pow(max(dot(viewDirection, reflectLightDirection), 0.0f), 32) * lightColorArg;
//}
 //float shadow_scalar() {
 //    // this function returns 1.0 when the surface receives light, and 0.0 when it is in a shadow
 //    // perform perspective divide
 //    vec3 jawn = fragmentPositionLightSpace.xyz;
 //    float scalar= fragmentPositionLightSpace.w
 //    vec3 normalized_device_coordinates = vec3(fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w);
 //    // transform to [0,1] range
 //    normalized_device_coordinates = normalized_device_coordinates * 0.5 + 0.5;
 //    // get closest depth value from light's perspective (using [0,1] range fragment_position_light_space as coords)
 //    float closest_depth = texture(shadow_map, normalized_device_coordinates.xy).r;
 //    // get depth of current fragment from light's perspective
 //    float current_depth = normalized_device_coordinates.z;
 //    // check whether current frag pos is in shadow
 //    float bias = 0;  // bias applied in depth map: see shadow_vertex.glsl
 //    return ((current_depth - bias) < closest_depth) ? 1.0 : 0.0;
 //}

float spotlightScalar() {
    float theta = dot(normalize(fragmentPosition - lightPosition), lightDirection);

    if (theta > lightCutoffInner) {
        return 1.0;
    }
    else if (theta > lightCutoffOuter) {
        return (1.0 - cos(PI * (theta - lightCutoffOuter) / (lightCutoffInner - lightCutoffOuter))) / 2.0;
    }
    else {
        return 0.0;
    }
}

void main()
{
    vec3 norm = normalize(fragmentNormal);
    vec3 lightDir = normalize(lightPosition - fragmentPosition);
    vec3 viewDir = normalize(viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);
    FragColor = vec4(0, 0, 0, 0);
    vec4 fragColor = vec4(0, 0, 0, 0);


        if (shouldApplyShadows) {
          // gl_FragDepth = gl_FragCoord.z;
        FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
            
        }
        else{
        
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular = shadingSpecularStrength * spec * lightColor;        
        //diffuse
        float diff = max(dot(norm, viewDir), 0.0f);
        diffuse = shadingDiffuseStrength*diff * lightColor;
        //ambient and texture
        ambient = shadingAmbientStrength * lightColor;

            vec3 normalized_device_coordinates = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;
            normalized_device_coordinates = normalized_device_coordinates * 0.5 + 0.5;
             float closest_depth = texture(shadowMap, normalized_device_coordinates.xy).r;
             float current_depth = normalized_device_coordinates.z;
             // check whether current frag pos is in shadow
            float bias = .001;  // bias applied in depth map: see shadow_vertex.glsl
            float shadow = 0.0f;           
            shadow = ((current_depth - bias) < closest_depth) ? 1.0 : 0.0; 
             float spotlight = spotlightScalar();
             float scalar = (shadow);
                 

            vec3 result = (ambient + scalar * ( diffuse + specular ) ) * objectColor;
            FragColor =vec4(result,1.0f);
            //FragColor = vec4(color* theColor, 1.0f);
       }
        
   // }

   // FragColor = fragColor;
}
