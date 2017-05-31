README
CPSC 453 Assignment 3
David Sepulveda
10161931

How To Compile:
	make
How To Run:
	./boilerplate

Scenes:
	Fish: press 1
	Teacup: press 2
	Name: press 3
	The quick brown fox jumps over the lazy dog and scrolls: press 4
		Increase speed in a direction by pressing the left or right arrow key
	Spooky Effect 1: press 5
		Increase vertical speed by pressing up or down
	Spooky Effect 2: press 6
		Increase vertical speed by pressing up or down
	Spooky Effect 3: press 7
		Increase vertical speed by pressing up or down
	Star Wars crawl: press 8
		Increase vertical speed by pressing up or down
	Alex Brush Regular Font: press 9
	Source Sans Pro Regular Font: press 0

Change font:
	Press 9, 0, or type in the filepath of your chosen font.

Note:
	You can also choose your own series of strings to print to the screen if you so desire. However, I did not implement a dynamic
	form of this, therefore you would need to hard code in an array of strings and pass it in to the drawStrings method. The alternative
	is to simply type in your own text all on one line.

	I also attempted the bonus that allows the user to move around control points but I ran into a problem when I tried rendering more than
	one geometry.

	Also note, some scenes are able to scale even though it is not required for these scenes to scale or scroll. I just thought it was kind of
	nice to see my fish swim accross the screen. However, I did not implement any kind of code to make it so that this code works as well as the
	required parts of the assignment. These are not bugs, I was just too lazy to fix them because they are not a required part of the assignment.

Collaborators:
	I spoke to Susant as I was writing the code to allowed me to change the strings that printed on the screen and he said it would be
	cool if I could type in the string, so I made it so typing was allowed.