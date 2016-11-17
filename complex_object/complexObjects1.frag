#version 330


in Data{
	vec4 color;
}In;

out vec4 color;

void main()
{
	// output the colour
	color = In.color; 
	//gl_FragColor=vec4(1.0,0.0,0.0,0.0);
}