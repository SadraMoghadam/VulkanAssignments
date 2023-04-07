

int wall_height = 1;
int vert_counter = 0;

typedef std::vector< std::tuple<int, int, int> > tuple;


void CubeCreator(std::vector<float>& vPos, std::vector<int>& vIdx, int startRow, int endRow, int startCol, int endCol)
{
	vPos.push_back(startCol); vPos.push_back(wall_height); vPos.push_back(startRow);
	vPos.push_back(startCol); vPos.push_back(wall_height); vPos.push_back(endRow);
	vPos.push_back(endCol); vPos.push_back(wall_height); vPos.push_back(startRow);
	vPos.push_back(endCol); vPos.push_back(wall_height); vPos.push_back(endRow);
	vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
	vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);

	vPos.push_back(startCol); vPos.push_back(0); vPos.push_back(startRow);
	vPos.push_back(startCol); vPos.push_back(0); vPos.push_back(endRow);
	vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 4);
	vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 4); vIdx.push_back(vert_counter + 5);

	vPos.push_back(endCol); vPos.push_back(0); vPos.push_back(startRow);
	vPos.push_back(endCol); vPos.push_back(0); vPos.push_back(endRow);
	vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3); vIdx.push_back(vert_counter + 6);
	vIdx.push_back(vert_counter + 3); vIdx.push_back(vert_counter + 6); vIdx.push_back(vert_counter + 7);

	vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 3); vIdx.push_back(vert_counter + 5);
	vIdx.push_back(vert_counter + 3); vIdx.push_back(vert_counter + 5); vIdx.push_back(vert_counter + 7);

	vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 4);
	vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 4); vIdx.push_back(vert_counter + 6);

	vert_counter += 8;
}


void Assignment08::createMazeMesh(int row, int col, char** maze) {


	int n = 1;
	// ground and outer walls
	vPos.push_back(0); vPos.push_back(0.0); vPos.push_back(0);
	vPos.push_back(row); vPos.push_back(0); vPos.push_back(0);
	vPos.push_back(0); vPos.push_back(0.0); vPos.push_back(col);
	vPos.push_back(row); vPos.push_back(0); vPos.push_back(col);
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2);
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3);

	vert_counter += 4;

	tuple tcrr;//ColRowRow
	tuple trcc;//RowColCol
	for (int i = 0; i < row; i++)
	{
		int startRow = 0, endRow = 1;
		for (int j = 0; j < col; j++)
		{
			if (maze[i][j] == '#')
			{
				if (j == 0)
					continue;
				if (maze[i][j - 1] == ' ')
				{
					startRow = j;
				}
				endRow = j + 1;
				if (j == col - 1)
				{
					trcc.push_back(std::tuple<int, int, int>(i, startRow, endRow));
				}
			}
			else
			{
				if (j == 0)
					continue;
				if (maze[i][j - 1] == '#')
				{
					trcc.push_back(std::tuple<int, int, int>(i, startRow, endRow));
				}
			}
		}
	}

	int trccSize = trcc.size();
	for (int i = 0; i < trccSize; i++)
	{
		int startRow = 0, endRow = 0, startCol = 0, endCol = 0;
		std::tie(startRow, startCol, endCol) = trcc[i];
		endRow = startRow + 1;
		std::cout << "->>>>" << startRow << "-" << startCol << "-" << endCol;


		CubeCreator(vPos, vIdx, startRow, endRow, startCol, endCol);

	}
}




#pragma region First Solution
//Mesh size : V = 4248, I = 2130
//Created model : V = 1416, I = 2130


/*
void Assignment08::createMazeMesh(int row, int col, char** maze) {
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
*/
#pragma endregion


#pragma region Second Solution Failed
/*
typedef std::vector< std::tuple<int, int, int> > tuple;



void RowWallCreator(std::vector<float> vPos, std::vector<int> vIdx, int startRow, int endRow, int targetCol)
{
	vPos.push_back(startRow); vPos.push_back(0); vPos.push_back(targetCol);
	vPos.push_back(endRow); vPos.push_back(0); vPos.push_back(targetCol);
	vPos.push_back(startRow); vPos.push_back(wall_height); vPos.push_back(targetCol);
	vPos.push_back(endRow); vPos.push_back(wall_height); vPos.push_back(targetCol);
	vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
	vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);

	vert_counter += 4;
}


void ColWallCreator(std::vector<float> vPos, std::vector<int> vIdx, int startCol, int endCol, int targetRow)
{
	vPos.push_back(targetRow); vPos.push_back(0); vPos.push_back(startCol);
	vPos.push_back(targetRow); vPos.push_back(0); vPos.push_back(endCol);
	vPos.push_back(targetRow); vPos.push_back(wall_height); vPos.push_back(startCol);
	vPos.push_back(targetRow); vPos.push_back(wall_height); vPos.push_back(endCol);
	vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
	vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);

	vert_counter += 4;
}

void Assignment08::createMazeMesh(int row, int col, char** maze) {


	int n = 1;
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

	tuple tcrr;//ColRowRow
	tuple trcc;//RowColCol
	for (int i = 0; i < row; i++)
	{
		int startRow = 0, endRow = 1;
		for (int j = 0; j < col; j++)
		{
			if (maze[i][j] == '#')
			{
				if (j == 0)
					continue;
				if (maze[i][j - 1] == ' ')
				{
					startRow = j;
				}
				endRow = j + 1;
				if (j == col - 1)
				{
					trcc.push_back(std::tuple<int, int, int>(i + 1, startRow, endRow));
					if (i == 0)
						trcc.push_back(std::tuple<int, int, int>(i, startRow, endRow));
				}
			}
			else
			{
				if (j == 0)
					continue;
				if (maze[i][j - 1] == '#')
				{
					trcc.push_back(std::tuple<int, int, int>(i + 1, startRow, endRow));

					if (i == 0)
						trcc.push_back(std::tuple<int, int, int>(i, startRow, endRow));
				}
			}
		}
	}

	for (int j = 0; j < col; j++)
	{
		int startCol = 0, endCol = 1;
		for (int i = 0; i < row; i++)
		{
			if (maze[i][j] == '#')
			{
				if (j == 0)
					continue;
				if (maze[i][j - 1] == ' ')
				{
					startCol = j;
				}
				endCol = j + 1;
				if (j == col - 1)
				{
					tcrr.push_back(std::tuple<int, int, int>(j + 1, startCol, endCol));
					if (i == 0)
						tcrr.push_back(std::tuple<int, int, int>(j, startCol, endCol));
				}
			}
			else
			{
				if (j == 0)
					continue;
				if (maze[i][j - 1] == '#')
				{
					tcrr.push_back(std::tuple<int, int, int>(i + 1, startCol, endCol));

					if (i == 0)
						tcrr.push_back(std::tuple<int, int, int>(i, startCol, endCol));
				}
			}
		}
	}


	int trccSize = trcc.size();
	for (int i = 0; i < trccSize; i++)
	{
		int targetRow = 0, startCol = 0, endCol = 0;
		std::tie(targetRow, startCol, endCol) = trcc[i];
		std::cout << "->>>>" << targetRow << "-" << startCol << "-" << endCol;


		//ColWallCreator(vPos, vIdx, startCol, endCol, targetRow);
		vPos.push_back(targetRow); vPos.push_back(0); vPos.push_back(startCol);
		vPos.push_back(targetRow); vPos.push_back(0); vPos.push_back(endCol);
		vPos.push_back(targetRow); vPos.push_back(wall_height); vPos.push_back(startCol);
		vPos.push_back(targetRow); vPos.push_back(wall_height); vPos.push_back(endCol);
		vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
		vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);

		vert_counter += 4;

		if (targetRow != 0)
		{
			vPos.push_back(targetRow - 1); vPos.push_back(wall_height); vPos.push_back(startCol);
			vPos.push_back(targetRow - 1); vPos.push_back(wall_height); vPos.push_back(endCol);
			vPos.push_back(targetRow); vPos.push_back(wall_height); vPos.push_back(startCol);
			vPos.push_back(targetRow); vPos.push_back(wall_height); vPos.push_back(endCol);
			vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
			vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);

			vert_counter += 4;
		}
	}

	int tcrrSize = tcrr.size();
	for (int i = 0; i < tcrrSize; i++)
	{
		int targetCol = 0, startRow = 0, endRow = 0;
		std::tie(targetCol, startRow, endRow) = tcrr[i];
		std::cout << "->>>>" << targetCol << "-" << startRow << "-" << endRow;


		//RowWallCreator(vPos, vIdx, startRow, endRow, targetCol);
		vPos.push_back(startRow); vPos.push_back(0); vPos.push_back(targetCol);
		vPos.push_back(endRow); vPos.push_back(0); vPos.push_back(targetCol);
		vPos.push_back(startRow); vPos.push_back(wall_height); vPos.push_back(targetCol);
		vPos.push_back(endRow); vPos.push_back(wall_height); vPos.push_back(targetCol);
		vIdx.push_back(vert_counter); vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2);
		vIdx.push_back(vert_counter + 1); vIdx.push_back(vert_counter + 2); vIdx.push_back(vert_counter + 3);

		vert_counter += 4;
	}

}
*/
#pragma endregion
