#include <iostream>
using std::cout;
using std::endl;
using std::cin;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <stack>
using std::stack;
#include <ctime>

//FORWARD FUNCTION DECLARATIONS
bool fileExists(string file_name);
int makeGrid(int (&grid)[9][9], int (&values)[9][9][10]);
void setEqual(int (&grid1)[9][9], int (&grid2)[9][9]);
bool areEqual(int (&grid1)[9][9], int (&grid2)[9][9]);
void checkCols(int (&grid)[9][9], int (&values)[9][9][10]);
void checkRows(int (&grid)[9][9], int (&values)[9][9][10]);
void checkBoxes(int (&grid)[9][9], int (&values)[9][9][10]);
void onlyInRow(int (&grid)[9][9], int (&values)[9][9][10]);
void onlyInCol(int (&grid)[9][9], int (&values)[9][9][10]);
void onlyInBox(int (&grid)[9][9], int (&values)[9][9][10]);
void groupInRow(int (&grid)[9][9], int (&values)[9][9][10]);
void groupInCol(int (&grid)[9][9], int (&values)[9][9][10]);
void groupInBox(int (&grid)[9][9], int (&values)[9][9][10]);
void nakedPairsRow(int (&grid)[9][9], int (&values)[9][9][10]);
void nakedPairsCol(int (&grid)[9][9], int (&values)[9][9][10]);
void nakedPairsBox(int (&grid)[9][9], int (&values)[9][9][10]);
string makeGuess(const int (&values)[9][9][10]);
void useGuess(string guess, int (&values)[9][9][10], stack<string>& values_stack, stack<string>& guess_stack);
void fixWrongGuess(int& errors, int (&grid)[9][9], int (&values)[9][9][10], stack<string>& values_stack, stack<string>& guess_stack);
int setValues(int (&grid)[9][9], int (&values)[9][9][10]);
int checkValidity(int (&grid)[9][9]);
bool checkIfSolved(int (&grid)[9][9]);
void printGrid(int grid[9][9]);
void printValues(int values[9][9][10]);

int main()
{
	int grid[9][9] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0},  
		{0, 0, 0, 0, 0, 0, 0, 0, 0}};
	int values[9][9][10] = {0};
	int earlier_grid[9][9];
	bool solved = false;	
	int iteration = 1;
	int errors = 0;
	int guesses_made = 0;
	bool grid_unchanging = false;
	int quit = 0;
	stack<string> values_stack;
	stack<string> guess_stack;

	quit = makeGrid(grid, values);
	if(quit)
	{
		return 0;
	}
	cout << endl << endl << "Initial Board: ";
	printGrid(grid);
	clock_t begin = clock();
	while(!solved)
	{
		//create initial grid to see if we are making progress
		if(iteration % 5 == 0)
		{
			setEqual(earlier_grid, grid);
		}

		//eliminate potential values for blank squares depending on 
		//already known squares in the same row/column/box.
		checkRows(grid, values);
		checkCols(grid, values);
		checkBoxes(grid, values);
		
		//if a blank square is the only square in a row/column/box
		//that can be a certain value, then it must be that value
		onlyInRow(grid, values);
		onlyInCol(grid, values);
		onlyInBox(grid, values);

		//when only 2-3 squares in a row/column/box can be a certain value
		//and all elements in the group are in the same box/box/row&column
		//other squares in that box/box/row&column cannot be that value
		groupInRow(grid, values);
		groupInCol(grid, values);
		groupInBox(grid, values);
		
		//when it can be determined that these n values must go in these
		//n positions, but it's impossible to determine which go where
		//example: square A must be 4 or 9 and square B must be 4 or 9
		nakedPairsRow(grid, values);
		nakedPairsCol(grid, values);
		nakedPairsBox(grid, values);

		//set values which have only one possibility
		errors += setValues(grid, values);
		errors += checkValidity(grid);

		//print the grid on each iteration of the loop
		//printGrid(grid);
		//printValues(values);
		
		//halt execution if the puzzle is solved
		solved = checkIfSolved(grid);

		//check if the program cannot make any more progress
		iteration++;
		if(iteration % 5 == 0)
		{
			grid_unchanging = areEqual(earlier_grid, grid);
		}
		//if so, start making some guesses!
		if(grid_unchanging)
		{
			string guess = makeGuess(values);
			guesses_made += 1;
			useGuess(guess, values, values_stack, guess_stack);
			setValues(grid, values);
			grid_unchanging = false;
		}
		//statistically, most of our guesses will be wrong
		fixWrongGuess(errors, grid, values, values_stack, guess_stack);
		errors = 0;
	}
	clock_t end = clock();

	//the puzzle has been solved, so print the results
	cout << "Final Board: ";
	printGrid(grid);
	cout << "Execution Time: " << double(end - begin) / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Guesses Made: " << guesses_made << endl;
	return 0;
}




//https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-usin     g-standard-c-c11-c
bool fileExists(string file_name)
{
	ifstream the_file(file_name.c_str());
    return the_file.good();
}




int makeGrid(int (&grid)[9][9], int (&values)[9][9][10])
{
	//USE FILE FOR TESTING
	string filename = "example_puzzle.txt";
	cout << "Please enter the filename of your sudoku puzzle: ";
	cin >> filename;
	if(fileExists(filename))
	{
		string line;
		ifstream puzzle(filename.c_str());
		for(int i = 0; i < 9; i++)
		{
			puzzle >> line;
			for(int j = 0; j < line.length(); j++)
			{
				grid[i][j] = line[j] - '0';
			}
		}
	}
	else
	{
		cout << "The requested file does not exist in your current directory." << endl;
		return 1;
	}

	int value;
	//for each square on the grid
	for(int row = 0; row < 9; row++)
	{
		for(int col = 0; col < 9; col++)
		{
			value = grid[row][col];
			//blank squares can equal any value
			if(value == 0)
			{
				for(int i = 1; i < 10; i++)
				{
					values[row][col][i] = 1;	//indicates it can be anything
				}
			}
			else
			{
				values[row][col][value] = 1;	//indicates it can only be that number
				values[row][col][0] = 1;		//indicates the value at this position is certain
			}
		}
	}
	return 0;
}



void printGrid(int grid[9][9])
{
	//prints all known values within the grid
	int row_counter = 0;
	cout << endl << endl << "+-------+-------+-------+" << endl;
	for(int row = 0; row < 9; row++)
	{
		int col_counter = 0;
		cout << "| ";
		for(int col = 0; col < 9; col++)
		{
			cout << grid[row][col] << " ";
			col_counter++;
			if(col_counter % 3 == 0)
			{
				cout << "| ";
			}
		}
		cout << endl;
		row_counter++;
		if(row_counter % 3 == 0)
		{
			cout << "+-------+-------+-------+" << endl;
		}
	}
	cout << endl << endl;
}



void printValues(int values[9][9][10])
{
	//prints all possible values for each location within the grid
	//known or given values should have only one possibility
	for(int row = 0; row < 9; row++)
	{
		for(int col = 0; col < 9; col++)
		{
			for(int val = 1; val < 10; val++)
			{
				if(values[row][col][val] != 0)
				{
					cout << val;
				}
			}
			cout << "  ";
		}
		cout << endl;
	}
	cout << endl << endl;
}



void checkCols(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int col = 0; col < 9; col++)
	{
		for(int x = 0; x < 9; x++)
		{
			//this flag means that the value in that position is known
			if(values[x][col][0])
			{
				// no other spaces in that column can be the same number
				for(int a = 0; a < 9; a++)
				{
					//ignore the square we found the number in
					if(a != x)
					{
						values[a][col][ grid[x][col] ] = 0;
					}
				}
			}
		}
	}
}



void checkRows(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int row = 0; row < 9; row++)
	{
		for(int x = 0; x < 9; x++)
		{
			//this flag means that the value in that position is known
			if(values[row][x][0])
			{
				// no other spaces in that row can be the same number
				for(int a = 0; a < 9; a++)
				{
					//ignore the square we found the number in
					if(a != x)
					{
						values[row][a][ grid[row][x] ] = 0;
					}
				}
			}
		}
	}
}



void checkBoxes(int (&grid)[9][9], int (&values)[9][9][10])
{
	//I sincerely apologize for the crazy looping. That was never my intention.
	
	//row and col iteration find the upper-left corner of each box
	for(int row = 0; row < 9; row += 3)
	{
		for(int col = 0; col < 9; col += 3)
		{
			//i and j find the other elements in the box
			for(int i = row; i - row < 3; i++)
			{
				for(int j = col; j - col < 3; j++)
				{
					//this flag means that the value in that position is known
					if(values[i][j][0])
					{
						//no other spaces in that box can be the same number
						for(int a = row; a - row < 3; a++)
						{
							for(int b = col; b - col < 3; b++)
							{
								//ignore the square we found the number in
								if(!(a == i && b == j))
								{
									values[a][b][ grid[i][j] ] = 0;
								}
							}
						}
					}
				}
			}
		}
	}
}



int setValues(int (&grid)[9][9], int (&values)[9][9][10])
{
	/*
	   This function sets the values of any square in the grid which only has one potential number. 
	   It may be important to know that the value in values[x][y][0] is set to 1 if the value has been
	   determined, and remains a 0 otherwise. This way, other functions only need to check one value
	   to determine whether or not a value is already known.
	*/

	int possible_values;
	int value;
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			//only try to set values which are not set yet
			if(!values[i][j][0])
			{
				possible_values = 0;
				for(int k = 1; k < 10; k++)
				{
					if(values[i][j][k])
					{
						possible_values += 1;
						value = k;
					}
				}
				//there should always be at least one potential value for every square
				if(possible_values == 0)
				{
					//cout << "Error at: " << i << " " << j << endl;
					return 1;
				}
				//set the value, and show that it has been set
				if(possible_values == 1)
				{
					grid[i][j] = value;
					values[i][j][0] = 1;
				}
			}
		}
	}
	return 0;
}



bool checkIfSolved(int (&grid)[9][9])
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(grid[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;
}



void onlyInRow(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int row = 0; row < 9; row++)
	{
		int potential_values[10] = {0};
		
		//count the number of times within a row a certain value is possible
		for(int i = 1; i < 10; i++)
		{
			for(int x = 0; x < 9; x++)
			{
				if(values[row][x][i])
				{
					potential_values[i] += 1;
				}
			}
		}
		//once the list is full, check those counts for each number
		for(int i = 1; i < 10; i++)
		{
			//if there is one possibility, then it must be that number
			if(potential_values[i] == 1)
			{
				for(int x = 0; x < 9; x++)
				{
					//find the one place where that occurs
					if(values[row][x][i])
					{
						//show that it can only be that value
						for(int j = 1; j < 10; j++)
						{
							if(j != i)
							{
								values[row][x][j] = 0;
							}
						}
					}
				}
			}
		}
	}
}



void onlyInCol(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int col = 0; col < 9; col++)
	{
		int potential_values[10] = {0};
		
		//count the number of times within a column a certain value is possible
		for(int i = 1; i < 10; i++)
		{
			for(int x = 0; x < 9; x++)
			{
				if(values[x][col][i])
				{
					potential_values[i] += 1;
				}
			}
		}
		//once the list is full, check those counts for each number
		for(int i = 1; i < 10; i++)
		{
			//if there is one possibility, then it must be that number
			if(potential_values[i] == 1)
			{
				for(int x = 0; x < 9; x++)
				{
					//find the one place where that occurs
					if(values[x][col][i])
					{
						//show that it can only be that value
						for(int j = 1; j < 10; j++)
						{
							if(j != i)
							{
								values[x][col][j] = 0;
							}
						}
					}
				}
			}
		}
	}
}



void onlyInBox(int (&grid)[9][9], int (&values)[9][9][10])
{
	//row and col iteration find the upper-left corner of each box
	for(int row = 0; row < 9; row += 3)
	{
		for(int col = 0; col < 9; col += 3)
		{
			int potential_values[10] = {0};
			//count the number of times within a box a certain value is possible
			for(int i = 1; i < 10; i++)
			{
				//a and b find the other elements in the box
				for(int a = row; a - row < 3; a++)
				{
					for(int b = col; b - col < 3; b++)
					{
						if(values[a][b][i])
						{
							potential_values[i] += 1;
						}
					}
				}
			}

			//once the list is full, check those counts for each number
			for(int i = 1; i < 10; i++)
			{
				//if there is one possibility, then it must be that number
				if(potential_values[i] == 1)
				{
					//iterate through the box
					for(int a = row; a - row < 3; a++)
					{
						for(int b = col; b - col < 3; b++)
						{
							//find the one place where that occurs
							if(values[a][b][i])
							{
								//show that it can only be that value
								for(int j = 1; j < 10; j++)
								{
									if(j != i)
									{
										values[a][b][j] = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}



void setEqual(int (&grid1)[9][9], int (&grid2)[9][9])
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			grid1[i][j] = grid2[i][j];
		}
	}
}



bool areEqual(int (&grid1)[9][9], int (&grid2)[9][9])
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(grid1[i][j] != grid2[i][j])
			{
				return false;
			}
		}
	}
	return true;
}



void groupInRow(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int row = 0; row < 9; row++)
	{
		int potential_values[10] = {0};
		int group_member_at[9] = {0};
		bool in_same_box = true;
		int start_col = 0;
		int start_row = row - (row % 3);

		//for each of the nine possible numbers
		for(int i = 1; i < 10; i++)
		{
			//assume this technique is applicable
			in_same_box = true;
			//reset the array determining where our 2-3 values are
			for(int a = 0; a < 9; a++)
			{
				group_member_at[a] = 0;
			}
			//write down how many and where the 2-3 values are
			for(int x = 0; x < 9; x++)
			{
				if(values[row][x][i])
				{
					group_member_at[x] = 1;
					potential_values[i] += 1;
				}
			}
			//only proceed if the technique is applicable to our situation
			if(potential_values[i] == 3 || potential_values[i] == 2)
			{
				if((group_member_at[0] && group_member_at[3]) || (group_member_at[0] && group_member_at[4]) || (group_member_at[0] && group_member_at[5]) || (group_member_at[0] && group_member_at[6]) || (group_member_at[0] && group_member_at[7]) || (group_member_at[0] && group_member_at[8]) || (group_member_at[1] && group_member_at[3]) || (group_member_at[1] && group_member_at[4]) || (group_member_at[1] && group_member_at[5]) || (group_member_at[1] && group_member_at[6]) || (group_member_at[1] && group_member_at[7]) || (group_member_at[1] && group_member_at[8]) || (group_member_at[2] && group_member_at[3]) || (group_member_at[2] && group_member_at[4]) || (group_member_at[2] && group_member_at[5]) || (group_member_at[2] && group_member_at[6]) || (group_member_at[2] && group_member_at[7]) || (group_member_at[2] && group_member_at[8]) || (group_member_at[3] && group_member_at[6]) || (group_member_at[3] && group_member_at[7]) || (group_member_at[3] && group_member_at[8]) || (group_member_at[4] && group_member_at[6]) || (group_member_at[4] && group_member_at[7]) || (group_member_at[4] && group_member_at[8]) ||(group_member_at[5] && group_member_at[6]) || (group_member_at[5] && group_member_at[7]) || (group_member_at[5] && group_member_at[8]))
				{
					in_same_box = false;
				}
				if(in_same_box)
				{
					for(int x = 0; x < 9; x++)
					{
						if(group_member_at[x])
						{
							start_col = x - (x % 3);
							break;
						}
					}
					for(int a = start_row; a < start_row + 3; a++)
					{
						for(int b = start_col; b < start_col + 3; b++)
						{
							if(a != row)
							{
								values[a][b][i] = 0;
							}
						}
					}
				}
			}
		}
	}
}



void groupInCol(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int col = 0; col < 9; col++)
	{
		int potential_values[10] = {0};
		int group_member_at[9] = {0};
		bool in_same_box = true;
		int start_col = col - (col % 3);
		int start_row = 0;

		//for each of the nine possible numbers
		for(int i = 1; i < 10; i++)
		{
			//assume this technique is applicable
			in_same_box = true;
			//reset the array determining where our 2-3 values are
			for(int a = 0; a < 9; a++)
			{
				group_member_at[a] = 0;
			}
			//write down how many and where the 2-3 values are
			for(int x = 0; x < 9; x++)
			{
				if(values[x][col][i])
				{
					group_member_at[x] = 1;
					potential_values[i] += 1;
				}
			}

			//only proceed if the technique is applicable to our situation
			if(potential_values[i] == 3 || potential_values[i] == 2)
			{
				if((group_member_at[0] && group_member_at[3]) || (group_member_at[0] && group_member_at[4]) || (group_member_at[0] && group_member_at[5]) || (group_member_at[0] && group_member_at[6]) || (group_member_at[0] && group_member_at[7]) || (group_member_at[0] && group_member_at[8]) || (group_member_at[1] && group_member_at[3]) || (group_member_at[1] && group_member_at[4]) || (group_member_at[1] && group_member_at[5]) || (group_member_at[1] && group_member_at[6]) || (group_member_at[1] && group_member_at[7]) || (group_member_at[1] && group_member_at[8]) || (group_member_at[2] && group_member_at[3]) || (group_member_at[2] && group_member_at[4]) || (group_member_at[2] && group_member_at[5]) || (group_member_at[2] && group_member_at[6]) || (group_member_at[2] && group_member_at[7]) || (group_member_at[2] && group_member_at[8]) || (group_member_at[3] && group_member_at[6]) || (group_member_at[3] && group_member_at[7]) || (group_member_at[3] && group_member_at[8]) || (group_member_at[4] && group_member_at[6]) || (group_member_at[4] && group_member_at[7]) || (group_member_at[4] && group_member_at[8]) ||(group_member_at[5] && group_member_at[6]) || (group_member_at[5] && group_member_at[7]) || (group_member_at[5] && group_member_at[8]))
				{
					in_same_box = false;
				}
				if(in_same_box)
				{
					for(int x = 0; x < 9; x++)
					{
						if(group_member_at[x])
						{
							start_row = x - (x % 3);
							break;
						}
					}
					for(int a = start_row; a < start_row + 3; a++)
					{
						for(int b = start_col; b < start_col + 3; b++)
						{
							if(b != col)
							{
								values[a][b][i] = 0;
							}
						}
					}
				}
			}
		}
	}
}



void groupInBox(int (&grid)[9][9], int (&values)[9][9][10])
{
	for(int a = 0; a < 9; a += 3)
	{
		for(int b = 0; b < 9; b += 3)
		{
			int potential_values[10] = {0};
			int group_member_at[9] = {0};
			bool in_same_col = true;
			bool in_same_row = true;
			int group_column = 0;
			int group_row = 0;

			//for each of the nine possible numbers
			for(int i = 1; i < 10; i++)
			{
				//assume this technique is applicable
				in_same_col = true;
				in_same_row = true;
				//reset the array determining where our 2-3 values are
				//for within a box, the indices correspond to locations:
				// 0 1 2
				// 3 4 5
				// 6 7 8
				for(int x = 0; x < 9; x++)
				{
					group_member_at[x] = 0;
				}
				//write down how many and where the 2-3 values are
				for(int row = a; row < a + 3; row++)
				{
					for(int col = b; col < b + 3; col++)
					{
						if(values[row][col][i])
						{
							group_member_at[3*(row-a) + (col-b)] = 1;
							potential_values[i] += 1;
						}
					}
				}

				//only proceed if the technique is applicable to our situation
				if(potential_values[i] == 3 || potential_values[i] == 2)
				{
					if((group_member_at[0] && group_member_at[3]) || (group_member_at[0] && group_member_at[4]) || (group_member_at[0] && group_member_at[5]) || (group_member_at[0] && group_member_at[6]) || (group_member_at[0] && group_member_at[7]) || (group_member_at[0] && group_member_at[8]) || (group_member_at[1] && group_member_at[3]) || (group_member_at[1] && group_member_at[4]) || (group_member_at[1] && group_member_at[5]) || (group_member_at[1] && group_member_at[6]) || (group_member_at[1] && group_member_at[7]) || (group_member_at[1] && group_member_at[8]) || (group_member_at[2] && group_member_at[3]) || (group_member_at[2] && group_member_at[4]) || (group_member_at[2] && group_member_at[5]) || (group_member_at[2] && group_member_at[6]) || (group_member_at[2] && group_member_at[7]) || (group_member_at[2] && group_member_at[8]) || (group_member_at[3] && group_member_at[6]) || (group_member_at[3] && group_member_at[7]) || (group_member_at[3] && group_member_at[8]) || (group_member_at[4] && group_member_at[6]) || (group_member_at[4] && group_member_at[7]) || (group_member_at[4] && group_member_at[8]) ||(group_member_at[5] && group_member_at[6]) || (group_member_at[5] && group_member_at[7]) || (group_member_at[5] && group_member_at[8]))
					{
						in_same_row = false;
					}
					if((group_member_at[0] && group_member_at[1]) || (group_member_at[0] && group_member_at[2]) || (group_member_at[0] && group_member_at[4]) || (group_member_at[0] && group_member_at[5]) || (group_member_at[0] && group_member_at[7]) || (group_member_at[0] && group_member_at[8]) || (group_member_at[1] && group_member_at[2]) || (group_member_at[1] && group_member_at[3]) || (group_member_at[1] && group_member_at[5]) || (group_member_at[1] && group_member_at[6]) || (group_member_at[1] && group_member_at[8]) || (group_member_at[2] && group_member_at[3]) || (group_member_at[2] && group_member_at[4]) || (group_member_at[2] && group_member_at[6]) || (group_member_at[2] && group_member_at[7]) || (group_member_at[3] && group_member_at[4]) || (group_member_at[3] && group_member_at[5]) || (group_member_at[3] && group_member_at[7]) || (group_member_at[3] && group_member_at[8]) || (group_member_at[4] && group_member_at[5]) || (group_member_at[4] && group_member_at[6]) || (group_member_at[4] && group_member_at[8]) || (group_member_at[5] && group_member_at[6]) || (group_member_at[5] && group_member_at[7]) ||(group_member_at[6] && group_member_at[7]) || (group_member_at[6] && group_member_at[8]) || (group_member_at[7] && group_member_at[8]))
					{
						in_same_col = false;
					}
					if(in_same_col && !in_same_row)
					{
						for(int x = 0; x < 9; x++)
						{
							if(group_member_at[x])
							{
								group_column = b + (x % 3);
								break;
							}
						}
						for(int x = 0; x < 9; x++)
						{
							if((x - x % 3) != a)
							{
								values[x][group_column][i] = 0;
							}
						}
					}

					if(!in_same_col && in_same_row)
					{
						for(int x = 0; x < 9; x++)
						{
							if(group_member_at[x])
							{
								group_row = a + int(x / 3);
								break;
							}
						}
						for(int x = 0; x < 9; x++)
						{
							if(x - (x % 3) != b)
							{
								values[group_row][x][i] = 0;
							}
						}
					}
			
				}
			}
		}
	}
}




void nakedPairsBox(int (&grid)[9][9], int (&values)[9][9][10])
{
	/*
	  This is a more advanced technique. In its simplest form, there are two boxes of interest and it can be determined 
	  which two numbers are placed there, but the order of the two numbers which go there is unknown. Since the program 
	  at this point does not narrow options unless a concrete guess is established, this function is necessary to reduce
	  the possible values of the squares in the box to reflect that these two numbers MUST go in that specific location.
	*/

	//for every number 1-9
	for(int x = 1; x < 10; x++)
	{
		//for each box in the grid
		for(int box_row = 0; box_row < 9; box_row += 3)
		{
			for(int box_col = 0; box_col < 9; box_col += 3)
			{
				int numbers_in_group[10] = {0};
				numbers_in_group[x] = 1;

				int poss_positions = 0;
				//find the number of places within the box a value can appear
				for(int row = box_row; row < box_row+3; row++)
				{
					for(int col = box_col; col < box_col+3; col++)
					{
						if(values[row][col][x])
						{
							poss_positions += 1;
						}
					}
				}
				
				//see if there are any other numbers with the exact same restrictions
				int group_size = 1;
				for(int y = x+1; y < 10; y++)
				{
					bool same = true;
					for(int row = box_row; row < box_row+3; row++)
					{
						for(int col = box_col; col < box_col+3; col++)
						{
							//check if all potential values are in the same boxes
							if(values[row][col][x] != values[row][col][y])
							{
								same = false;
							}
						}
					}
					if(same)
					{
						numbers_in_group[y] = 1;
						group_size += 1;
					}
				}
				if(group_size == poss_positions)
				{
					//we found a place to use this technique! now implement it
					
					//for each number in the group
					for(int i = 1; i < 10; i++)
					{
						if(numbers_in_group[i])
						{
							//for each square in the box
							for(int row = box_row; row < box_row+3; row++)
							{
								for(int col = box_col; col < box_col+3; col++)
								{
									//if it belongs to the group must be one of the numbers in the group
									if(values[row][col][i])
									{
										//set the values matrix to reflect this
										for(int j = 1; j < 10; j++)
										{
											if(!numbers_in_group[j])
											{
												values[row][col][j] = 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}




void nakedPairsRow(int (&grid)[9][9], int (&values)[9][9][10])
{
	/*
	  This is a more advanced technique. In its simplest form, there are two squares of interest and it can be determined 
	  which two numbers are placed there, but the order of the two numbers which go there is unknown. Since the program 
	  at this point does not narrow options unless a concrete guess is established, this function is necessary to reduce
	  the possible values of the squares in the row to reflect that these two numbers MUST go in that specific location.
	*/

	//for every number 1-9
	for(int x = 1; x < 10; x++)
	{
		//for each box in the grid
		for(int row = 0; row < 9; row++)
		{
			int numbers_in_group[10] = {0};
			numbers_in_group[x] = 1;

			int poss_positions = 0;
			//find the number of places within the box a value can appear
			for(int col = 0; col < 9; col++)
			{
				if(values[row][col][x])
				{
					poss_positions += 1;
				}
			}
			
			//see if there are any other numbers with the exact same restrictions
			int group_size = 1;
			for(int y = x+1; y < 10; y++)
			{
				bool same = true;
				for(int col = 0; col < 9; col++)
				{
					//check if all potential values are in the same boxes
					if(values[row][col][x] != values[row][col][y])
					{
						same = false;
					}
				}
				if(same)
				{
					numbers_in_group[y] = 1;
					group_size += 1;
				}
			}
			if(group_size == poss_positions)
			{
				//we found a place to use this technique! now implement it
				
				//for each number in the group
				for(int i = 1; i < 10; i++)
				{
					if(numbers_in_group[i])
					{
						for(int col = 0; col < 9; col++)
						{
							//if it belongs to the group must be one of the numbers in the group
							if(values[row][col][i])
							{
								//set the values matrix to reflect this
								for(int j = 1; j < 10; j++)
								{
									if(!numbers_in_group[j])
									{
										values[row][col][j] = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}




void nakedPairsCol(int (&grid)[9][9], int (&values)[9][9][10])
{
	/*
	  This is a more advanced technique. In its simplest form, there are two squares of interest and it can be determined 
	  which two numbers are placed there, but the order of the two numbers which go there is unknown. Since the program 
	  at this point does not narrow options unless a concrete guess is established, this function is necessary to reduce
	  the possible values of the squares in the column to reflect that these two numbers MUST go in that specific location.
	*/

	//for every number 1-9
	for(int x = 1; x < 10; x++)
	{
		//for each box in the grid
		for(int col = 0; col < 9; col++)
		{
			int numbers_in_group[10] = {0};
			numbers_in_group[x] = 1;

			int poss_positions = 0;
			//find the number of places within the box a value can appear
			for(int row = 0; row < 9; row++)
			{
				if(values[row][col][x])
				{
					poss_positions += 1;
				}
			}
			
			//see if there are any other numbers with the exact same restrictions
			int group_size = 1;
			for(int y = x+1; y < 10; y++)
			{
				bool same = true;
				for(int row = 0; row < 9; row++)
				{
					//check if all potential values are in the same boxes
					if(values[row][col][x] != values[row][col][y])
					{
						same = false;
					}
				}
				if(same)
				{
					numbers_in_group[y] = 1;
					group_size += 1;
				}
			}
			if(group_size == poss_positions)
			{
				//we found a place to use this technique! now implement it
				
				//for each number in the group
				for(int i = 1; i < 10; i++)
				{
					if(numbers_in_group[i])
					{
						for(int row = 0; row < 9; row++)
						{
							//if it belongs to the group must be one of the numbers in the group
							if(values[row][col][i])
							{
								//set the values matrix to reflect this
								for(int j = 1; j < 10; j++)
								{
									if(!numbers_in_group[j])
									{
										values[row][col][j] = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}




int checkValidity(int (&grid)[9][9])
{
	/*
	   After allowing the program to make guesses, it is possible that we ended up with a "solved" grid that actually
	   violates some of the key rules of sudoku. To make sure that our solution is valid, we use this simple test.
	*/

	int used_values[10] = {0};

	//ROWS
	for(int row = 0; row < 9; row++)
	{
		for(int i = 0; i < 10; i++)
		{
			used_values[i] = 0;
		}
		for(int col = 0; col < 9; col++)
		{
			if(grid[row][col] != 0)
			{
				if(used_values[ grid[row][col] ])
				{
					return 1;
				}
				else
				{
					used_values[ grid[row][col] ] = 1;
				}
			}
		}
	}
	//now we know all rows are valid

	//COLUMNS
	for(int col = 0; col < 9; col++)
	{
		for(int i = 0; i < 10; i++)
		{
			used_values[i] = 0;
		}
		for(int row = 0; row < 9; row++)
		{
			if(grid[row][col] != 0)
			{
				if(used_values[ grid[row][col] ])
				{
					return 1;
				}
				else
				{
					used_values[ grid[row][col] ] = 1;
				}
			}
		}
	}
	//now we know all rows and columns are valid

	//BOXES
	for(int box_row = 0; box_row < 9; box_row += 3)
	{
		for(int box_col = 0; box_col < 9; box_col += 3)
		{
			for(int i = 0; i < 10; i++)
			{
				used_values[i] = 0;
			}
			for(int row = box_row; row < box_row+3; row++)
			{
				for(int col = box_col; col < box_col+3; col++)
				{
					if(grid[row][col] != 0)
					{
						if(used_values[ grid[row][col] ])
						{
							return 1;
						}
						else
						{
							used_values[ grid[row][col] ] = 1;
						}
					}
				}
			}
		}
	}
	//now we know that our entire solution is valid!
	return 0;
}




string makeGuess(const int (&values)[9][9][10])
{
	/* In order to improve the efficiency of the program, it is a good idea to have the program make a guess
	   in one of the squares in which the fewest options remain. This program will return the first square it sees
	   with only two options or whichever square has the fewest options remaining. This information is packed into 
	   a string, where the first digit is the row of the guess, the second digit is the column of the guess, and the
	   third digit is the actual number that was guessed.
	*/
	int min_options = 9;
	string guess = "000";
	for(int row = 0; row < 9; row++)
	{
		for(int col = 0; col < 9; col++)
		{
			if(!values[row][col][0])
			{
				int options_here = 0;
				int an_option = 0;
				for(int x = 1; x < 10; x++)
				{
					if(values[row][col][x])
					{
						options_here++;
						an_option = x;
					}
				}
				if(options_here < min_options)
				{
					min_options = options_here;
					guess[0] = char('0' + row);
					guess[1] = char('0' + col);
					guess[2] = char('0' + an_option);
				}
				if(min_options == 2)
				{
					return guess;
				}
			}
		}
	}
	return guess;
}




void useGuess(string guess, int (&values)[9][9][10], stack<string>& values_stack, stack<string>& guess_stack)
{
	//convert current grid to string because I can't figure out how the save a multidimensional array returned from stack.top()
	string values_string = "";
	for(int row = 0; row < 9; row++)
	{
		for(int col = 0; col < 9; col++)
		{
			for(int num = 0; num < 10; num++)
			{
				values_string = values_string + char('0' + values[row][col][num]);
			}
		}
	}

	//save a copy of the known correct value grid before making a (possibly incorrect) guess
	guess_stack.push(guess);
	values_stack.push(values_string);

	//implement the guess
	int row = char(guess[0] - '0');
	int col = char(guess[1] - '0');
	int num = char(guess[2] - '0');
	
	//set all possible values to zero
	for(int i = 1; i < 10; i++)
	{
		values[row][col][i] = 0;
	}
	//except our guess
	values[row][col][num] = 1;
}





void fixWrongGuess(int& errors, int (&grid)[9][9], int (&values)[9][9][10], stack<string>& values_stack, stack<string>& guess_stack)
{
	if(errors == 0)
	{
		return;
	}

	//if there is an error, reset the values grid to where we last made a guess
	string values_string = values_stack.top();
	values_stack.pop();

	//convert string to 3D matrix
	int row = 0;
	int col = 0;
	int num = 0;
	for(int i = 0; i < values_string.length(); i++)
	{
		row = i / 90;
		col = (i % 90) / 10;
		num = i % 10;
		values[row][col][num] = values_string[i] - '0';
	}

	int poss_values = 0;
	//update the grid we show the user to reflect these changes
	for(int row = 0; row < 9; row++)
	{
		for(int col = 0; col < 9; col++)
		{
			poss_values = 0;
			//if a values is known (there should be one option)
			if(values[row][col][0])
			{
				for(int i = 1; i < 10; i++)
				{
					if(values[row][col][i])
					{
						grid[row][col] = i;
						poss_values += 1;
					}
				}
			}
			else
			{
				grid[row][col] = 0;
			}
			if(poss_values > 1)
			{
				cout << "Something is definitely wrong." << endl;
			}
		}
	}
	
	string last_guess = guess_stack.top();
	guess_stack.pop();

	//unpack the data in the guess string
	row = char(last_guess[0] - '0');
	col = char(last_guess[1] - '0');
	num = char(last_guess[2] - '0');

	//we have learned that our guess was wrong
	values[row][col][num] = 0;

	//hopefully this fixed the error
	errors = 0;
	setValues(grid, values);
}
