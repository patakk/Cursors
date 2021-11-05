#version 430

uniform sampler2DRect tex0;
uniform sampler2DRect flow;
uniform float blurAmnt;
uniform vec2 res;

in vec2 texCoordVarying;
out vec4 outputColor;

// Gaussian weights from http://dev.theomader.com/gaussian-kernel-calculator/

float unity_noise_randomValue (vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233)))*43758.5453);
}

 float unity_noise_interpolate (float a, float b, float t)
{
    return (1.0-t)*a + (t*b);
}

float unity_valueNoise (vec2 uv)
{
    vec2 i = floor(uv);
    vec2 f = fract(uv);
    f = f * f * (3.0 - 2.0 * f);

    uv = abs(fract(uv) - 0.5);
    vec2 c0 = i + vec2(0.0, 0.0);
    vec2 c1 = i + vec2(1.0, 0.0);
    vec2 c2 = i + vec2(0.0, 1.0);
    vec2 c3 = i + vec2(1.0, 1.0);
    float r0 = unity_noise_randomValue(c0);
    float r1 = unity_noise_randomValue(c1);
    float r2 = unity_noise_randomValue(c2);
    float r3 = unity_noise_randomValue(c3);

    float bottomOfGrid = unity_noise_interpolate(r0, r1, f.x);
    float topOfGrid = unity_noise_interpolate(r2, r3, f.x);
    float t = unity_noise_interpolate(bottomOfGrid, topOfGrid, f.y);
    return t;
}

float noise(vec2 UV, float Scale)
{
    float t = 0.0;

    float freq = pow(2.0, float(0));
    float amp = pow(0.5, float(3-0));
    t += unity_valueNoise(vec2(UV.x*Scale/freq, UV.y*Scale/freq))*amp;

    freq = pow(2.0, float(1));
    amp = pow(0.5, float(3-1));
    t += unity_valueNoise(vec2(UV.x*Scale/freq, UV.y*Scale/freq))*amp;

    freq = pow(2.0, float(2));
    amp = pow(0.5, float(3-2));
    t += unity_valueNoise(vec2(UV.x*Scale/freq, UV.y*Scale/freq))*amp;

    return t;
}

vec4 blur13(sampler2DRect image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = texture2DRect(image, uv) * 0.18;
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(2.2941176470588234) * direction;
  vec2 off3 = vec2(3.2941176470588234) * direction;
  vec2 off4 = vec2(4.2941176470588234) * direction;
  vec2 off5 = vec2(5.176470588235294) * direction;
  color += texture2DRect(image, uv + (off1 / resolution)) * 0.15;
  color += texture2DRect(image, uv - (off1 / resolution)) * 0.15;
  color += texture2DRect(image, uv + (off2 / resolution)) * 0.12;
  color += texture2DRect(image, uv - (off2 / resolution)) * 0.12;
  color += texture2DRect(image, uv + (off3 / resolution)) * 0.08;
  color += texture2DRect(image, uv - (off3 / resolution)) * 0.08;
  color += texture2DRect(image, uv + (off4 / resolution)) * 0.04;
  color += texture2DRect(image, uv - (off4 / resolution)) * 0.04;
  color += texture2DRect(image, uv + (off5 / resolution)) * 0.02;
  color += texture2DRect(image, uv - (off5 / resolution)) * 0.02;
  return color;
}

vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec2 flowDir = texture2DRect(flow, texCoordVarying.xy).xy;
    float ang = flowDir.x*2*3.14159-3.14159;
    float vel = flowDir.y;
    float vx = vel*cos(ang);
    float vy = vel*sin(ang);
    vec2 blurDir = vec2(vx, vy);


    //flow_dir = vec2(-flow_dir.y, flow_dir.x);
    vec4 blurred = blur13(tex0, texCoordVarying.xy, vec2(1.,1.), blurDir*4);
    //vec2 pos = vec2(texCoordVarying.xy);
    //float nz = .4*noise(pos, 0.02) + .6*noise(pos, 0.1);
    //color = vec4(nz, nz, nz, 1.0);
    
    vec3 hsv = vec3(flowDir.x, vel, vel);
    vec3 rgb = hsv2rgb(hsv);
    //outputColor = texture2DRect(tex0, texCoordVarying.xy);
    outputColor = blurred;
    //outputColor = vec4(rgb, 1.0);
    //outputColor = vec4(flow_dir, 0., 1.);
     
}
