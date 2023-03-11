

void SetupProjectionMatrices(Assignment04 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	
	
	// Isometric projection
	S = glm::mat4(1);
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Dimetric
	S = glm::mat4(1);
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Trimetric
	S = glm::mat4(1);
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Cabinet
	S = glm::mat4(1);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
