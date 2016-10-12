#version 100

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texcoord;
uniform vec2 u_scroll;
uniform mat4 projection;
varying lowp vec4 v_color;
varying mediump vec2 texcoord;
varying vec2 vertpos;

void main() {
	mat4 scroll_translate = mat4(
		vec4(1.0, 0.0, 0.0, -u_scroll.x),
		vec4(0.0, 1.0, 0.0, -u_scroll.y),
		vec4(0.0, 0.0, 1.0, 0.0),
		vec4(0.0, 0.0, 0.0, 1.0)
	);
	gl_Position = projection * (a_position * scroll_translate);
	v_color = a_color;
	texcoord = a_texcoord;
	vertpos = (projection * a_position).xy;
}
