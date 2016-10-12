#version 100

varying mediump vec2 texcoord;
varying lowp vec4 v_color;
uniform mediump sampler2D texture;
uniform lowp vec4 drProgress;

void main() {
	gl_FragColor = mix(texture2D(texture, texcoord),
		vec4(0.2, 0.3, 0.8, 0.8) + drProgress, 0.8);
}
