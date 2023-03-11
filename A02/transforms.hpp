

void SetupMatrices(Assignment02 *A) {
/**************
 Method Matrix(int N, float v11, float v12, ... , float v14, float v21, ... , float v44) of object A,
 receives in N the number of matrix that needs to be set, and in values v11 to v44 the 16 values
 composing the corrisponding matrix 
***************/

	// To solve the puzzle: translate 2 along x, and 3 along z
	A->Matrix(1,
			  1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1); // sets the matrix corresponding to piece 1

	// To solve the puzzle: rotate -15 degrees around the z axis
	A->Matrix(2,
			  1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1); // sets the matrix corresponding to piece 2

	// To solve the puzzle: mirror over the yz plane
	A->Matrix(3,
			  1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1); // sets the matrix corresponding to piece 3

	// To solve the puzzle: perform a shear along the z axis, with hx = -0.5 (and hy=0)
	A->Matrix(4,
			  1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1); // sets the matrix corresponding to piece 4

	// To solve the puzzle: scale of 2 along the x axis, and 1.25 on the z axis			  
	A->Matrix(5,
			  1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1); // sets the matrix corresponding to piece 5

	// To solve the puzzle: perform a proportional scaling of a factor of 3
	A->Matrix(6,
			  1, 0, 0, 0,
			  0, 1, 0, 0,
			  0, 0, 1, 0,
			  0, 0, 0, 1); // sets the matrix corresponding to piece 6

}
