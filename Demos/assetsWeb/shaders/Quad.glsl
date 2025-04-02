#type vertex
#version 300 es
precision mediump float;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_VP;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_VP * vec4(a_Position, 1.0);
}

#type fragment
#version 300 es
precision mediump float;

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[16];

void main()
{
	// stupid glsl es
	switch (int(v_TexIndex))
	{
    case 0: color = texture(u_Textures[0], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 1: color = texture(u_Textures[1], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 2: color = texture(u_Textures[2], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 3: color = texture(u_Textures[3], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 4: color = texture(u_Textures[4], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 5: color = texture(u_Textures[5], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 6: color = texture(u_Textures[6], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 7: color = texture(u_Textures[7], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 8: color = texture(u_Textures[8], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 9: color = texture(u_Textures[9], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 10: color = texture(u_Textures[10], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 11: color = texture(u_Textures[11], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 12: color = texture(u_Textures[12], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 13: color = texture(u_Textures[13], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 14: color = texture(u_Textures[14], v_TexCoord * v_TilingFactor) * v_Color; break;
    case 15: color = texture(u_Textures[15], v_TexCoord * v_TilingFactor) * v_Color; break;
    default: color = vec4(0.0); break;
	}
}