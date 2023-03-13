

void SetupProjectionMatrices(Assignment05 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	
	
	// Fov-y = 60 deg
	S = glm::mat4(1);
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Fov-y = 105 deg
	S = glm::mat4(1);
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Fov-y = 25 deg
	S = glm::mat4(1);
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Right View, Fov-x = 45 deg
	S = glm::mat4(1);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
