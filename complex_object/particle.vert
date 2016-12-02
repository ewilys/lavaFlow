#version 330



in vec3 vtxPos;
in vec4 vtxColour;

uniform mat4 view;
uniform mat4 projection ;
const float minRad=5;
const float radius=25;
const float elevation=40;


uniform vec3 center;

// The output is a colour that is passed on to the fragment shader

out vec4 vColour;


float circularWave(vec3 p, vec3 centre,	float b)
{

	float height,d;
	float dz, dx;
	
	dz = p.z - centre.z;
	dx = p.x - centre.x;
	d = sqrt(dz*dz + dx*dx);
	
	 height = -sqrt((1+b*b)/(1+b*b*cos(d)*cos(d)))*cos(d);
	// height = -cos(d);

	return(height);
}



void main(void)

{
	
	vec4 worldPos=vec4(vtxPos,1.0);

	float dist=sqrt(pow(center.x-worldPos.x,2)+pow(center.z-worldPos.z,2));
	if (dist<radius && dist>minRad){
		//linear interpolation
		worldPos.y=elevation+dist*(0-elevation)/radius;
	}
	if (dist<=minRad){
		worldPos.y=(elevation+minRad*(0-elevation)/radius)+circularWave(vec3(worldPos.xyz),center,1.5);
	}


	vColour = vtxColour;
	gl_Position = projection*view*worldPos;

}
