#include <iostream>
#include <string>
#include <fstream>

using namespace std;


//FORWARD FUNCTION DECLARATIONS
void makeGrid(int (&grid)[9][9], int (&values)[9][9][10]);
void checkCols(int (&grid)[9][9], int (&values)[9][9][10]);
void checkRows(int (&grid)[9][9], int (&values)[9][9][10]);
void checkBoxes(int (&grid)[9][9], int (&values)[9][9][10]);
void onlyInRow(int (&grid)[9][9], int (&values)[9][9][10]);
void onlyInCol(int (&grid)[9][9], int (&values)[9][9][10]);
void onlyInBox(int (&grid)[9][9], int (&values)[9][9][10]);
void setValues(int (&grid)[9][9], int (&values)[9][9][10]);
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
	bool solved = false;	

	makeGrid(grid, values);
	printGrid(grid);
	while(!solved)
	{
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

		//set values which have only one possibility
		setValues(grid, values);

		//print the grid on each iteration of the loop
		printGrid(grid);
		
		//halt execution if the puzzle is solved
		solved = checkIfSolved(grid);
	}
	printValues(values);
	return 0;
}


void makeGrid(int (&grid)[9][9], int (&values)[9][9][10])
{
	//USE FILE FOR TESTING
	string line;
	ifstream puzzle;
	puzzle.open("sudoku_22.txt");
	for(int i = 0; i < 9; i++)
	{
		puzzle >> line;
		for(int j = 0; j < line.length(); j++)
		{
			grid[i][j] = line[j] - '0';
		}
	}

	//USE USER INPUT FOR FINAL VERSION
	/*bool correct_input = false;
	string line;
	while(!correct_input)
	{
		start:
		cout << "Please enter the sudoku board, with nine numbers per line and zeros representing blank spaces." << endl;
		
		for(int lines = 0; lines < 9; lines++)
		{
			cin >> line;
			//check all lines are nine digits long
			if(line.length() != 9)
			{
				cout << "You must enter nine numbers on each line." << endl << endl;
				goto start;
			}
			//check all values given are digits 0-9
			for(int i = 0; i < 9; i++)
			{
				if(line[i] - '0' < 0 || line[i] - '0' > 9)
				{
					cout << "You must enter only digits 0-9." << endl << endl;
					goto start;
				}
			}
			//the use input has passed both tests, so fill in the grid now
			correct_input = true;
			for(int i = 0; i < 9; i++)
			{
				grid[lines][i] = line[i] - '0';
			}
		}
	}
	*/


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



void setValues(int (&grid)[9][9], int (&values)[9][9][10])
{
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
				if(possible_values == 0)
				{
					cout << "ERROR: This puzzle is unsolveable, or a mistake was made." << endl;
				}
				if(possible_values == 1)
				{
					grid[i][j] = value;
					values[i][j][0] = 1;
				}
			}
		}
	}
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
			if(potential_values[i] == 0)
			{
				cout << "ERROR: This puzzle is unsolveable, or a mistake was made." << endl;
	
			}
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
			if(potential_values[i] == 0)
			{
				cout << "ERROR: This puzzle is unsolveable, or a mistake was made." << endl;
	
			}
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
				if(potential_values[i] == 0)
				{
					cout << "ERROR: This puzzle is unsolveable, or a mistake was made." << endl;
		
				}
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