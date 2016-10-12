#version 100

varying mediump vec2 texcoord;
varying lowp vec4 v_color;
uniform mediump sampler2D texture;
uniform bool noTexture;

void main() {
	if (noTexture)
		gl_FragColor = v_color;
	else
		gl_FragColor = texture2D(texture, texcoord) * v_color;
}
