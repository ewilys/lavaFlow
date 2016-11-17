#version 330

uniform sampler2D texSampler;

in Data{
	vec2 texCoord;
	vec4 color;
}In;

out vec4 color;

void main()
{
	// output the colour
	color = In.color; 
	
	//colour=texture2D(texSampler,In.texCoord);
	//gl_FragColor=vec4(1.0,0.0,0.0,0.0);
}