// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
in vec2 textureCoords;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;
in vec2 position;

uniform float myObjects[999];
uniform int arraySize;
uniform vec3 origin;
int phong = 2;

#define LIGHT 0
#define SPHERE 1
#define REFLECTIVESPHERE 2
#define TRIANGLE 3
#define REFLECTIVETRIANGLE 4
#define PLANE 5
#define REFLECTIVEPLANE 6
#define REFLECTIONS 100


//sphere
float checkIntersection(vec3 direction, vec3 origin, vec4 sphere, float t){
	float ret = t;

	vec3 center = vec3(sphere.x, sphere.y, sphere.z);
	float radius = sphere.w;		

	float a = dot(direction, direction);
	float b = 2 * ((dot(origin, direction)) - (dot(center, direction)));
	float c = dot(origin, origin) - (2 * dot(origin, center)) + dot(center, center) - (radius * radius);

	float discrim = (b*b) - (4*a*c);

	if (discrim < 0){
		return ret;
	}

	float t1 = (-b + sqrt(discrim)) / (2*a);
	float t2 = (-b - sqrt(discrim)) / (2*a);

	if (t1 < t2){
		ret = t1; 
	}	

	else {
		ret = t2;
	}
	return ret;
}


//triangle
float checkIntersection(vec3 d, vec3 o, vec3 p0, vec3 p1, vec3 p2, float t){
	float ret = t;

	vec3 e1 = p1 - p0;
	vec3 e2 = p2 - p0;
	vec3 s = o - p0;

	mat3 m0 = mat3(-d, e1, e2);
	mat3 m1 = mat3(s, e1, e2);
	mat3 m2 = mat3(-d, s, e2);
	mat3 m3 = mat3(-d, e1, s);

	float d0 = determinant(m0);
	float d1 = determinant(m1);
	float d2 = determinant(m2);
	float d3 = determinant(m3);

	vec3 dets = vec3(d1, d2, d3);

	vec3 tuv = (1/d0) * dets;

	if (tuv.x > 0)
		if (((tuv.y + tuv.z) >= 0) && ((tuv.y + tuv.z) <= 1))
			if ((tuv.y >= 0) && (tuv.y <= 1))
				if ((tuv.z >= 0) && (tuv.z <= 1))
					ret = (tuv.x >= 0) ? tuv.x : ret;

	return ret;
}


//plane
float checkIntersection(vec3 d, vec3 o, vec3 p, vec3 q, float t){
	float ret = t;

	vec3 n = p * q;

	float qn = dot(q, n);
	float on = dot(o, n);
	float dn = dot(d, n);

	float numerator = qn - on;
	ret = numerator/dn;

	return ret;
}


vec3 sNormalVec(vec3 origin, float t, vec3 d, vec4 sphere){
	return normalize((origin + (t * d)) - vec3(sphere.x, sphere.y, sphere.z));
}

vec3 tNormalVec(vec3 t0, vec3 t1, vec3 t2){
	vec3 t1t0 = t2 - t0;
	vec3 t1t2 = t2 - t1;
	return (dot(t1t0, t1t2) > 0) ? normalize(cross(t1t0, t1t2)) : -normalize(cross(t1t0, t1t2));
}


vec3 colorEquation(vec3 l, vec3 cl, vec3 n, vec3 cr, vec3 light, vec3 origin, vec3 d, float t, vec3 cp){

	vec3 e = normalize(-d);
	vec3 h = normalize(e + l);
	vec3 ca = vec3(0.2f, 0.2f, 0.2f);

	return cr * (ca + cl * max(0, dot(n, l))) + cl * cp * pow(max(0, dot(h, n)), phong);	
}

float checkShadows(float[999] myObjects, float t, int start, int arraySize, vec3 origin, vec3 direction){
	float prevT = t;
	float minDist = 1000000000;
	int increment;
	int i = start;
	while (i < arraySize){
		increment = 1;

		if ((myObjects[i] >= SPHERE) && (myObjects[i] <= REFLECTIVESPHERE)){
			t = checkIntersection(direction, origin, vec4(myObjects[i+1], myObjects[i+2], myObjects[i+3], myObjects[i+4]), t);
			if (t != prevT && t >= 0.001f){
				if (t < minDist)
					minDist = t;
			}
			else if (t != prevT && t < 0.01f){
				t = prevT;
			}
			increment = 0;
			i += 8;
		}
		else if ((myObjects[i] >= TRIANGLE) && (myObjects[i] <= REFLECTIVETRIANGLE)){
			t = checkIntersection(direction, origin, vec3(myObjects[i+1], myObjects[i+2], myObjects[i+3]), vec3(myObjects[i+4], myObjects[i+5], myObjects[i+6]), vec3(myObjects[i+7], myObjects[i+8], myObjects[i+9]), t);
			if (t != prevT && t >= 0.01f){
				if (t < minDist)
					minDist = t;
			}
			else if (t != prevT && t < 0.01f){
				t = prevT;
			}
			increment = 0;
			i += 13;
		}
		else if ((myObjects[i] >= PLANE) && (myObjects[i] <= REFLECTIVEPLANE)){
			t = checkIntersection(direction, origin, vec3(myObjects[i+1], myObjects[i+2], myObjects[i+3]), vec3(myObjects[i+4], myObjects[i+5], myObjects[i+6]), t);
			if (t != prevT && t >= 0.01f){
				if (t < minDist)
					minDist = t;
			}
			else if (t != prevT && t < 0.01f){
				t = prevT;
			}
			increment = 0;
			i += 10;
		}
		i += increment;
	}

	return minDist;
}

vec2 checkIntersects(float[999] myObjects, int arraySize, vec3 direction, vec3 origin, float closestPoint, int lightIndex){
	int i = lightIndex + 7;

	float t;
	vec2 ret = vec2(-1000.f, t);


	int increment;
	vec4 sphere;

	while (i < arraySize){
		increment = 1;
		if ((myObjects[i] >= SPHERE) && (myObjects[i] <= (REFLECTIVESPHERE))) {
			sphere = vec4(myObjects[i+1], myObjects[i+2], myObjects[i+3], myObjects[i+4]);
			t = checkIntersection(direction, origin, sphere, t);
			if (t > 0.1 && t < closestPoint){
				closestPoint = t;
				ret[0] = i;
				ret[1] = t;
			}
			increment = 0;
			i += 8;
		}
		else if ((myObjects[i] >= TRIANGLE) && (myObjects[i] <= (REFLECTIVETRIANGLE))) {
			t = checkIntersection(direction, origin, vec3(myObjects[i+1], myObjects[i+2], myObjects[i+3]), vec3(myObjects[i+4], myObjects[i+5], myObjects[i+6]), vec3(myObjects[i+7], myObjects[i+8], myObjects[i+9]), t);
			if (t > 0.01 && t < closestPoint){
				closestPoint = t;
				ret[0] = i;
				ret[1] = t;
			}
			increment = 0;
			i += 13;
		}
		else if ((myObjects[i] >= PLANE) && (myObjects[i] <= (REFLECTIVEPLANE))) {
			t = checkIntersection(direction, origin, vec3(myObjects[i+1], myObjects[i+2], myObjects[i+3]), vec3(myObjects[i+4], myObjects[i+5], myObjects[i+6]), t);
			if (t > 0.00001 && t < closestPoint){
				closestPoint = t;
				ret[0] = i;
				ret[1] = t;
			}
			increment = 0;
			i += 10;
		}
		i += increment;
	}

	return ret;
}

vec3 reflectionRay;
vec3 reflectOrigin;
float reflectiveness;
vec3 calculateColor(float[999] myObjects, int arraySize, vec3 direction, vec3 origin, float closestPoint, int lightIndex){
	vec3 colour;
	float t;
	vec3 light = vec3(myObjects[1], myObjects[2], myObjects[3]);
	vec3 norm;
	vec4 sphere;
	vec3 objectColor;
	vec3 l;
	vec3 cl = vec3(myObjects[lightIndex + 4], myObjects[lightIndex + 5], myObjects[lightIndex + 6]);
	vec3 cp = vec3(1.f, 1.f, 1.f);
	
	int i = lightIndex + 7;

	float isShadow = 0;

	vec2 ret = checkIntersects(myObjects, arraySize, direction, origin, closestPoint, lightIndex);
	if (ret[0] != -1000){
		i = int(ret[0]);
		t = ret[1];
		if ((myObjects[i] >= SPHERE) && (myObjects[i] <= (REFLECTIVESPHERE))){
			sphere = vec4(myObjects[i+1], myObjects[i+2], myObjects[i+3], myObjects[i+4]);
			objectColor = vec3(myObjects[i+5], myObjects[i+6], myObjects[i+7]);
			norm = sNormalVec(origin, t, direction, sphere);
			l = normalize(light - (origin + (t * direction)));
			isShadow = checkShadows(myObjects, t, lightIndex + 7, arraySize, origin + t*direction, l);
			if (isShadow < 0 || isShadow < length(light - (origin + (t * direction)))){
				cl = vec3(0, 0, 0);
			}
			reflectOrigin = origin + t*direction;
			vec3 reflectNormal = norm;
			reflectionRay = normalize(direction - (2 * dot(direction, reflectNormal) * reflectNormal));
			reflectiveness = myObjects[i] - SPHERE;
			colour = colorEquation(l, cl, norm, objectColor, light, origin, direction, t, cp);
		}
		else if ((myObjects[i] >= TRIANGLE) && (myObjects[i] <= (REFLECTIVETRIANGLE))){
			objectColor = vec3(myObjects[i+10], myObjects[i+11], myObjects[i+12]);
			norm = tNormalVec(vec3(myObjects[i+1], myObjects[i+2], myObjects[i+3]), vec3(myObjects[i+4], myObjects[i+5], myObjects[i+6]), vec3(myObjects[i+7], myObjects[i+8], myObjects[i+9]));
			l = normalize(light - (origin + (t * direction)));
			isShadow = checkShadows(myObjects, t, lightIndex + 7, arraySize, origin + t*direction, l);
			if (isShadow < 0 || isShadow < length(light - (origin + (t * direction)))){
				cl = vec3(0, 0, 0);
			}
			reflectOrigin = origin + t*direction;
			vec3 reflectNormal = norm;
			reflectionRay = normalize(direction - (2 * dot(direction, reflectNormal) * reflectNormal));
			reflectiveness = myObjects[i] - TRIANGLE;
			colour = colorEquation(l, cl, norm, objectColor, light, origin, direction, t, cp);
		}
		else if ((myObjects[i] >= PLANE) && (myObjects[i] <= (REFLECTIVEPLANE))){
			objectColor = vec3(myObjects[i+7], myObjects[i+8], myObjects[i+9]);
			norm = vec3(myObjects[i+1], myObjects[i+2], myObjects[i+3]);
			l = normalize(light - (origin + (t * direction)));
			isShadow = checkShadows(myObjects, t, lightIndex + 7, arraySize, origin + t*direction, l);
			if (isShadow < 0 || isShadow < length(light - (origin + (t * direction)))){
				cl = vec3(0, 0, 0);
			}
			reflectOrigin = origin + t*direction;
			vec3 reflectNormal = norm;
			reflectionRay = normalize(direction - (2 * dot(direction, reflectNormal) * reflectNormal));
			reflectiveness = myObjects[i] - PLANE;
			colour = colorEquation(l, cl, norm, objectColor, light, origin, direction, t, cp);
		}
	}
	return colour;
}

void main(void)
{
	float view = 3.14159/3;
	float z = 1 / tan(view/2); 

	int lightIndex = 0;

	vec3 direction = normalize(vec3(position, -z));
	//vec3 origin = vec3(-1, 0, 0);

	vec3 colour = vec3(Colour);
 
	float t;
	
	float closestPoint = 1000000;

	colour = calculateColor(myObjects, arraySize, direction, origin, closestPoint, lightIndex);
	
	int i = 1;
	float multiplier = reflectiveness;
	int reflectionLevel = 0;
	vec3 temp1;
	colour = colour * (1 - reflectiveness);
	vec3 fcolour;
	while (i < REFLECTIONS && reflectiveness != 0){
		
		fcolour = calculateColor(myObjects, arraySize, reflectionRay, reflectOrigin, closestPoint, lightIndex);
		colour += (fcolour * (1 - reflectiveness)) * multiplier;
		multiplier *= reflectiveness;
		i++;
		reflectionLevel++;
	}
	// more accurate calculation, far slower.
//	i = reflectionLevel;
//	vec4 temp;
//	while(i >= 0){
//		temp = vec4(myStack1[i]);
//		fcolour = mix(fcolour, vec3(temp[0], temp[1], temp[2]), 1 - temp[3]);
//		i--;
//	}
	FragmentColour = vec4(colour, 0);
}
