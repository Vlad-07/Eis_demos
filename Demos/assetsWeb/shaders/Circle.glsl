//type vertex
#version 100
precision mediump float;

attribute vec3 a_WorldPosition;
attribute vec3 a_LocalPosition;
attribute vec4 a_Color;
attribute float a_Thickness;
attribute float a_Fade;

uniform mat4 u_VP;

varying vec3 v_LocalPosition;
varying vec4 v_Color;
varying float v_Thickness;
varying float v_Fade;

void main()
{
	v_LocalPosition = a_LocalPosition;
	v_Color = a_Color;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
	gl_Position = u_VP * vec4(a_WorldPosition, 1.0);
}


//type fragment
#version 100
precision mediump float;

varying vec3 v_LocalPosition;
varying vec4 v_Color;
varying float v_Thickness;
varying float v_Fade;

void main()
{
	float distance = 1.0 - length(v_LocalPosition);
	float circleAlpha = smoothstep(0.0, v_Fade, distance);
	circleAlpha *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);

	if (circleAlpha == 0.0)
		discard;

	gl_FragColor = v_Color;
	gl_FragColor.a *= circleAlpha;
}