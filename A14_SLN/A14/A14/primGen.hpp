void Assignment14::createCubeMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
/*	vDef.push_back({{0.0f, 0.0f, 0.1f}, {0.5f, 0.0f, 0.866f}});	// vertex 0 - Position and Normal
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal
	vDef.push_back({{1.0f, 1.0f, 0.1f}, {-0.5f, 0.0f, 0.866f}});// vertex 3 - Position and Normal

	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	
	// The primitive built here is a cube centered in the origin, whose edge lenght is 2.
*/	
	// Fill array vPos with the positions of the vertices of the mesh
	vDef.push_back({{0.0f, 0.0f, 0.1f}, {0.5f, 0.0f, 0.866f}});	// vertex 0 - Position and Normal
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal
	vDef.push_back({{1.0f, 1.0f, 0.1f}, {-0.5f, 0.0f, 0.866f}});// vertex 3 - Position and Normal
	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
}

void Assignment14::createFunctionMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is the surface y = sin(x) * cos(z) with -3 <= x <= 3 and -3 <= z <= 3.
	
	// Fill array vPos with the positions of the vertices of the mesh
	vDef.push_back({{0.0f, 0.0f, 0.1f}, {0.5f, 0.0f, 0.866f}});	// vertex 0 - Position and Normal
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 2 - Position and Normal

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
}

void Assignment14::createCylinderMesh(std::vector<Vertex> &vDef, std::vector<uint32_t> &vIdx) {
	// The procedure fills array vPos with the positions of the vertices and of the normal vectors of the mesh
	// The procedures also fill the array vIdx with the indices of the vertices of the triangles
	// The primitive built here is a cylinder, with radius 1, and height 2, centered in the origin.
	
	// Fill array vPos with the positions of the vertices of the mesh
	vDef.push_back({{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 0 - Position and Normal
	vDef.push_back({{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});	// vertex 1 - Position and Normal
	vDef.push_back({{1.0f, 1.0f, 0.1f}, {-0.5f, 0.0f, 0.866f}});// vertex 2 - Position and Normal

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
}
