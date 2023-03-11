

void SetupTriangles(Assignment01 *A) {
/**************
 Function Triangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b)

 draws a triangle connecting points (x1,y1)  (x2, y2) and (x3, y3), colored with the (r,g,b) specified color.
 vertex are expressed in Normalized screen coordinates, (i.e. in range [-1,+1]), following the Vulkan convention.
 The red, green and blue (r,g,b) values are in the range [0,1].
 
 Using function Triangle(...) try to draw a children house, similar to the one below.
 
 First remove the two function below (given only as an example), and replace them with your own.
 Since it is a C program, you can use for loops if you think it can be helpful in your drawing.
 The for() command below that draws a sun like shape in exact center of the screen near the top,
 is an example on how you can use it: please remove it, and replace it with your own code.
 It is not necessery to use a for loop if you do not really need it: it has been put here only as
 an example to help students not familiar with C++.
***************/

	A->Triangle(0.1, 0.6, 0.1, -0.4, -0.25, 0.1, 1, 0, 0);		//Red
	A->Triangle(0.1, 0.34, 0.38, -0.03, 0.1, -0.4, .9, .7, 0);	//Yellow
	A->Triangle(0.1, -0.4, 0.3, -0.13, 0.3, -0.65, .6, 0, .6);	//Left Purpule
	A->Triangle(0.5, -0.4, 0.3, -0.13, 0.3, -0.65, .6, 0, .6);	//Right Purpule
	A->Triangle(0.1, -0.4, 0.1, -0.93, 0.3, -0.65, 1, 0.5, 0.5);//Pink
	A->Triangle(0.5, -0.4, 0.5, -0.93, 0.3, -0.65, 0.5, 1, 1);	//Cyan
	A->Triangle(.28, .85, -.25, .85, -0.25, 0.1, 0.3, .5, .8);	//Light Blue
	A->Triangle(-.45, .6, -.25, .85, -0.625, 0.85, 0.25, .5, 0);//Right Green
	A->Triangle(-.45, .6, -.8, .6, -0.625, 0.85, 0.25, .5, 0);	//Left Green

	//A->Triangle(0.1, -0.1, 0.1, 0.1, 0, 0, 0, 1, 0);	// Draws a green triangle pointing left
}
