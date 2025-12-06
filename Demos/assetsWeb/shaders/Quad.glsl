//type vertex
#version 100
precision mediump float;

attribute vec3 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TexCoord;
attribute float a_TexIndex;
attribute float a_TilingFactor;

uniform mat4 u_VP;

varying vec4 v_Color;
varying vec2 v_TexCoord;
varying float v_TexIndex;
varying float v_TilingFactor;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_VP * vec4(a_Position, 1.0);
}

//type fragment
#version 100
precision mediump float;

varying vec4 v_Color;
varying vec2 v_TexCoord;
varying float v_TexIndex;
varying float v_TilingFactor;

uniform sampler2D u_Textures[16];

void main()
{
	vec2 uv = v_TexCoord * v_TilingFactor;
	vec4 color = vec4(1.0, 0.0, 1.0, 1.0);
		 if (v_TexIndex < 0.5)  color = texture2D(u_Textures[0], uv);
	else if (v_TexIndex < 1.5)  color = texture2D(u_Textures[1], uv);
	else if (v_TexIndex < 2.5)  color = texture2D(u_Textures[2], uv);
	else if (v_TexIndex < 3.5)  color = texture2D(u_Textures[3], uv);
	else if (v_TexIndex < 4.5)  color = texture2D(u_Textures[4], uv);
	else if (v_TexIndex < 5.5)  color = texture2D(u_Textures[5], uv);
	else if (v_TexIndex < 6.5)  color = texture2D(u_Textures[6], uv);
	else if (v_TexIndex < 7.5)  color = texture2D(u_Textures[7], uv);
	else if (v_TexIndex < 8.5)  color = texture2D(u_Textures[8], uv);
	else if (v_TexIndex < 9.5)  color = texture2D(u_Textures[9], uv);
	else if (v_TexIndex < 10.5) color = texture2D(u_Textures[10], uv);
	else if (v_TexIndex < 11.5) color = texture2D(u_Textures[11], uv);
	else if (v_TexIndex < 12.5) color = texture2D(u_Textures[12], uv);
	else if (v_TexIndex < 13.5) color = texture2D(u_Textures[13], uv);
	else if (v_TexIndex < 14.5) color = texture2D(u_Textures[14], uv);
	else if (v_TexIndex < 15.5) color = texture2D(u_Textures[15], uv);

	gl_FragColor = color * v_Color;
}