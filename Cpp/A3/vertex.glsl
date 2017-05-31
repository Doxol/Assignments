// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec3 VertexColour;

// output to be interpolated between vertices and passed to the fragment stage
out vec3 tcColour;

uniform float offset;
uniform float offsetY;
uniform float windowScale;
uniform float spookyEffect;

void main()
{
	vec2 scaledPos = windowScale * VertexPosition;

	scaledPos.x -= (-offset);
	scaledPos.y -= 0.1;

	vec3 color = VertexColour;

	if (spookyEffect == 1){
		scaledPos.x -= offset * 2;
		/*
		if (scaledPos.x < 0)
			scaledPos.x += (scaledPos.y * scaledPos.y * 2.f);
		else
			scaledPos.x -= (scaledPos.y * scaledPos.y * 2.f);
		*/
		scaledPos.y += (offsetY);		
		scaledPos.x *= 0.1 - scaledPos.y;

	}
	else if (spookyEffect == 2){
		scaledPos.y += (offsetY);
		if (scaledPos.x >= 0){
			scaledPos.x *= 0.5f - 2.f * (scaledPos.y * 0.2f);
			scaledPos.y *= 0.5f - 2.f * (scaledPos.y * 0.2f);
		}
		else if (scaledPos.x <= 0){
			scaledPos.x *= 0.5f - 2.f * (scaledPos.y * 0.2f);
			scaledPos.y *= 0.5f - 2.f * (scaledPos.y * 0.2f);
		}
	}
	else if (spookyEffect == 3){
		scaledPos.y += (offsetY);
		if (scaledPos.x >= 0){
			scaledPos.x *= 0.5f - 2.f * (abs(scaledPos.y) * 0.2f);
		}
		else if (scaledPos.x <= 0){
			scaledPos.x *= 0.5f - 2.f * (abs(scaledPos.y) * 0.2f);
		}
	}
	else if (spookyEffect == 4){
		scaledPos.y += (offsetY);
	/*	if (scaledPos.x >= 0){
			scaledPos.x *= abs((abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * scaledPos.y)));
			scaledPos.y *= abs((abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * scaledPos.y)));
		}
		else if (scaledPos.x <= 0){
			scaledPos.x *= abs((abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * scaledPos.y)));
			scaledPos.y *= abs((abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * scaledPos.y)));	
		}*/

		scaledPos *= (abs(1/(scaledPos.y + 1.3f)));
	}
	/*else if (spookyEffect == 4){
		scaledPos.y += (offsetY);
		if (scaledPos.x >= 0){
			scaledPos.x *= (abs(scaledPos.y) * 0.8f - 0.01f);
		}
		else if (scaledPos.x <= 0){
			scaledPos.x *= (abs(scaledPos.y) * 0.8f - 0.01f);
		}
	}
	else if (spookyEffect == 4){
		scaledPos.y += (offsetY);
		if (scaledPos.x >= 0){
			scaledPos.x *= (abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * abs(scaledPos.y)));
			scaledPos.y *= (abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * abs(scaledPos.y)));
		}
		else if (scaledPos.x <= 0){
			scaledPos.x *= (abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * abs(scaledPos.y)));
			scaledPos.y *= (abs(scaledPos.y - 1.2f) * 0.8f - (0.01f * abs(scaledPos.y)));	
		}
	}*/


    // assign vertex position without modification
    gl_Position = vec4(scaledPos, 0.0, 1.0);

    // assign output colour to be interpolated
    tcColour = color;
}
