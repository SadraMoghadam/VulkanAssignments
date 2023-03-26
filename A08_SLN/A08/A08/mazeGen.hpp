void Assignment08::createMazeMesh(int row, int col, char **maze) {
	// The procedure gets in input the number of rows <row> of the maze, and the number of columns <col>
	// Element <maze[r][c]>, with 0 <= r <= row-1 and 0 <= c <= col-1 contains:
	//		Symbol ' ' if there is an empty space
	//		Symbol '#' if there is a wall
	//
	// Example: The following piece of code executes the instruction in the ... section
	//          if there is a wall in position r=5, c=7
	//
	// int r, c;
	// r = 5; c = 7;
	// if(maze[r][c] == '#') {
	//		...
	// }
	//
	// The example below creates a square, with four vertices and two triangles.
	// It must be deleted and replaced with your code creating the mesh of the maze.
	
	// Fill array vPos with the positions of the vertices of the mesh
	vPos.push_back(0.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 0
	vPos.push_back(1.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 1
	vPos.push_back(0.0); vPos.push_back(1.0); vPos.push_back(0.0);	// vertex 2
	vPos.push_back(1.0); vPos.push_back(1.0); vPos.push_back(0.0);	// vertex 3

	// Fill the array vIdx with the indices of the vertices of the triangles
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	
}
