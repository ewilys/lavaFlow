#version 330

// Vertex Shader example
// The variable vtxPos will contain the vertex position (vertex geometry).  Note that there are different ways to declare it.  Here I use “attribute in” to make it clear that it is an input to the shader and it is an attribute of the vertex.   The declaration could have also been “attribute” which is commented out below.
in vec3 vtxPos;

// This is the colour attribute of the vertex.  Note that here only the qualifier “attribute” is used.
in vec4 vtxColour;
uniform mat4 view;
uniform mat4 projection ;

// The output is a colour that is passed on to the fragment shader

out vec4 vColour;

void main(void)

{
	
	vColour = vtxColour;
	gl_Position = projection*view*vec4(vtxPos,1.0);

}
