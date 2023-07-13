void create_triangle(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 norm)
{
	int starting_point = vDef.size();
	vDef.push_back({ pos1, norm });
	vDef.push_back({ pos2, norm });
	vDef.push_back({ pos3, norm });

	vIdx.push_back(starting_point); vIdx.push_back(starting_point + 1); vIdx.push_back(starting_point + 2);
}

void create_rectangle(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec3 norm)
{

	int starting_point = vDef.size();
	vDef.push_back({ pos1, norm });
	vDef.push_back({ pos2, norm });
	vDef.push_back({ pos3, norm });
	vDef.push_back({ pos4, norm });

	vIdx.push_back(starting_point); vIdx.push_back(starting_point + 1); vIdx.push_back(starting_point + 2);
	vIdx.push_back(starting_point + 1); vIdx.push_back(starting_point + 2); vIdx.push_back(starting_point + 3);
}

void create_cylinder(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx, int sides, float height, float radius)
{
	glm::vec3 top_center = { 0, height / 2, 0 };
	glm::vec3 bottom_center = { 0, -height / 2, 0 };
	glm::vec3 norm = { 0, 1, 0 };
	glm::vec3 side_norm = { 0, 0, 0 };
	float angle = 0.0f;
	vDef.push_back({ top_center, norm });
	vDef.push_back({ bottom_center, norm });
	for (int i = 0; i <= sides; i++)
	{
		angle = 2 * i * glm::pi<float>() / sides;
		double x = radius * glm::cos(angle);
		double z = radius * glm::sin(angle);
		vDef.push_back({ {x, height / 2, z}, norm });
		vDef.push_back({ {x, -height / 2, z}, norm });
		// side points
		angle = 2 * i + 1 * glm::pi<float>() / sides;
		double next_x = radius * glm::cos(angle);
		double next_z = radius * glm::sin(angle);
		//side_norm = { (x + next_x / 2), 0, (z + next_z / 2) };
		side_norm = { x, 0, z};
		vDef.push_back({ {x, height / 2, z},  side_norm });
		vDef.push_back({ {x, -height / 2, z}, side_norm });
	}
	for (int i = 0; i <= sides; i++)
	{
		// top of the cylinder
		vIdx.push_back(0); vIdx.push_back((i) * 4 + 2); vIdx.push_back((i + 1) * 4 + 2);
		// bottom of the cylinder
		vIdx.push_back(1); vIdx.push_back((i) * 4 + 3); vIdx.push_back((i + 1) * 4 + 3);
		// sides of cylinder
		vIdx.push_back((i + 1) * 4); vIdx.push_back((i + 2) * 4); vIdx.push_back((i + 1) * 4 + 1);
		vIdx.push_back((i + 2) * 4); vIdx.push_back((i + 1) * 4 + 1); vIdx.push_back((i + 2) * 4 + 1);
	}
}

void Assignment14::createCubeMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
	// The primitive built here is a cube centered in the origin, whose edge lenght is 2.
	create_rectangle(vDef, vIdx, { -1, -1, -1 }, { -1, -1, 1 }, { -1, 1, -1 }, { -1, 1, 1 }, { 1, 0, 0 });
	create_rectangle(vDef, vIdx, { 1, -1, -1 }, { 1, -1, 1 }, { 1, 1, -1 }, { 1, 1, 1 }, { 1, 0, 0 });
	create_rectangle(vDef, vIdx, { -1, -1, -1 }, { -1, -1, 1 }, { 1, -1, -1 }, { 1, -1, 1 }, { 0, 1, 0 });
	create_rectangle(vDef, vIdx, { -1, 1, -1 }, { -1, 1, 1 }, { 1, 1, -1 }, { 1, 1, 1 }, { 0, 1, 0 });
	create_rectangle(vDef, vIdx, { -1, -1, -1 }, { -1, 1, -1 }, { 1, -1, -1 }, { 1, 1, -1 }, { 0, 0, 1 });
	create_rectangle(vDef, vIdx, { -1, -1, 1 }, { -1, 1, 1 }, { 1, -1, 1 }, { 1, 1, 1 }, { 0, 0, 1 });
}

void Assignment14::createFunctionMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.
	float starting_range = 3.0f;
	float step = 5.0f;
	float range = starting_range * step;
	float full_length = range * 2 + 1;
	glm::vec3 norm = { 0, 0, 0 };
	for (int xi = -range; xi <= range; xi++)
	{
		for (int zi = -range; zi <= range; zi++)
		{
			float x = (float)xi / step;
			float z = (float)zi / step;
			float y = sin(x) * cos(z);
			glm::vec3 der_x = { 1, cos(x) * cos(z), 0 };
			glm::vec3 der_z = { 0, sin(x) * -sin(z), 1 };
			norm = { cos(x) * cos(z), 1, sin(x) * -sin(z) };
			//norm = norm / abs(norm);
			vDef.push_back({ {x, y, z},  norm });
		}
	}
	for (int x = -range; x <= range; x++)
	{
		for (int z = -range; z <= range; z++)
		{
			if (x == -range || x == range || z == -range)
			{
				continue;
			}
			//std::cout << (x + range) + (z + range) * full_length << "&" << (x + range) + (z + range - 1) * full_length << "&" << (x + range) + (z + range - 1) * full_length + 1 << " @@@ ";
			vIdx.push_back((x + range) + (z + range) * full_length); vIdx.push_back((x + range) + (z + range - 1) * full_length); vIdx.push_back((x + range) + (z + range - 1) * full_length + 1);
			vIdx.push_back((x + range) + (z + range) * full_length); vIdx.push_back((x + range) + (z + range) * full_length + 1); vIdx.push_back((x + range) + (z + range - 1) * full_length + 1);
		}
	}
}

void Assignment14::createCylinderMesh(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.

	create_cylinder(vDef, vIdx, 100, 2, 1);
}

