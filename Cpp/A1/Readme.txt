Readme
David Sepulveda
10161931

Collaborators:
	Nour Hammad

How to Compile: Use the make file provided. Ex:
~ make



Scenes:
	1: circle
	2: spiral
	3: square
	4: nested squares
	5: sierpinski triangles
	6: barnsley fern

Commands:
	up:
		when current image is:
			- spiral: increases number of revolutions
			- nested squares: increases level of nesting
			- sierpinski triangles: increases level (caps at 7)
			- barnsley fern: increases number of dots (caps at 1000000000)
	down:
		when current image is:
			- spiral: decreases number of revolutions (caps at 1)
			- nested squares: decreases level of nesting (caps at 1)
			- sierpinski triangles: decreases level (caps at 0)
			- barnsley fern: decreases number of dots (caps at 3000)
	left:
		when current image is:
			- spiral: increases starting angle (doesn't work fully)
			- nested squares: increases starting angle (essentially turns the image)
			- sierpinski triangles: increases starting angles (turns each individual triangle)
	right:
		when current image is:
			- spiral: decreases starting angle (doesn't work fully)
			- nested squares: decreases starting angle (essentially turns the image)
			- sierpinski triangles: decreases starting angles (turns each individual triangle)

Features:
	I was unable to complete the snowflake, but I implemented other features. My sierpinski triangles rotate, as do my nested squars.
	My spiral rotates in a mostly working way, but it causes a bug when increasing the rotations after this will mess up the leangth of the spiral.

Resources:
	Barsnley fern: https://en.wikipedia.org/wiki/Barnsley_fern