#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;

const float minRad=5;
const float radius=25;
const float elevation=40;

const float x=100;
const float z=100;


in vec4 vtxPos;
in vec4 vtxCol;

 
out Data{
	vec4 color;
} Out;

float circularWave(vec3 p, vec3 centre,	float waveLength, float amplitude,float frequency,float displacement)
{

	float height,d;
	float dz, dx;
	
	dz = p.z - centre.z;
	dx = p.x - centre.x;
	d = sqrt(dz*dz + dx*dx);
	
	 height = amplitude*sin(waveLength*d+frequency+displacement);
	if (d==0){
		height=-5;
	}
	return(height);
}


void main(){
	
	gl_Position = projection * view * model * vtxPos;
	vec4 worldPos=model*vtxPos;

	float dist=sqrt(pow(x-worldPos.x,2)+pow(z-worldPos.z,2));
	if (dist<radius){
		//linear interpolation
		worldPos.y=elevation+dist*(0-elevation)/radius;
	}
	if (dist<minRad){
		worldPos.y=worldPos.y+circularWave(vec3(worldPos.xyz), vec3(x,0,z),1, 2, 0.1,10);
	}

	gl_Position = projection*view*worldPos;
	// set the colour
	Out.color = vtxCol;
}