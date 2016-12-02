#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;

const float minRad=5;
const float radius=25;
const float elevation=40;


uniform vec3 center;

in vec4 vtxPos;
in vec4 vtxCol;
in vec2 vtxCoord;

 
out Data{
	vec2 texCoord;
	vec4 color;
} Out;

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


void main(){
	
	//gl_Position = projection * view * model * vtxPos;
	vec4 worldPos=model*vtxPos;

	float dist=sqrt(pow(center.x-worldPos.x,2)+pow(center.z-worldPos.z,2));
	if (dist<radius && dist>minRad){
		//linear interpolation
		worldPos.y=elevation+dist*(0-elevation)/radius;
	}
	if (dist<=minRad){
		worldPos.y=(elevation+minRad*(0-elevation)/radius)+circularWave(vec3(worldPos.xyz),center,1.5);
	}

	gl_Position = projection*view*worldPos;
	// set the colour
	Out.texCoord = vtxCoord;
	Out.color = vtxCol;
}