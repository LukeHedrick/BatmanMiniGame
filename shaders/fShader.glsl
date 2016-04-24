#version 430

uniform sampler2D myTexture;
in vec2 fragUV;

void main()
{
	gl_FragColor = texture(myTexture, fragUV);
}