#version 100

varying mediump vec2 texcoord;
varying lowp vec4 v_color;
uniform mediump sampler2D texture;
uniform mediump float hazardTime;
varying mediump vec2 vertpos;

void main() {
	gl_FragColor = texture2D(texture, texcoord*vec2(0.75, 0.75)) * vec4(0.65, 0.8, 0.65, -0.8/2.0 * (cos(3.141592*v_color.a/1.0) - 1.0) + 0.0);
}
