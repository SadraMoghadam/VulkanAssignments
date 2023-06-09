

void SetupProjectionMatrices(Assignment05 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S;
	float n = 0.1f, f = 50.0f;
	float fov;
	
	
	// Fov-y = 60 deg
	fov = glm::radians(60.0f);
	//S = glm::mat4(1.0f / (Ar * tan(-teta / 2.0f)), 0, 0, 0, 0, -1.0f / tan(-teta / 2.0f), 0, 0, 0, 0, f / (n - f), -1, 0, 0, n * f / (n - f), 0);
	S = glm::perspective(fov, Ar, 0.1f, 50.0f);
	S[1][1] *= -1;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Fov-y = 105 deg
	fov = glm::radians(105.0f);
	S = glm::perspective(fov, Ar, 0.1f, 50.0f);
	S[1][1] *= -1;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Fov-y = 25 deg
	fov = glm::radians(25.0f);
	S = glm::perspective(fov, Ar, 0.1f, 50.0f);
	S[1][1] *= -1;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Right View, Fov-x = 45 deg
	fov = glm::radians(45.0f);
	S = glm::mat4(1.0f / (Ar * tan(fov / 2.0f)), 0, 0, 0, 0, -1.0f / tan(fov / 2.0f), 0, 0, 1, 0, f / (n - f), -1, 0, 0, n * f / (n - f), 0);
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
