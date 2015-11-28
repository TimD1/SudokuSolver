#include <iostream>
#include <string>
#include <fstream>

using namespace std;


//FORWARD FUNCTION DECLARATIONS
void makeGrid(int (&grid)[9][9], int (&values)[9][9][10]);
void checkCols(int (&grid)[9][9], int (&values)[9][9][10]);
void checkRows(int (&grid)[9][9], int (&values)[9][9][10]);
void checkBoxes(int (&grid)[9][9], int (&values)[9][9][10]);
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
	while(!solved)
	{
		checkRows(grid, values);
		checkCols(grid, values);
		checkBoxes(grid, values);
		setValues(grid, values);
		solved = checkIfSolved(grid);
		printGrid(grid);
		
	}
	//printValues(values);
	return 0;
}


void makeGrid(int (&grid)[9][9], int (&values)[9][9][10])
{
	//USE FILE FOR TESTING
	string line;
	ifstream puzzle;
	puzzle.open("README.md");
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
	int known_number;
	for(int col = 0; col < 9; col++)
	{
		for(int x = 0; x < 9; x++)
		{
			known_number = 0;
			//this flag means that the value in that position is known
			if(values[x][col][0])
			{
				for(int i = 1; i < 10; i++)
				{
					if(values[x][col][i])
					{
						known_number = i;
						break;
					}
				}
			}
			
			//if a known value occupies that position
			if(known_number != 0)
			{
				for(int a = 0; a < 9; a++)
				{
					// no other values in that row can be the same number
					if(a != x)
					{
						values[a][col][known_number] = 0;
					}
				}
			}
		}
	}
}



void checkRows(int (&grid)[9][9], int (&values)[9][9][10])
{
	int known_number;
	for(int row = 0; row < 9; row++)
	{
		for(int x = 0; x < 9; x++)
		{
			//this flag means that the value in that position is known
			if(values[row][x][0])
			{
				for(int a = 0; a < 9; a++)
				{
					// no other values in that row can be the same number
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
					//if a value is known for certain
					if(values[i][j][0])
					{
						//no other spaces in that box can be the same value
						for(int a = row; a - row < 3; a++)
						{
							for(int b = col; b - col < 3; b++)
							{
								//ignore the square we found the values in
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
					cout << "ERROR: This puzzle is unsolveable, or a msitake was made." << endl;
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
