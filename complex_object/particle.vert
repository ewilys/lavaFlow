#version 330



in vec3 vtxPos;
in vec4 vtxColour;
in float vtxTemp;

uniform mat4 view;
uniform mat4 projection ;
const float minRad=5;
const float radius=25;
const float elevation=40;


uniform vec3 center;

// The output is a colour that is passed on to the fragment shader

out vec4 vColour;
out float Temp;


float circularWave(vec3 p, vec3 centre,	float b)
{

	float height,d;
	float dz, dx;
	
	dz = p.z - centre.z;
	dx = p.x - centre.x;
	d = sqrt(dz*dz + dx*dx);
	
	 height = -sqrt((1+b*b)/(1+b*b*cos(d)*cos(d)))*cos(d);
	if (height<0)height=0;

	return(height);
}



void main(void)

{
	
	vec4 worldPos=vec4(vtxPos,1.0);

	float dist=sqrt(pow(center.x-worldPos.x,2)+pow(center.z-worldPos.z,2));
	if (dist<radius && dist>minRad){
		//linear interpolation
		worldPos.y=worldPos.y+elevation+dist*(0-elevation)/radius;
	}
	if (dist<=minRad){
		worldPos.y=worldPos.y+(elevation+minRad*(0-elevation)/radius)+circularWave(vec3(worldPos.xyz),center,1.5);
	}


	vColour = vtxColour;
	Temp=vtxTemp;
	gl_Position = projection*view*worldPos;

}
