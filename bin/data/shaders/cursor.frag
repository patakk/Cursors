uniform sampler2D tex0;

void main () {
    vec2 xy = gl_TexCoord[0].st;
    xy = vec2(xy.x, 1. - xy.y);
    vec4 color = texture2D(tex0, xy);
    
    gl_FragColor = vec4(1.,0.,0.,1.); // this works
    // gl_FragColor = color; // this doesn't work, it's black
}