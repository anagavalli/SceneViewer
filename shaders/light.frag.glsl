# version 330 core

in vec3 vertNormal;
in vec4 vertPos;

uniform mat4 modelview;

out vec4 fragColor;

uniform vec3 color;

const int numLights = 10; 
uniform bool enableLighting; // are we lighting at all (global).
uniform vec4 lightPosn[numLights]; // positions of lights
uniform vec4 lightColor[numLights]; // colors of lights
uniform int numUsed;               // number of lights used

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess;

vec4 computeLight (const in vec3 lightDir, const in vec4 lightColor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

    float nDotL = dot(normal, lightDir)  ;
    vec4 lambert = mydiffuse * lightColor * max (nDotL, 0.0) ;

    float nDotH = dot(normal, halfvec) ;
    vec4 phong = myspecular * lightColor * pow (max(nDotH, 0.0), myshininess) ;

    vec4 retval = lambert + phong ;
    return retval ;
}

void main (void) 
{       
    if (enableLighting) {
        vec4 finalcolor = vec4(0.0, 0.0, 0.0, 1.0);
        finalcolor = finalcolor + ambient + emission;

        // Compute normal, needed for shading.
        vec3 normal = normalize(mat3(transpose(inverse(modelview))) * vertNormal) ;

        for (int i = 0; i < numLights; i++) {
            if (i > numUsed) break;

            vec4 curLightPos = lightPosn[i];
            vec4 curLightColor = lightColor[i];

            vec4 modelPos = modelview * vertPos;
            vec3 pos3d = modelPos.xyz / modelPos.w; // Dehomogenize current location
            vec3 viewDir = normalize(-pos3d) ;

            // Directional light
            if (curLightPos.w == 0) {
                vec3 lightDir = normalize (curLightPos.xyz);
                vec3 halfVec = normalize (lightDir + viewDir);
                finalcolor = finalcolor + computeLight(lightDir, curLightColor, normal, halfVec, diffuse, specular, shininess);
            }
            // Point light
            else {
                vec3 lightPosition = curLightPos.xyz / curLightPos.w;
                vec3 lightDir = normalize (lightPosition - pos3d); // no attenuation
                vec3 halfVec = normalize (lightDir + viewDir);
                finalcolor = finalcolor + computeLight(lightDir, curLightColor, normal, halfVec, diffuse, specular, shininess);
            }
        }

        fragColor = finalcolor;
    } else {
        fragColor = vec4(color, 1.0f);
    }
}
