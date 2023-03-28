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
	
	//// Fill array vPos with the positions of the vertices of the mesh
	//vPos.push_back(0.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 0
	//vPos.push_back(1.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 1
	//vPos.push_back(0.0); vPos.push_back(1.0); vPos.push_back(0.0);	// vertex 2
	//vPos.push_back(1.0); vPos.push_back(1.0); vPos.push_back(0.0);	// vertex 3

	//// Fill the array vIdx with the indices of the vertices of the triangles
	//vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);	// First triangle
	//vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);	// Second triangle
	//vPos.push_back(0.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 0
	//vPos.push_back(1.0); vPos.push_back(0.0); vPos.push_back(0.0);	// vertex 1
	//vPos.push_back(0.0); vPos.push_back(0); vPos.push_back(1);	// vertex 2
	//vPos.push_back(1.0); vPos.push_back(0); vPos.push_back(1);	// vertex 3

	//// Fill the array vIdx with the indices of the vertices of the triangles
	//vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(6);	// First triangle
	//vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(7);


	int wall_height = 1;
	int n = 1;
	int vert_counter = 0;
	// ground and outer walls
	vPos.push_back(0); vPos.push_back(0.0); vPos.push_back(0);
	vPos.push_back(row); vPos.push_back(0); vPos.push_back(0);
	vPos.push_back(0); vPos.push_back(0.0); vPos.push_back(col);
	vPos.push_back(row); vPos.push_back(0); vPos.push_back(col);
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);

	vPos.push_back(0); vPos.push_back(wall_height); vPos.push_back(0);
	vPos.push_back(row); vPos.push_back(wall_height); vPos.push_back(0);
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(4);
	vIdx.push_back(1); vIdx.push_back(4); vIdx.push_back(5);

	vPos.push_back(0); vPos.push_back(wall_height); vPos.push_back(col);
	vPos.push_back(row); vPos.push_back(wall_height); vPos.push_back(col);
	vIdx.push_back(2); vIdx.push_back(3); vIdx.push_back(6);
	vIdx.push_back(3); vIdx.push_back(6); vIdx.push_back(7);

	vert_counter += 8;


	// inner walls
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (maze[i][j] == '#')
			{
				vPos.push_back(i); vPos.push_back(wall_height); vPos.push_back(j);
				vPos.push_back(i); vPos.push_back(wall_height); vPos.push_back(j + 1);
				vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j);
				vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j + 1);
				vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
				vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);
				vert_counter += 4;

				// north and west outer walls
				if (i == 0)
				{
					vPos.push_back(i); vPos.push_back(0.0); vPos.push_back(j);
					vPos.push_back(i); vPos.push_back(wall_height); vPos.push_back(j);
					vPos.push_back(i); vPos.push_back(0.0); vPos.push_back(j + 1);
					vPos.push_back(i); vPos.push_back(wall_height); vPos.push_back(j + 1);
					vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
					vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);
					vert_counter += 4;
				}
				else if (i == row - 1)
				{
					vPos.push_back(i + 1); vPos.push_back(0.0); vPos.push_back(j);
					vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j);
					vPos.push_back(i + 1); vPos.push_back(0.0); vPos.push_back(j + 1);
					vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j + 1);
					vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
					vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);
					vert_counter += 4;
				}
			}

			if (j == col - 1)
				continue;
			if ((maze[i][j] == '#' && maze[i][j + 1] == ' ') || (maze[i][j] == ' ' && maze[i][j + 1] == '#'))
			{
				vPos.push_back(i); vPos.push_back(0.0); vPos.push_back(j + 1);
				vPos.push_back(i); vPos.push_back(wall_height); vPos.push_back(j + 1);
				vPos.push_back(i + 1); vPos.push_back(0.0); vPos.push_back(j + 1);
				vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j + 1);
				vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
				vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);
				vert_counter += 4;
			}

			if (i == row - 1)
				continue;
			if ((maze[i][j] == '#' && maze[i + 1][j] == ' ') || (maze[i][j] == ' ' && maze[i + 1][j] == '#'))
			{
				vPos.push_back(i + 1); vPos.push_back(0.0); vPos.push_back(j);
				vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j);
				vPos.push_back(i + 1); vPos.push_back(0.0); vPos.push_back(j + 1);
				vPos.push_back(i + 1); vPos.push_back(wall_height); vPos.push_back(j + 1);
				vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
				vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);
				vert_counter += 4;
			}
		}
	}

}
