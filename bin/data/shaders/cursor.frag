uniform sampler2D tex0;

void main () {
    vec2 xy = gl_TexCoord[0].st;
    xy = vec2(xy.x, 1. - xy.y);
    vec4 color = texture(tex0, xy);
    
    gl_FragColor = vec4(color);
}