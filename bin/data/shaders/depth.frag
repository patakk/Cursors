#version 150
in vec3 vecNormal;

in float vDepth;

out vec4 fragColor;

// these are passed in from OF programmable renderer
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 normalMatrix;

void main(){
    vec3 light = vec3(0.5, 0.2, 1.0);
    light = normalize(light);

    // dot product
    float dProd = max(0.0, dot(vecNormal, light));

    //color
    vec4 color = vec4(vec3(vDepth), 1);
    vec4 col = vec4( vec3( dProd ) * vec3( color ), 1.0 );
    fragColor = color;
}