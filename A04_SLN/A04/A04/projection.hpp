

void SetupProjectionMatrices(Assignment04 *A, float Ar) {
/**************
 Method Matrix(int N, glm::mat4 M) of object A,
 receives in N the number of the matrix that needs to be set, and the matrix in M
***************/

	glm::mat4 S, R1, R2, Sh;
	float width = 10;
	float near = -50, far = 50;
	float ro = .5f;
	
	
	// Isometric projection
	R1 = glm::rotate(glm::mat4(1), glm::radians(35.26f), glm::vec3(1, 0, 0));
	R2 = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
	S = glm::mat4(1.0f / width, 0, 0, 0, 0, static_cast<float>(-Ar / width), 0, 0, 0, 0, 1.0f / (near - far), 0, 0, 0, near / (near - far), 1.0f);
	//S = glm::mat4(0.1f, 0, 0, 0, 0, -0.17f, 0, 0, 0, 0, -0.01f, 0, 0, 0, 0.5f, 1);
	S = S * R1 * R2;
	A->Matrix(1, S); // sets the matrix corresponding to piece 1

	// Dimetric
	R1 = glm::rotate(glm::mat4(1), glm::radians(20.0f), glm::vec3(1, 0, 0));
	R2 = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 1, 0));
	S = glm::mat4(1.0f / width, 0, 0, 0, 0, static_cast<float>(-Ar / width), 0, 0, 0, 0, 1.0f / (near - far), 0, 0, 0, near / (near - far), 1.0f);
	S = S * R1 * R2;
	A->Matrix(2, S); // sets the matrix corresponding to piece 2

	// Trimetric
	R1 = glm::rotate(glm::mat4(1), glm::radians(40.0f), glm::vec3(1, 0, 0));
	R2 = glm::rotate(glm::mat4(1), glm::radians(-60.0f), glm::vec3(0, 1, 0));
	S = glm::mat4(1.0f / width, 0, 0, 0, 0, static_cast<float>(-Ar / width), 0, 0, 0, 0, 1.0f / (near - far), 0, 0, 0, near / (near - far), 1.0f);
	S = S * R1 * R2;
	A->Matrix(3, S); // sets the matrix corresponding to piece 3

	// Cabinet
	Sh = glm::mat4(1.0f, 0, 0, 0, 0, 1.0f, 0, 0, -ro * cos(-30), -ro * sin(-30), 1.0f, 0, 0, 0, 0, 1.0f);
	S = glm::mat4(1.0f / width, 0, 0, 0, 0, static_cast<float>(-Ar / width), 0, 0, 0, 0, 1.0f / (near - far), 0, 0, 0, near / (near - far), 1.0f);
	S = S * Sh;
	A->Matrix(4, S); // sets the matrix corresponding to piece 4

}
