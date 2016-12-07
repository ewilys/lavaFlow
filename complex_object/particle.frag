#version 430 


in vec4 vColour; // the vertex colour
in float Temp;

uniform sampler2D partSampler;
const float Tmax=1;

void main(void)
{
		float x,y;
		if (Temp==0){
			gl_FragColor = vec4(0,0,0,1);
		}
		else{
			x=3*Temp/(8*Tmax);
			y=1-3*Temp/(8*Tmax);
			vec2 textCoord=vec2(x,y);
			gl_FragColor = texture2D(partSampler,textCoord);
		}
}
