//TES transforms new vertices. Runs once for all output vertices

#version 410
layout(isolines) in; // Controls how tesselator creates new geometry

in vec3 teColour[]; // input colours

out vec3 Colour; // colours to fragment shader

uniform float controlPointsNumber;

void main()
{
    // gl_TessCoord variable lets us know where we are within the patch.
    // In this case, the primitive mode is isolines, so gl_TessCoord.x is distance along curve.
    // if the primitive mode were triangles, gl_TessCoord would be a barycentric coordinate.
    float u = gl_TessCoord.x;
	
    // order 1 bernstein basis
    float b0 = 1.0 - u;
    float b1 = u;

    // Just like bezier sum
    if (controlPointsNumber == 3)
        gl_Position = pow(b1, 2) * gl_in[0].gl_Position + 2 * (b1) * b0 * gl_in[1].gl_Position + pow(b0, 2) * gl_in[2].gl_Position;
    else if (controlPointsNumber == 4)
        gl_Position = pow(b0, 3) * gl_in[0].gl_Position + 3 * pow(b0, 2) * b1 * gl_in[1].gl_Position + 3 * b0 * pow(b1, 2) * gl_in[2].gl_Position + pow(b1, 3) * gl_in[3].gl_Position;

    // Determine colours for new points
    Colour 	= b0 * teColour[0] + 
	     	  b1 * teColour[1];
}



