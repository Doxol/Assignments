// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated color received from vertex stage
in vec3 color;
in vec2 textureCoords;

// first output is mapped to the framebuffer's color index by default
out vec4 Fragmentcolor;

uniform sampler2DRect tex;

uniform float colorConversion;
uniform float hue;
uniform float exposureAdjustment;
uniform int gaussian;
uniform int sobel;

void main(void)
{
	int res = 1;
	vec2 newCoords;
	newCoords.x = res * (int(textureCoords.x)/res);
	newCoords.y = res * (int(textureCoords.y)/res);

	// write color output without modification
	vec4 color = texture(tex, newCoords);
	float myW;
	myW = color[3];


	float r = color[0];
	float g = color[1];
	float b = color[2];


	if (colorConversion == 1){
		float lum = 0.333f * color[0] + 0.333f * color[1] + 0.333f * color[2];
		color[0] = lum;
		color[1] = lum;
		color[2] = lum;
	}
	else if (colorConversion == 2){
		float lum = 0.299f * color[0] + 0.587f * color[1] + 0.114f * color[2];
		color[0] = lum;
		color[1] = lum;
		color[2] = lum;
	}
	else if (colorConversion == 3){
		float lum = 0.213f * color[0] + 0.715f * color[1] + 0.072f * color[2];
		color[0] = lum;
		color[1] = lum;
		color[2] = lum;
	}
	// sepia
	// the algorithm is found at this link: http://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created
	else if (colorConversion == 4){
		float lum = (0.393f * r) + (0.769f * g) + (0.189f * b);
		color[0] = lum;
		lum = (0.349f * r) + (0.686f * g) + (0.168f * b);
		color[1] = lum;
		lum = (0.272f * r) + (0.534f * g) + (0.131f * b);
		color[2] = lum;
	}
	// hue shift
	// the algorithm is found at this link: http://stackoverflow.com/questions/8507885/shift-hue-of-an-rgb-color  and here  http://beesbuzz.biz/code/hsv_color_transforms.php
	else if (colorConversion == 5){
		float U = cos((hue * 3.14159f)/180);
		float W = sin((hue * 3.14159f)/180);


		color[0] = (0.299f + 0.701f * U + 0.168f * W) * r + (0.587f - 0.587f * U + 0.330f * W) * g + (0.114f - 0.114f * U - 0.497f * W) * b;
		color[1] = (0.299f - 0.299f * U + 0.328f * W) * r + (0.587f + 0.413f * U + 0.035f * W) * g + (0.114f - 0.114f * U - 0.292f * W) * b;
		color[2] = (0.299f - 0.300f * U + 1.250f * W) * r + (0.587f - 0.588f * U + 1.05f * W) * g + (0.114f + 0.886f * U - 0.203f * W) * b;		
	}
	// exposure adjustment
	// the algorithm is found at this link: http://stackoverflow.com/questions/12166117/what-is-the-math-behind-exposure-adjustment-on-photoshop
	else if (colorConversion == 6){
		color[0] = r * pow(2, exposureAdjustment);
		color[1] = g * pow(2, exposureAdjustment);
		color[2] = b * pow(2, exposureAdjustment);
	}
	if (gaussian == 3){	
		float gaussianKernel[9] = float[](
			0.04f, 0.12f, 0.04f,
			0.12f, 0.36f, 0.12f,
			0.04f, 0.12f, 0.04f
		);

		int size = gaussian;
		float dev = size * 0.2f;


		int j = 0;

		r = 0.f;
		g = 0.f;
		b = 0.f;

		j = 0;
		vec2 pixel = newCoords;


		for(int y = (size/2); y >= -(size/2); y--){
			int k = 0;
			for(int i = -(size/2); i <= (size/2); i++){
				
				pixel = newCoords;
				pixel.x += i;
				pixel.y += y;
				vec4 pixelColor = texture(tex, pixel);
				
				r += gaussianKernel[j + k] * pixelColor.x;
				g += gaussianKernel[j + k] * pixelColor.y;
				b += gaussianKernel[j + k] * pixelColor.z;
				k++;
			}
			j += gaussian;
		}

		color = vec4(r, g, b, myW);
	}
	else if (gaussian == 5){	
		float gaussianKernel[25] = float[](
			0.0036f, 0.0144f, 0.024f, 0.0144f, 0.0036f,
			0.0144f, 0.0576f, 0.096f, 0.0576f, 0.0144f,
			0.024f, 0.096f, 0.16f, 0.096f, 0.024f,
			0.0144f, 0.0576f, 0.096f, 0.0576f, 0.0144f,
			0.0036f, 0.0144f, 0.024f, 0.0144f, 0.0036f
		);

		int size = gaussian;

		int j = 0;

		r = 0.f;
		g = 0.f;
		b = 0.f;


		j = 0;
		vec2 pixel = newCoords;

		for(int y = (size/2); y >= -(size/2); y--){
			int k = 0;
			for(int i = -(size/2); i <= (size/2); i++){
				
				pixel = newCoords;
				pixel.x += i;
				pixel.y += y;
				vec4 pixelColor = texture(tex, pixel);
				
				r += gaussianKernel[j + k] * pixelColor.x;
				g += gaussianKernel[j + k] * pixelColor.y;
				b += gaussianKernel[j + k] * pixelColor.z;
				k++;

			}
			j += gaussian;

		}

		color = vec4(r, g, b, myW);
	}
	else if (gaussian == 7){	
		float gaussianKernel[49] = float[](
			0.0009f, 0.0033f, 0.0066f, 0.0084f, 0.0066f, 0.0033f, 0.0009f,
			0.0033f, 0.0121f, 0.0242f, 0.0308f, 0.0242f, 0.0121f, 0.0033f,
			0.0066f, 0.0242f, 0.0484f, 0.0606f, 0.0484f, 0.0242f, 0.0066f,
			0.0084f, 0.0308f, 0.0616f, 0.0784f, 0.0616f, 0.0308f, 0.0084f,
			0.0066f, 0.0242f, 0.0484f, 0.0606f, 0.0484f, 0.0242f, 0.0066f,
			0.0033f, 0.0121f, 0.0242f, 0.0308f, 0.0242f, 0.0121f, 0.0033f,
			0.0009f, 0.0033f, 0.0066f, 0.0084f, 0.0066f, 0.0033f, 0.0009f
		);

		int size = gaussian;

		int j = 0;

		r = 0.f;
		g = 0.f;
		b = 0.f;

		vec2 pixel = newCoords;

		for(int y = (size/2); y >= -(size/2); y--){
			int k = 0;
			for(int i = -(size/2); i <= (size/2); i++){
				
				pixel = newCoords;
				pixel.x += i;
				pixel.y += y;
				vec4 pixelColor = texture(tex, pixel);
				
				r += gaussianKernel[j + k] * pixelColor.x;
				g += gaussianKernel[j + k] * pixelColor.y;
				b += gaussianKernel[j + k] * pixelColor.z;
				k++;

			}
			j += gaussian;

		}

		color = vec4(r, g, b, myW);
	}
	if (sobel >= 1){
		float sobelKernel[9];
		if (sobel == 1){
			sobelKernel = float[](
				1, 0, -1,
				2, 0, -2,
				1, 0, -1
			);
		}
		else if (sobel == 2){
			sobelKernel = float[](
				-1, -2, -1,
				0, 0, 0,
				1, 2, 1
			);
		}
		else{
			sobelKernel = float[](
				0, -1, 0,
				-1, 5, -1,
				0, -1, 0
			);
		}

		int size = 3;

		int j = 0;

		r = 0.f;
		g = 0.f;
		b = 0.f;

		vec2 pixel = newCoords;
		for(int y = (size/2); y >= -(size/2); y--){
			int k = 0;
			for(int i = -(size/2); i <= (size/2); i++){
				
				pixel = newCoords;
				pixel.x += i;
				pixel.y += y;
				vec4 pixelColor = texture(tex, pixel);
				
				r += sobelKernel[j + k] * pixelColor.x;
				g += sobelKernel[j + k] * pixelColor.y;
				b += sobelKernel[j + k] * pixelColor.z;
				k++;

			}
			j += 3;

		}

		color = vec4(r, g, b, myW);
	}


	Fragmentcolor = color;
}
