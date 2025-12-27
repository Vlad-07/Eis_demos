//type vertex
#version 100
precision mediump float;

attribute vec3 a_Position;
attribute vec4 a_Color;

uniform mat4 u_VP;

varying vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_VP * vec4(a_Position, 1.0);
}


//type fragment
#version 100
precision mediump float;

varying vec4 v_Color;

void main()
{
	gl_FragColor = v_Color;
}