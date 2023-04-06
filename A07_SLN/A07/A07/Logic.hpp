

float yaw = 0, pitch = 0, roll = 0;
glm::quat rot = glm::quat(1, 0, 0, 0);

glm::mat4 ViewPrjOld = glm::mat4(1);

void GameLogic(Assignment07* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World)
{
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(45.0f);
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	// Player starting point
	const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);

	// Camera target height and distance
	const float camHeight = 0.25;
	const float camDist = 1.5;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(120.0f);
	const float MOVE_SPEED = 2.0f;

	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assginment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	bool fire = false;
	A->getSixAxis(deltaT, m, r, fire);

	// Game Logic implementation
	// Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
	static glm::vec3 Pos = StartingPosition;


	// To be done in the assignment
	//glm::vec3 u = glm::vec3(glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1));
	//glm::vec3 vz = glm::normalize(glm::vec3(camDist, 0, camHeight));
	//glm::vec3 vx = glm::normalize(glm::cross(u, vz));
	//glm::vec3 vy = glm::cross(vz, vx);
	//glm::mat4 Mc = glm::mat4(glm::vec4(vx, 0), glm::vec4(vy, 0), glm::vec4(vz, 0), glm::vec4(0, 0, 0, 1));
	//glm::mat4 Mv = glm::inverse(Mc);
	///// View of third person, no rotation while rotating the character

	glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1));
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));
	pitch += ROT_SPEED * r.x * deltaT;
	yaw += ROT_SPEED * r.y * deltaT;
	roll += ROT_SPEED * r.z * deltaT;
	Pos += ux * MOVE_SPEED * m.x * deltaT;
	Pos += uy * MOVE_SPEED * m.y * deltaT;
	Pos += uz * MOVE_SPEED * m.z * deltaT;
	glm::mat4 T = glm::translate(glm::mat4(1.0), Pos);
	if (pitch <= minPitch)
		pitch = minPitch;
	else if (pitch >= maxPitch)
		pitch = maxPitch;
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0), -pitch, glm::vec3(1, 0, 0));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0), -yaw, glm::vec3(0, 1, 0));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0), -roll, glm::vec3(0, 0, 1));
	World = T * Ry;



	//glm::vec3 c = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z + camDist);
	glm::vec3 c = glm::vec3(glm::vec4(Pos.x, Pos.y + camHeight + (camDist * sin(pitch)), Pos.z + camDist * cos(pitch), 1));
	glm::vec3 a = glm::vec3(glm::vec4(Pos.x, Pos.y, Pos.z, 1.0f)) + glm::vec3(0, camHeight, 0);
	glm::mat4 Mv = glm::lookAt(c, a, glm::vec3(0, 1, 0));
	glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	Mp[1][1] *= -1;
	ViewPrj = Mp * Mv;

	// DAMPING
	float lambda = 10;
	if(ViewPrjOld == glm::mat4(1))
		ViewPrjOld = ViewPrj;
	ViewPrj = ViewPrjOld * exp(-lambda * deltaT) + ViewPrj * (1 - exp(-lambda * deltaT));
	ViewPrjOld = ViewPrj;

	//glm::vec3 c = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z - camDist);
	///////// View of third person, yaw rotation
	//c = glm::vec3(glm::vec4(0, camHeight + (camDist * sin(pitch)), camDist * cos(pitch), 1) * World);
	//Pos = glm::vec3(glm::vec4(0, 0, 0, 1) * World) + glm::vec3(0, 0, camHeight);
	//glm::mat4 Mv = glm::lookAt(c, Pos, glm::vec3(0, 1, 0));
	////glm::mat4 Mv = glm::lookAt(glm::vec3(Pos.x, Pos.y + camHeight, Pos.z - camDist), glm::vec3(Pos.x, Pos.y, Pos.z), glm::vec3(0, 1, 0));
	//glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	//Mp[1][1] *= -1;
	//ViewPrj = Mp * Mv;
}






#pragma region GameLogicEuler



void GameLogic1(Assignment07* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World)
{
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(45.0f);
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	// Player starting point
	const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);

	// Camera target height and distance
	const float camHeight = 0.25;
	const float camDist = 1.5;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(120.0f);
	const float MOVE_SPEED = 2.0f;

	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assginment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	bool fire = false;
	A->getSixAxis(deltaT, m, r, fire);

	// Game Logic implementation
	// Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
	static glm::vec3 Pos = StartingPosition;


	// To be done in the assignment
	//glm::vec3 u = glm::vec3(glm::rotate(glm::mat4(1), yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1));
	//glm::vec3 vz = glm::normalize(glm::vec3(camDist, 0, camHeight));
	//glm::vec3 vx = glm::normalize(glm::cross(u, vz));
	//glm::vec3 vy = glm::cross(vz, vx);
	//glm::mat4 Mc = glm::mat4(glm::vec4(vx, 0), glm::vec4(vy, 0), glm::vec4(vz, 0), glm::vec4(0, 0, 0, 1));
	//glm::mat4 Mv = glm::inverse(Mc);
	///// View of third person, no rotation while rotating the character

	glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1));
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), -yaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));
	pitch += ROT_SPEED * r.x * deltaT;
	yaw += ROT_SPEED * r.y * deltaT;
	roll += ROT_SPEED * r.z * deltaT;
	Pos += ux * MOVE_SPEED * m.x * deltaT;
	Pos += uy * MOVE_SPEED * m.y * deltaT;
	Pos += uz * MOVE_SPEED * m.z * deltaT;
	//glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(MOVE_SPEED * m.x * deltaT, MOVE_SPEED * m.y * deltaT, MOVE_SPEED * m.z * deltaT));
	//glm::mat4 Rx = glm::rotate(glm::mat4(1.0), ROT_SPEED * r.x * deltaT, glm::vec3(1, 0, 0));
	//glm::mat4 Ry = glm::rotate(glm::mat4(1.0), ROT_SPEED * r.y * deltaT, glm::vec3(0, 1, 0));
	//glm::mat4 Rz = glm::rotate(glm::mat4(1.0), ROT_SPEED * r.z * deltaT, glm::vec3(0, 0, 1));
	//World = T * Rx * Ry * Rz;
	glm::mat4 T = glm::translate(glm::mat4(1.0), Pos);
	if (pitch <= minPitch)
		pitch = minPitch;
	else if (pitch >= maxPitch)
		pitch = maxPitch;
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0), -pitch, glm::vec3(1, 0, 0));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0), -yaw, glm::vec3(0, 1, 0));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0), -roll, glm::vec3(0, 0, 1));
	World = T * Ry * Rz;
	//World = Pos * yaw * pitch * roll;



	//glm::vec3 c = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z + camDist);
	glm::vec3 c = glm::vec3(glm::vec4(Pos.x, Pos.y + camHeight + (camDist * sin(pitch)), Pos.z + camDist * cos(pitch), 1));
	glm::vec3 a = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z);
	glm::mat4 Mv = glm::lookAt(c, a, glm::vec3(0, 1, 0));
	glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	Mp[1][1] *= -1;
	ViewPrj = Mp * Mv;


	//glm::vec3 c = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z - camDist);
	///////// View of third person, yaw rotation
	//c = glm::vec3(glm::vec4(0, camHeight + (camDist * sin(pitch)), camDist * cos(pitch), 1) * World);
	//Pos = glm::vec3(glm::vec4(0, 0, 0, 1) * World) + glm::vec3(0, 0, camHeight);
	//glm::mat4 Mv = glm::lookAt(c, Pos, glm::vec3(0, 1, 0));
	////glm::mat4 Mv = glm::lookAt(glm::vec3(Pos.x, Pos.y + camHeight, Pos.z - camDist), glm::vec3(Pos.x, Pos.y, Pos.z), glm::vec3(0, 1, 0));
	//glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	//Mp[1][1] *= -1;
	//ViewPrj = Mp * Mv;
}


#pragma endregion

#pragma region GameLogicQuat
void GameLogic2(Assignment07* A, float Ar, glm::mat4& ViewPrj, glm::mat4& World) {
	// The procedure must implement the game logic  to move the character in third person.
	// Input:
	// <Assignment07 *A> Pointer to the current assignment code. Required to read the input from the user
	// <float Ar> Aspect ratio of the current window (for the Projection Matrix)
	// Output:
	// <glm::mat4 &ViewPrj> the view-projection matrix of the camera
	// <glm::mat4 &World> the world matrix of the object
	// Parameters
	// Camera FOV-y, Near Plane and Far Plane
	const float FOVy = glm::radians(45.0f);
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	// Player starting point
	const glm::vec3 StartingPosition = glm::vec3(-41.50, 0.0, -19.5);

	// Camera target height and distance
	const float camHeight = 0.25;
	const float camDist = 1.5;
	// Camera Pitch limits
	const float minPitch = glm::radians(-8.75f);
	const float maxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float ROT_SPEED = glm::radians(120.0f);
	const float MOVE_SPEED = 2.0f;

	// Integration with the timers and the controllers
	// returns:
	// <float deltaT> the time passed since the last frame
	// <glm::vec3 m> the state of the motion axes of the controllers (-1 <= m.x, m.y, m.z <= 1)
	// <glm::vec3 r> the state of the rotation axes of the controllers (-1 <= r.x, r.y, r.z <= 1)
	// <bool fire> if the user has pressed a fire button (not required in this assginment)
	float deltaT;
	glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
	bool fire = false;
	A->getSixAxis(deltaT, m, r, fire);

	// Game Logic implementation
	// Current Player Position - statc variables make sure thattheri value remain unchanged in subsequent calls to the procedure
	static glm::vec3 Pos = StartingPosition;


	// To be done in the assignment
	pitch += ROT_SPEED * r.x * deltaT;
	yaw += ROT_SPEED * r.y * deltaT;
	roll += ROT_SPEED * r.z * deltaT;
	if (pitch <= minPitch)
		pitch = minPitch;
	else if (pitch >= maxPitch)
		pitch = maxPitch;
	/*glm::quat rot = glm::quat(glm::vec3(0, glm::radians(yaw), 0)) *
		glm::quat(glm::vec3(glm::radians(pitch), 0, 0)) *
		glm::quat(glm::vec3(0, 0, glm::radians(roll)));*/
	rot = rot * glm::rotate(glm::quat(1, 0, 0, 0), ROT_SPEED * r.x * deltaT, glm::vec3(1, 0, 0));
	rot = rot * glm::rotate(glm::quat(1, 0, 0, 0), ROT_SPEED * r.y * deltaT, glm::vec3(0, 1, 0));
	rot = rot * glm::rotate(glm::quat(1, 0, 0, 0), ROT_SPEED * r.z * deltaT, glm::vec3(0, 0, 1));
	glm::vec3 ux = glm::vec3(glm::mat4(rot) * glm::vec4(1, 0, 0, 1));
	glm::vec3 uy = glm::vec3(glm::mat4(rot) * glm::vec4(0, 1, 0, 1));
	glm::vec3 uz = glm::vec3(glm::mat4(rot) * glm::vec4(0, 0, 1, 1));
	Pos += ux * MOVE_SPEED * m.x * deltaT;
	Pos += uy * MOVE_SPEED * m.y * deltaT;
	Pos += uz * MOVE_SPEED * m.z * deltaT;

	glm::mat4 T = glm::translate(glm::mat4(1.0), Pos);
	World = T * glm::mat4(rot);
	/*World = World * glm::translate(glm::mat4(1), glm::vec3(MOVE_SPEED * m.x * deltaT, MOVE_SPEED * m.y * deltaT, MOVE_SPEED * m.z * deltaT));
	World = World * glm::rotate(glm::mat4(1), ROT_SPEED * r.x * deltaT, glm::vec3(1, 0, 0));
	World = World * glm::rotate(glm::mat4(1), ROT_SPEED * r.y * deltaT, glm::vec3(0, 1, 0));
	World = World * glm::rotate(glm::mat4(1), ROT_SPEED * r.z * deltaT, glm::vec3(0, 0, 1));*/
	//World = Pos * yaw * pitch * roll;



	//glm::vec3 c = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z + camDist);
	glm::vec3 c = glm::vec3(glm::vec4(Pos.x, Pos.y + camHeight + (camDist * sin(pitch)), Pos.z + camDist * cos(pitch), 1) * World);
	glm::vec3 a = glm::vec3(glm::vec4(Pos.x, Pos.y, Pos.z, 1.0f) * World) + glm::vec3(0, camHeight, 0);
	glm::mat4 Mv = glm::lookAt(c, a, glm::vec3(0, 1, 0));
	glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	Mp[1][1] *= -1;
	ViewPrj = Mp * Mv;


	//glm::vec3 c = glm::vec3(Pos.x, Pos.y + camHeight, Pos.z - camDist);
	///////// View of third person, yaw rotation
	//c = glm::vec3(glm::vec4(0, camHeight + (camDist * sin(pitch)), camDist * cos(pitch), 1) * World);
	//Pos = glm::vec3(glm::vec4(0, 0, 0, 1) * World) + glm::vec3(0, 0, camHeight);
	//glm::mat4 Mv = glm::lookAt(c, Pos, glm::vec3(0, 1, 0));
	////glm::mat4 Mv = glm::lookAt(glm::vec3(Pos.x, Pos.y + camHeight, Pos.z - camDist), glm::vec3(Pos.x, Pos.y, Pos.z), glm::vec3(0, 1, 0));
	//glm::mat4 Mp = glm::perspective(FOVy, Ar, nearPlane, farPlane);
	//Mp[1][1] *= -1;
	//ViewPrj = Mp * Mv;
}
#pragma endregion
