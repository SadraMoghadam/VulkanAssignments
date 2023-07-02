void create_triangle(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 norm)
{
	int starting_point = vDef.size();
	vDef.push_back({ pos1, norm });
	vDef.push_back({ pos2, norm });
	vDef.push_back({ pos3, norm });

	vIdx.push_back(starting_point); vIdx.push_back(starting_point + 1); vIdx.push_back(starting_point + 2);
}

void create_rectangle(std::vector<Vertex>& vDef, std::vector<uint32_t>& vIdx, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec3 norm, 
	glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec2 uv4)
{

	int starting_point = vDef.size();
	vDef.push_back({ pos1, norm, uv1});
	vDef.push_back({ pos2, norm, uv2 });
	vDef.push_back({ pos3, norm, uv3 });
	vDef.push_back({ pos4, norm, uv4 });

	vIdx.push_back(starting_point); vIdx.push_back(starting_point + 1); vIdx.push_back(starting_point + 2);
	vIdx.push_back(starting_point + 1); vIdx.push_back(starting_point + 2); vIdx.push_back(starting_point + 3);
}

void Assignment15::createBoxMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	float width = 1024.0f;
	float height = 512.0f;
	float box_len = 2 / 2;
	float depth = box_len / 3.7;
	create_rectangle(vDef, vIdx, { -box_len, -box_len, -depth }, { -box_len, -box_len, depth }, { -box_len, box_len, -depth }, { -box_len, box_len, depth }, { 1, 0, 0 },
		{ 0.0f / width, 415.0f / height }, { 92.0f / width, 413.0f / height }, { 0.0f / width, 96.0f / height }, { 92.0f / width, 96.0f / height });
	create_rectangle(vDef, vIdx, { box_len, -box_len, -depth }, { box_len, -box_len, depth }, { box_len, box_len, -depth }, { box_len, box_len, depth }, { 1, 0, 0 },
		{ 536.0f / width, 415.0f / height }, { 445.0f / width, 413.0f / height }, { 536.0f / width, 96.0f / height }, { 445.0f / width, 96.0f / height });
	create_rectangle(vDef, vIdx, { -box_len, -box_len, -depth }, { -box_len, -box_len, depth }, { box_len, -box_len, -depth }, { box_len, -box_len, depth }, { 0, 1, 0 },
		{ 96.0f / width, 512.0f / height }, { 96.0f / width, 421.0f / height }, { 442.0f / width, 512.0f / height }, { 442.0f / width, 421.0f / height });
	create_rectangle(vDef, vIdx, { -box_len, box_len, -depth }, { -box_len, box_len, depth }, { box_len, box_len, -depth }, { box_len, box_len, depth }, { 0, 1, 0 },
		{ 96.0f / width, 0.0f / height }, { 96.0f / width, 90.0f / height }, { 442.0f / width, 0.0f / height }, { 442.0f / width, 90.0f / height });
	create_rectangle(vDef, vIdx, { -box_len, -box_len, -depth }, { -box_len, box_len, -depth }, { box_len, -box_len, -depth }, { box_len, box_len, -depth }, { 0, 0, 1 },
		{ 887.0f / width, 415.0f / height }, { 887.0f / width, 96.0f / height }, { 540.0f / width, 413.0f / height }, { 540.0f / width, 96.0f / height });
	create_rectangle(vDef, vIdx, { -box_len, -box_len, depth }, { -box_len, box_len, depth }, { box_len, -box_len, depth }, { box_len, box_len, depth }, { 0, 0, 1 },
		{ 96.0f / width, 415.0f / height }, { 96.0f / width, 96.0f / height }, { 442.0f / width, 413.0f / height }, { 442.0f / width, 96.0f / height });
}

void Assignment15::createSphereMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The primitive built here is a sphere of radius 1, centered in the origin, on which the Mars texture is applied seamless.

	float radius = 1.0f, theta, phi, x, y, z, norm_x, norm_y, norm_z, u, v;
	int sides = 100;
	for (int i = 0; i <= sides; i++)
	{
		float theta = i * glm::pi<float>() / sides;
		for (int j = 0; j <= sides; j++)
		{
			phi = j * ( 2 * glm::pi<float>() / sides);
			x = radius * sin(theta) * cos(phi);
			y = radius * cos(theta);
			z = radius * sin(phi) * sin(theta);
			norm_x = sin(theta) * cos(phi) / radius;
			norm_y = sin(theta) * sin(phi) / radius;
			norm_z = cos(theta) / radius;
			u = (float)j / sides;
			v = (float)i / sides;
			vDef.push_back({ {x, y, z}, { norm_x, norm_y, norm_z }, {u, v} });
		}
	}
	for (int i = 0; i < sides; i++)
	{
		for (int j = 0; j < sides; j++)
		{

			vIdx.push_back(i * (sides + 1) + j); vIdx.push_back(i * (sides + 1) + j + 1); vIdx.push_back(i * (sides + 1) + j + (sides + 1));
			vIdx.push_back(i * (sides + 1) + j + 1); vIdx.push_back(i * (sides + 1) + j + (sides + 1)); vIdx.push_back(i * (sides + 1) + j + (sides + 1) + 1);
		}
	}
}

